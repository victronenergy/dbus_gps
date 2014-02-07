dbus_gps
========

daemon on ccgx that reads info from a usb/serial gps in NMEA0183 format and publishes it on dbus.


To build and compile:

# create the make file
  cd software
  ./ext/velib/mk/init_build.sh
  
# now, there are two options. One build on a pc, to run on a pc:
export CC=gcc
make

# or, option two, setup the environment for cross compiling:
. /opt/bpp3/current/environment-setup-armv7a-vfp-neon-bpp3-linux-gnueabi
export CC=gcc
export CROSS_COMPILE=arm-bpp3-linux-gnueabi-
export TARGET=bpp3

# and then compile:
make

