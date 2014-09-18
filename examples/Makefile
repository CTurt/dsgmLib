EXAMPLES = $(wildcard */)

.PHONY: all

all: $(EXAMPLES)

$(EXAMPLES) : %/ : */%.nds

*/%.nds:
	@echo $(firstword $(subst ., ,$(word 2,$(subst /, ,$@))))
	@make -C $(firstword $(subst ., ,$(word 2,$(subst /, ,$@))))
	@echo