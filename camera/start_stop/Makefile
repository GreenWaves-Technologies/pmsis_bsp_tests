APP = test
APP_SRCS += test.c
APP_CFLAGS += -O3 -g #-DASYNC
USE_PMSIS_BSP=1

RUNNER_CONFIG = $(CURDIR)/config.ini

override runner_args += --config-opt=camera/image-stream=$(CURDIR)/imgTest0.pgm
override runner_args += --config-opt=runner/peripherals=true

PMSIS_OS ?= pulpos
BOARD_NAME ?= gapuino

ifeq '$(BOARD_NAME)' 'gapuino'
APP_CFLAGS += -DGAPUINO
ifeq ('$(PMSIS_OS)', 'pulpos')
APP_CFLAGS += -DCONFIG_HIMAX
endif
endif

ifeq '$(BOARD_NAME)' 'gapoc_a'
PLPBRIDGE_FLAGS += -ftdi
APP_CFLAGS += -DGAPOC_A
ifeq ('$(PMSIS_OS)', 'pulpos')
APP_CFLAGS += -DCONFIG_MT9V034
endif
#APP_CFLAGS += -DUSE_BRIDGE
endif

include $(RULES_DIR)/pmsis_rules.mk
