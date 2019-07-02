PULP_PROPERTIES += pulp_chip_family pulp_chip
include $(PULP_SDK_HOME)/install/rules/pulp_properties.mk

FLASH_TYPE ?= HYPER_FLASH

APP = test
APP_SRCS += test.c
APP_CFLAGS += -O3 -g -DFLASH_TYPE=$(FLASH_TYPE) -Werror
USE_PMSIS_BSP=1

ifeq '$(pulp_chip_family)' 'gap'
ifeq '$(FLASH_TYPE)' 'HYPER_FLASH'
RUNNER_CONFIG = $(CURDIR)/config.ini
endif
endif

ifeq '$(pulp_chip)' 'wolfe'
RUNNER_CONFIG = $(CURDIR)/config-wolfe.ini
endif


include $(PULP_SDK_HOME)/install/rules/pmsis.mk
