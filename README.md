Refrigerator Controller

This sketch was written to work as a refrigerator controller. It was used to replace a broken mechanical thermostat in
a mini-fridge. I used a waterproof DS18B20 digital temp sensor to monitor the temp in the fridge. An 8 amp SSR was
used to turn the fridge compressor on/off.

Important to note is the Sleep state. The mechanics of refrigerators prevent the compressor 
from turning back on to quickly. The manual for the fridge stated to wait a minimum of 3 min. after unplugging the fridge to
plug it back in. If you tried to turn the fridge on too soon the thermal protection circuit in the fridge would trip
after ~1 min. To error on the side of caution I wrote the sketch to go to "sleep" for 5 min. after the system detected
desired temp. was reached.

Please note that this sketch requires severl libraries to function which I've included in the repository. You may want to
check for updated version though.

Wire.h - is included with the Arduino IDE.
LiquidCrystal.h - included with the Arduino IDE.
OneButton - can be downloaded here: http://www.mathertel.de/Arduino/OneButtonLibrary.aspx
OneWire.h - can be downloaded from here: http://www.pjrc.com/teensy/td_libs_OneWire.html
DallasTemperature.h - can be downloaded from here: http://www.milesburton.com/?title=Dallas_Temperature_Control_Library 

For more information see - https://adambyers.com/2014/06/refrigerator-controller/ or ping adam@adambyers.com