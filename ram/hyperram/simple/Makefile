-include $(RULES_DIR)/properties.mk

override runner_args += --config-opt=runner/peripherals=true

RAM_TYPE ?= HYPERRAM

APP = test
APP_SRCS = test.c
APP_CFLAGS += -O3 -g

ifeq '$(RAM_TYPE)' 'HYPERRAM'
APP_CFLAGS += -DUSE_HYPERRAM
else
APP_CFLAGS += -DUSE_SPIRAM
CONFIG_SPIRAM = 1
endif



include $(RULES_DIR)/pmsis_rules.mk
