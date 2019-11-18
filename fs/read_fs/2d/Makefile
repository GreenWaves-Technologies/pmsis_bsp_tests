-include $(RULES_DIR)/properties.mk

FS_TYPE ?= read_fs

USE_PMSIS_BSP=1

FILE0= flash_file_0.bin
FILE1 = flash_file_1.bin
FILES = $(FILE0) $(FILE1)

ifeq '$(FS_TYPE)' 'host'
FILE0_PATH = $(CURDIR)/$(FILE0)
FILE1_PATH = $(CURDIR)/$(FILE1)
APP_CFLAGS += -DFS_HOST
export GAP_USE_OPENOCD=1
else
PLPBRIDGE_FLAGS += -f $(FILES) -jtag
override runner_args += $(foreach file,$(FILES), --config-opt=flash/fs/files=$(CURDIR)/$(file))
FILE0_PATH = $(FILE0)
FILE1_PATH = $(FILE1)
APP_CFLAGS += -DFS_READ_FS
endif

APP_CFLAGS += -DFILE0="$(FILE0_PATH)"
APP_CFLAGS += -DFILE1="$(FILE1_PATH)"

APP = test
APP_SRCS = test.c
APP_CFLAGS += -O3 -g

ifdef USE_CLUSTER
APP_CFLAGS += -DUSE_CLUSTER
endif


include $(RULES_DIR)/pmsis_rules.mk
