# Xradar
Xradar is a C++20 radar signal processing framework built around a
plugin-based, event-driven pipeline.

## Architecture
Xradar employs a custom thread pool, optimized for parallel computing. The
`execution` synchronizes processing pipeline, providing parallel computing for
processing branches.

The system is component based, and it can be extended using dynamic libraries
*at runtime*. The pipeline handles 3 kinds of components:

1. Producer: The root signal source. Initiates a processing pipeline. This
   pipeline may branch out depending on configuration.
2. Middleware: Signal processor middleware. One or more processor can be
   chained.
3. Consumer: End of the signal pipeline. Used for visualization/result
   recording.

A middleware component can apply backpressure to a producer by sending
delay-hint messages back to the root producer. Xradar provides a generic
interface for this backward communication; however, the specific handling of
these messages is entirely implementation-defined

### Pipeline
The processing pipeline is initiated by the `data_callback` of a producer
(input source). Signals are propagated and processed through the pipeline,
which is automatically parallelized whenever possible

```txt
source ----> middleware1 ----> middleware2 ----> visualizer
                           |                 |
                           --> middleware3 ---
```

Here, the `source` triggers signal propagation. First, `middleware1` runs; then,
`middleware2` and `middleware3` run *in parallel*. Finally, the `visualizer` is
fed the results from `middleware2` and `middleware3` in the order of their
construction.

Xradar shell introduces a fluent DSL for wiring components:
```sh
begin-pipeline processing_pipeline1

set-producer source -t mrm_radar  # Input triggers pipeline

add-middleware middleware1 -t cfar  # Chain processor middleware
add-middleware middleware2 -t ...
add-middleware middleware3 -t ...

add-consumer visualizer -t x11  # One or more consumer may be added
# i.e. for recording one may add another consumer

# Connection network
connect source -i middleware1

connect middleware2 -i middleware1
connect middleware3 -i middleware2

connect visualizer -i middleware2 -i middleware3

build-pipeline

# ...

# Run the pipeline
start-pipeline processing_pipeline1
```
