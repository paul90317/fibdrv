CONFIG_MODULE_SIG = n
TARGET_MODULE := fibdrv

obj-m := $(TARGET_MODULE).o
$(TARGET_MODULE)-y := module/$(TARGET_MODULE).o module/bignum/bn.o
ccflags-y := -std=gnu99 -Wno-declaration-after-statement

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

GIT_HOOKS := .git/hooks/applied

all: $(GIT_HOOKS) client fibget single
	$(MAKE) -C $(KDIR) M=$(PWD) modules

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	$(RM) clients/client out clients/time_get clients/single

load:
	sudo insmod $(TARGET_MODULE).ko
unload:
	sudo rmmod $(TARGET_MODULE) || true >/dev/null

client: clients/client.o clients/utils.o
	$(CC) -o clients/client $^

fibget: clients/fibget.o clients/utils.o
	$(CC) -o clients/fibget $^
single: clients/single.o clients/utils.o
	$(CC) -o clients/single $^
format:
	clang-format -i clients/*.[ch]
	clang-format -i module/*.[ch]
	clang-format -i module/*/*.[ch]

PRINTF = env printf
PASS_COLOR = \e[32;01m
NO_COLOR = \e[0m
pass = $(PRINTF) "$(PASS_COLOR)$1 Passed [-]$(NO_COLOR)\n"

time: all
	$(MAKE) unload
	$(MAKE) load
	@python3 scripts/driver.py
	$(MAKE) unload

compare: all
	$(MAKE) unload
	$(MAKE) load
	@python3 scripts/compare.py
	$(MAKE) unload

setcpu:
	isolcpus=0
	sudo sh -c "echo 0 > /proc/sys/kernel/randomize_va_space"
	sudo sh -c "echo 1 > /sys/devices/system/cpu/intel_pstate/no_turbo"
	sudo sh -c "echo off > /sys/devices/system/cpu/smt/control"
	sudo sh performance.sh

check: all
	$(MAKE) unload
	$(MAKE) load
	sudo ./clients/client > out
	$(MAKE) unload
	@scripts/verify.py && $(call pass)
