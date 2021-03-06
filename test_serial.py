#!/usr/bin/env python3
"""
The MIT License (MIT)

Copyright (c) 2014 François GUILLIER <dev @ guillier . org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
"""

import re
import serial
import time

serialPort = serial.Serial("/dev/ttyACM0", 38400, timeout=0)
#serialPort = serial.Serial("/dev/ttyAMA0", 38400, timeout=0)

while True:
    while serialPort.inWaiting():
        time.sleep(0.01)  # Wait for complete transmission
        line = serialPort.readline().decode().strip()
        print(line)
        m = re.match(r'TEMP,(\d+),(\d+\.\d)', line)
        if m:
            print('temperature - device {} = {}'.format(m.group(1),
                                                        m.group(2)))
        m = re.match(r'HYGR,(\d+),(\d+)', line)
        if m:
            print('humidity - device {} = {}'.format(m.group(1),
                                                     m.group(2)))
        m = re.match(r'RC,(\d+),(\d+),(\d)', line)
        if m:
            binbtn = m.group(2)
            btn = 'A' if binbtn[0] == '1' else ''
            btn += 'B' if binbtn[1] == '1' else ''
            btn += 'C' if binbtn[2] == '1' else ''
            btn += 'D' if binbtn[3] == '1' else ''
            btn += 'E' if binbtn[4] == '1' else ''
            action = 'on' if m.group(3) == '1' else 'off'
            print('remote - addr {} - button {} => {}'.format(m.group(1),
                                                              btn,
                                                              action))
