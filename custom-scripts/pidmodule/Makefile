obj-m := seg.o
BUILDROOT_DIR := ../buildroot
KDIR := $(BUILDROOT_DIR)/output/build/linux-custom
COMPILER := $(BUILDROOT_DIR)/output/host/bin/i686-buildroot-linux-uclibc-gcc

all:
	$(MAKE) -C $(KDIR) M=$$PWD
	$(MAKE) -C $(KDIR) M=$$PWD modules_install INSTALL_MOD_PATH=../../target
	$(COMPILER) -o seg seg.c
	cp seg $(BUILDROOT_DIR)/output/target/bin
	
	
	
clean:
	rm -f *.o *.ko
	rm -f modules.order
	rm -f Module.symvers
    
