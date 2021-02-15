# dbus_gps

Daemon on Venus OS that reads info from a usb/serial gps in NMEA0183 format and
publishes it on dbus. More information on Venus OS [here](
https://github.com/victronenergy/venus/wiki).

Building depens on:
 - python
 - dbus libs + headers
 - libevent libs + headers

For cross compiling, installing the [Venus SDK](https://updates.victronenergy.com/feeds/venus/release/sdk/) is sufficient, it
contains above dependencies. For a normal compile, to run 
the project on a pc, Google will help you :).

##### To build and compile

Create the make file
```bash
cd software
./configure
```
  
Now, there are two options:
 1. normal compile: build on a PC to execute on a PC
 2. cross compile: build on a PC, to later execute on a GX device.

##### Option 1 - normal compile
```bash
export CC=gcc
make
```

##### Option 2 - cross compile for running on a GX device
See <https://github.com/victronenergy/venus/wiki/howto-install-and-use-the-sdk>.
And after installation of the SDK, follow the procedure in chapter 2.5,
"Cross compiling velib projects".
