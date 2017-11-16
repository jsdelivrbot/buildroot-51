#!/bin/sh
  
cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config

cp $BASE_DIR/../custom-scripts/Sweb_server $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/Sweb_server

cp -rf $BASE_DIR/../custom-scripts/serverscripts $BASE_DIR/target/usr/bin
chmod +xr $BASE_DIR/target/usr/bin/serverscripts

#Compile the syscall_test.c
BUILDROOT_DIR=$BASE_DIR/..
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/bin/syscall_test $BUILDROOT_DIR/custom-scripts/syscall_test.c

make -C $BASE_DIR/../../hello_world_driver/

#Compile the disk_access_test.c
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/bin/disk_access_test $BUILDROOT_DIR/custom-scripts/disk_access_test.c


make -C $BASE_DIR/../../LabSisOpT2/

make -C $BASE_DIR/../custom-scripts/pidmodule/

#Compile my_sched.c
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/bin/thread_runner $BUILDROOT_DIR/custom-scripts/my_sched.c -lpthread

#Compile seg.c
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/bin/seg $BUILDROOT_DIR/custom-scripts/seg.c

echo "tracefs       /sys/kernel/tracing       tracefs defaults        0       0" >> $BASE_DIR/target/etc/fstab
