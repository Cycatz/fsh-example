# Define your module name
MODULE_NAME := fsh_example

# Define your custom_objs
FSH_CUSTOM_OBJS := fsh_example_mod.o


FSH_DIR := $(src)/fsh
ifneq ($(KERNELRELEASE),)

# The fsh.mk has already defined obj-m and ccflags-y for you
# You can use '+=' to append your custom args here
# For example:
#   ccflags-y += -Wall
#   $(MODULE_NAME)-objs += $(ANOTHER_CUSTOM_OBJS)
include $(FSH_DIR)/fsh.mk

else # ifneq ($(KERNELRELEASE),)

PWD := $(CURDIR) 

.PHONY: all

all: build

build:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean 

endif

install:
	insmod $(MODULE_NAME).ko

uninstall:
	rmmod $(MODULE_NAME) 
