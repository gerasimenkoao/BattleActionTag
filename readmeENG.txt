Battle Action Tag (BAT) - radar-style if shoot direction and intensivity

Prototype video: https://youtu.be/wzRuDxuIYS0

The direction determination range is 180 degrees.
Accuracy - 9 degrees.
The sector with the number of shots up to 4 is highlighted in blue, 5-9 - green, more - red.
Brightness is adjustable.
Power supply - from Powebank 5V, current consumption from Arduino Nano - 50-100mA (depends on the brightness and number of light sectors)

Necessary components:

1. Arduino 16MHz on chip ATmega328 (Uno, Pro Mini, Micro, Nano)
2. Two acoustic sensors with adjustable digital output (4 example KY-038)
3. Addressable LED strip 60 LEDs/meter - 1/3 meter (WS-2812b)
4. Potentiometer 100к
5. 3 meter of wires of three colors (1 meter each)
6. Base 20x30см (plywood)
7. A screen from the RadarScreen file printed on a 3D printer using white plastic

Drafting:

1. According to the BAT1_chematic scheme, sound sensors are soldered with 35-cm wires, an LED strip of 20 cm, and a potentiometer of 10 cm.
2. The LED tape is glued to the screen as in the BAT_components photo, the potentiometer is attached to the hole in the middle.
3. All components are attached to the base according to the rear and front photos, the distance between the microphones is 24 cm.
4. Through the Arduino IDE, the BAT1.ino sketch is loaded (you must first download the library https://github.com/FastLED/FastLED)
5. For adjustment, a mark is made in the middle between the microphones.

Settings:

1. The sensitivity of the microphones is adjusted at the threshold of activation of the indicator LEDs on them (they should not light up)
2. Using a Philips screwdriver ;-) on the mark between the microphones, I finally adjust their sensitivity in this way,
   so that a number close to zero (+-10) pops up in the serial port monitor of the Arduino IDE (Tools-Serial Monitor)
3. We get rid of the power indicator LEDs on the Arduino and sound sensors.

Work:

During operation, the system is rebooted either by power or by pressing the reset button on the Arduino.

Bugs:

All PowerBank models have power cut-off systems at low current consumption.
With "economical" Arduinos like the Pro Mini, this can happen.
Bugfix - additional 100 Ohm resistor between + and - power supply (+50mA)

System development plans:

1. Auto-balance of microphones at start
2. Pair-parallel connection of electret microphones to improve the directivity pattern (accuracy) and sensitivity
3. Improvement of timekeeping accuracy.
4. Adding a temperature/humidity sensor to determine the speed of sound (increasing accuracy)
5. Making constructive changes based on the results of operation
6. Involvement of other developers in the project.
