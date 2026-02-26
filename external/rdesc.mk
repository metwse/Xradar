TARGET_DIR = target
export PREFIX = $(PWD)

default: include/rdesc
	@echo > /dev/null

$(TARGET_DIR)/rdesc: $(TARGET_DIR)
	git clone https://github.com/metwse/rdesc --branch v0.2.x $@

include/rdesc: $(TARGET_DIR)/rdesc
	$(MAKE) -C $(TARGET_DIR)/rdesc FEATURES='full' install

$(TARGET_DIR) $(PREFIX):
	mkdir -p $@

.PHONY: default
