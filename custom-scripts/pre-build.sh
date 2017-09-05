#!/bin/sh
  
cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config

cp $BASE_DIR/../custom-scripts/Sweb_server $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/Sweb_server

cp -rf $BASE_DIR/../custom-scripts/serverscripts $BASE_DIR/target/usr/bin
chmod +xr $BASE_DIR/target/usr/bin/serverscripts

