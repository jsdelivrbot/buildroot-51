#!/bin/sh
  
cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config

cp $BASE_DIR/../custom-scripts/Sweb_server $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/Sweb_server

cp -rf $BASE_DIR/../custom-scripts/serverscripts $BASE_DIR/target/usr/bin
chmod +xr $BASE_DIR/target/usr/bin/serverscripts

cp $BASE_DIR/../custom-scripts/thread_runner $BASE_DIR/target/usr/bin

#Compile the syscall_test.c
BUILDROOT_DIR=$BASE_DIR/..
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/bin/syscall_test $BUILDROOT_DIR/custom-scripts/syscall_test.c

make -C $BASE_DIR/../../hello_world_driver/

#Compile the disk_access_test.c
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/bin/disk_access_test $BUILDROOT_DIR/custom-scripts/disk_access_test.c


make -C $BASE_DIR/../../LabSisOpT2/

