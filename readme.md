# Decoder for both La Crosse Weather Sensors TX2/TX3/TX4 and Alecto WS1700

To create the RRD file:

```
rrdtool create temperature_deviceXX.rrd --step 300 \
DS:temperature:ABSOLUTE:600:0:U \
RRA:AVERAGE:0.9:1:24 \
RRA:AVERAGE:0.9:6:35064 \
RRA:MIN:0.9:288:1826 \
RRA:AVERAGE:0.9:288:1826 \
RRA:MAX:0.9:288:1826
```

cf 

* http://www.guillier.org/blog/2014/09/lacrosse-tx2tx3-sensors/
* http://www.guillier.org/blog/2014/09/lacrosse-tx2tx3-sensors-and-digispark/ 
* http://www.guillier.org/blog/2014/10/lacrosse-digispark-raspberry-pi-roundup/
* http://www.guillier.org/blog/2015/10/new-433mhz-sensors/
for more details and schematic

