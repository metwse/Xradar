#include "../include/components.hpp"
#include "../include/evloop.hpp"
#include "../include/pipeline.hpp"

#include <any>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>


class worker : public evloop::base_event {
public:
    worker(auto id_, auto component_, auto &&inputs_, auto data_callback_)
        : id { id_ },
          component { component_ },
          inputs { inputs_ },
          data_callback { data_callback_ }
    {}

    ~worker() override = default;

    void operator()(std::shared_ptr<evloop::evloop>) override {
        /* TODO: use component::kind instead */
        if (auto middleware = dynamic_cast<component::base_middleware *>(component.get());
            middleware != NULL) {

            data_callback(id, middleware->process(std::move(inputs)));
        } else {
            auto consumer = dynamic_cast<component::base_consumer *>(component.get());
            consumer->send(std::move(inputs));
        }
    }

private:
    size_t id;

    std::shared_ptr<component::base_component> component;

    std::vector<std::shared_ptr<std::any>> inputs;

    std::function<void(size_t, std::unique_ptr<std::any>)> data_callback;
};

class execution : std::enable_shared_from_this<execution> {
private:
    struct token { explicit token() = default; };

public:
    execution(token, auto p_, auto data)
        : p { p_ }
        { result_map[p->producer_id] = std::shared_ptr { std::move(data) }; }

    static std::shared_ptr<execution> create(auto p, auto data)
        { return std::make_shared<execution>(token {}, p, std::move(data)); }

    void next_step() {
        std::lock_guard guard { m };

        auto &step = p->order.at(step_counter);
        awaiting_middleware = 0;

        auto shared_this = shared_from_this();

        for (auto id : step) {
            std::shared_ptr<component::base_component> component;
            std::vector<std::shared_ptr<std::any>> inputs;

            if (p->middleware.contains(id)) {
                component = p->middleware.at(id);
                awaiting_middleware++;
            } else {
                component = p->consumers.at(id);
            }

            auto &input_ids = p->input_table.at(id);
            inputs.reserve(input_ids.size());

            for (auto input_id : input_ids)
                inputs.push_back(result_map.at(input_id));

            p->evloop->push_event(
                std::make_unique<worker>(
                    id,
                    component,
                    inputs,
                    [shared_this](size_t id, std::unique_ptr<std::any> data)
                        { shared_this->handle_response(id, std::move(data)); }
                )
            );
        }

        step_counter++;
    }

    void handle_response(size_t id, std::unique_ptr<std::any> data) {
        {
            std::lock_guard guard { m };

            awaiting_middleware--;

            result_map[id] = std::move(data);
        }

        if (awaiting_middleware == 0)
            next_step();
    }

private:
    std::mutex m;
    std::shared_ptr<pipeline::pipeline> p;

    size_t step_counter {};
    size_t awaiting_middleware;

    std::map<size_t, std::shared_ptr<std::any>> result_map;
};


void pipeline::pipeline::initiate(std::unique_ptr<std::any> data)
    { execution::create(shared_from_this(), std::move(data))->next_step(); }
