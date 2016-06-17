# miyabi
ifeq ($(CONFIG_MACH_EBZ),y)
obj-$(CONFIG_SECURITY_MIYABI)		+= miyabi.o miyabi_common.o miyabi_sandbox.o miyabi_sysfs.o mnhelper.o
else
obj-$(CONFIG_SECURITY_MIYABI)		+= miyabi.o miyabi_common.o miyabi_sandbox.o miyabi_sysfs.o
endif

ifneq (, $(filter F%, $(SH_BUILD_ID)))
EXTRA_CFLAGS += -DLOCAL_MIYABI_FBUILD
endif

