/*
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
*/

// Decoder for La Crosse Weather Sensors TX2/TX3/TX4 with or without hygrometer
// Partially based on work by Jean-Paul ROUBELAT - F6FBB http://www.f6fbb.org/domo/sensors/tx3_th.php
// Data is sent to Serial via the Debugging Library (on pin 2)
// Version for Digispark http://digistump.com/category/1

// Should be ~ 1300uS
#define LP_MIN 1100
#define LP_MAX 1500
// Should be ~ 500uS
#define SP_MIN 300
#define SP_MAX 700

#define PIN 0 // From 433Mhz receiver

unsigned long dur; // pulse duration
byte nibble[11];
byte type; // data = temperature or humidity

void setup()
{
  pinMode(PIN, INPUT);
  Serial.begin(38400);
}

int read_tx ()
{
  for (int i=0; i<11; i++)
    nibble[i] = 0;
  nibble[1] = 10;

  // In theory preamble is 00001010 but here we don't care if first few
  // bits are garbled as long as it starts with 1010 with the correct
  // pulse timing.
  byte v = 0;
  do
  {
    v <<= 1;
    dur = pulseIn(PIN, HIGH);
    if ((dur > SP_MIN) && (dur < SP_MAX))
      v |= 1;  // Short Pulse => 1
    else if ((dur <= LP_MIN) || (dur >= LP_MAX))
      v = 0;  // Not Long Pulse => Reset
  } while (v != 10);  // 00001010b = 10d

  // Then we read all the remaining bits
  for (int i=8; i<44; i++)
  {
    nibble[i >> 2] <<= 1;
    dur = pulseIn(PIN, HIGH);
    if ((dur > SP_MIN) && (dur < SP_MAX))
      nibble[i >> 2] |= 1;  // Short Pulse => 1  
    else if ((dur <= LP_MIN) || (dur >= LP_MAX))
      return -1;  // Not Long Pulse => Reset
  }

  // Should be either 0 (Temperature) or 14 (Hygrometry)
  if ((nibble[2] == 0) || (nibble[2] == 14))
    type = nibble[2];
  else
    return -1;

  // Next 7 bits are the device number generated randomly when batteries are
  // changed on the sensor.
  // Lowest bit of the nibble #4 is supposed to be the parity but this
  // doesn't seem to match on the older sensors (TX2)
  int device = (nibble[3] << 3) | (nibble[4] >> 1);

  // Temperature is composed of nibbles #5 & #6 + #7 for the decimal part
  // There is also a offset of 50 deg Celcius.
  int val = nibble[5] * 10 + nibble[6];
  int vald = 0;
  if (type == 0)
  {
    val -= 50;
    vald = nibble[7];
  }

  // Last nibble is CRC and should match 4 lowest bits of the arithmetic
  // addition of all the previous nibbles
  int crc1 = 0;
  for (int i=0; i<10; i++)
    crc1 += nibble[i];
  
  crc1 &= 15;

  if (crc1 != nibble[10])
    return -2;  // CRC KO

  if ((nibble[5] != nibble[8]) || (nibble[6] != nibble[9]))
    return -3;  // Digit check KO

  // Format : e.g. for device #39
  // TEMP,39,20.5
  // HYGR,39,48
  if (type == 0)
  {
    Serial.print("TEMP,");
    Serial.print(device);
    Serial.print(",");
    Serial.print(val);
    Serial.print(".");
    Serial.println(vald);
  }
  else
  {
    Serial.print("HYGR,");  
    Serial.print(device);
    Serial.print(",");
    Serial.println(val);
  }

  delay(50); // Signal is repeated by sensor. if CRC is OK then additional transmissions can be ignored.
  return 0;
}
 

void loop() { read_tx(); }
