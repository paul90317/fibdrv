CONFIG_MODULE_SIG = n
TARGET_MODULE := fibdrv

obj-m := $(TARGET_MODULE).o
$(TARGET_MODULE)-y := module/$(TARGET_MODULE).o module/bignum/bn.o
ccflags-y := -std=gnu99 -Wno-declaration-after-statement

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

GIT_HOOKS := .git/hooks/applied

all: $(GIT_HOOKS) client time_get
	$(MAKE) -C $(KDIR) M=$(PWD) modules

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	$(RM) clients/client out clients/time_get

load:
	sudo insmod $(TARGET_MODULE).ko
unload:
	sudo rmmod $(TARGET_MODULE) || true >/dev/null

client: clients/client.o clients/utils.o
	$(CC) -o clients/client $^

time_get: clients/time_get.o
	$(CC) -o clients/time_get $^

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
	isolcpus=0
	sudo taskset 0x1 ./clients/time_get > out
	$(MAKE) unload
	@python3 scripts/time_get.py

check: all
	$(MAKE) unload
	$(MAKE) load
	sudo ./clients/client > out
	$(MAKE) unload
	@scripts/verify.py && $(call pass)
