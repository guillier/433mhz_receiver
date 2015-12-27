# Decoder for sensors and remote

Compatible with: 

* La Crosse Weather Sensors TX2/TX3/TX4
* Alecto WS1700 sensor
* YC-2000B remote ("rc-switch")

# RRD

To create the RRD files (used by sensor2rrd.py):

```
rrdtool create temperature_deviceXX.rrd --step 300 \
DS:temperature:ABSOLUTE:600:0:U \
RRA:AVERAGE:0.9:1:24 \
RRA:AVERAGE:0.9:6:35064 \
RRA:MIN:0.9:288:1826 \
RRA:AVERAGE:0.9:288:1826 \
RRA:MAX:0.9:288:1826
```

```
rrdtool create humidity_deviceXX.rrd --step 300 \
DS:humidity:ABSOLUTE:600:0:100 \
RRA:AVERAGE:0.9:1:24 \
RRA:AVERAGE:0.9:6:35064 \
RRA:MIN:0.9:288:1826 \
RRA:AVERAGE:0.9:288:1826 \
RRA:MAX:0.9:288:1826
```

# More information

cf 

* http://www.guillier.org/blog/2014/09/lacrosse-tx2tx3-sensors/
* http://www.guillier.org/blog/2014/09/lacrosse-tx2tx3-sensors-and-digispark/ 
* http://www.guillier.org/blog/2014/10/lacrosse-digispark-raspberry-pi-roundup/
* http://www.guillier.org/blog/2015/10/new-433mhz-sensors/
* http://www.guillier.org/blog/2015/12/433mhz-remote-sockets-part-2/

for more details and schematic

