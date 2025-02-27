# dbus_gps

Venus OS daemon that reads [NMEA0183](https://en.wikipedia.org/wiki/NMEA_0183) GGA, GNS and RMC sentences from a usb/serial gps and publishes it on dbus.
More information on Venus OS [here](https://github.com/victronenergy/venus/wiki).

Building depends on:
 - python
 - dbus libs + headers
 - libevent libs + headers

For cross compiling, installing the [Venus SDK](https://updates.victronenergy.com/feeds/venus/release/sdk/) is sufficient, it
contains above dependencies. For a normal compile, to run the project on a pc, Google will help you installing above mentioned
dependencies

### Step 1. cloning incl. submodule

```
$ git clone https://github.com/victronenergy/dbus_gps.git
Cloning into 'dbus_gps'...
remote: Enumerating objects: 53, done.
remote: Counting objects: 100% (53/53), done.
remote: Compressing objects: 100% (35/35), done.
remote: Total 586 (delta 24), reused 35 (delta 16), pack-reused 533
Receiving objects: 100% (586/586), 113.58 KiB | 0 bytes/s, done.
Resolving deltas: 100% (340/340), done.
Checking connectivity... done.

$ cd ./dbus_gps

$ git submodule update --init
Submodule 'software/ext/velib' (git@github.com:victronenergy/velib.git) registered for path 'software/ext/velib'
Cloning into 'software/ext/velib'...
remote: Enumerating objects: 528, done.
remote: Counting objects: 100% (528/528), done.
remote: Compressing objects: 100% (186/186), done.
remote: Total 38046 (delta 312), reused 439 (delta 270), pack-reused 37518
Receiving objects: 100% (38046/38046), 34.83 MiB | 13.68 MiB/s, done.
Resolving deltas: 100% (28475/28475), done.
Checking connectivity... done.
Submodule path 'software/ext/velib': checked out '0c24aa8f2e894a664f5305ba903000a936e21886'
```

### Step 2. Building

First, create the make file:
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

### Step 3. Copy it over to your GX Device

Typically, one would use scp for this. And see [here](https://www.victronenergy.com/live/ccgx:root_access)
for how to obtain root access.

### Step 4. Running

Here, be aware that the `serial-starter`, aka the plug-and-play system on Venus OS,
will have already automatically started dbus_gps. More info about that
[here](https://github.com/victronenergy/venus/wiki/howto-add-a-driver-to-Venus#3-installing-a-driver).
Therefore, two options:

1. Copy your binary over the existing one in `/opt/victronenergy/gps_dbus`. And then restart that service, 
use `svc -t somethingehere` and then look at the log output, using either
`cat /data/log/andsoforth | tai64nlocal` or `tail -F /data/log/andsoforth | tai64nlocal`. See the
[Venus OS commandline](https://github.com/victronenergy/venus/wiki/commandline-introduction)
manual for usage of `svc` and why piping through tai64nlocal helps when looking at log files.

2. Stop the serial starter `svc -d /service/serial-starter`, and then just start the binary using the
right commandline arguments, ie. as a minimum the right serial port:

```
# ./gps-dbus/gps_dbus --help
./gps-dbus/gps_dbus

  -t, --timeout
   The maximum time in seconds to wait for a device first connection before exiting (error 129).
   0 deactivates the timeout. Default is 0.
   Only used for initial connection. Once connected, the device is declared disconnected
   after 5 seconds without incoming data (error 130).
   
  -b, --baud
   The baud rate of the serial port.
   When omitted the baud rate is set to 4800.

COMMON OPTIONS:

  --banner
   Print the version during startup.

  --dbus address   Use specified address to connect to the dbus, e.g.
     - session
     - system
     - unix:abstract=/tmp/dbus-O4H8uXQdeA

  -h, --help
   Print this help.

  --log-before <count>
   When set to non zero <count> number of log lines, which are
   not enabled themselves, are nevertheless shown before an
   log which is enabled. For example an error message will be
   preceeded by <count> warning and info messages which occured
   before the error. Combined with -v also warning messages will
   be preceeded by info messages

  --log-after <count>
   Like -log-before, but temporarily enables tracing of disabled messages
   after a warning or error occured.

  -s, --serial
   The serial port to use (default /dev/ttyS0).

  -v --verbose
   Enable log messages. Specifying multiple times increases verbosity: for INFO level, use -vv.

  -V --version
   Print the version number.

Victron Energy B.V.
```
