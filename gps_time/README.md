# gps-time

Copyright (c) 2021 LHardwick-git
Licensed under the BSD 3-Clause license. 
See LICENSE file in the project root for full license information.

USE CASE
When running Venus OS on a Raspberry Pi there is no real time clock.
As my application is on a boat there may be no internet connection.
In this case the local device time is not set at startup so logs are created with the wrong time stamps.

SOLUTION

This is a shell script that runs at startup and reads the time from the data stream from a USB GPS device. If time information is found in the data stream it is read and used to set the device local time. As a result Venus OS logs are stamped with the correct time.

If the USB GPS device is not connected or not producing a data stream with the right data
the script will exit. The script is run in the background to ensure that it does not block other
parts of the Venus OS initialisation.

REQUIREMENTS / TEST ENVIRONMENT
This has been tested in the following environment.
It should work in other cases.

   Venus OS 2.6
   RPI 3B+
   USB GPS device with NMEA data stream output.

INSTALLATION

Place the gps-time.sh file in directory /data/venus-rpi-setup/
Make sure it has execute set
>chmod a+x /data/venus-rpi-setup/gps-time.sh

Place a copy of rc.local in directory /data
[ Or add the lines from from rc.local to you own rc.local if you need to
  run other things as well ]
Make sure rc.local has execute access
>chmod a+x /data/rc.local

If you choose to out gps-time.sh somewhere else, 
then change rc.local to refer to the right place.

IN USE
You can check the results of the script running in the log file.
/var/volatile/log/gps-time

If all goes well the time on the Venus OS console will be correct at start up.

Hope it works for you 

    
