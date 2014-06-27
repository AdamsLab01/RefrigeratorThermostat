/**********************************************************************************************************************
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

All code (except external libraries and third party code) is published under the MIT License.

**********************************************************************************************************************/

// Include libraries
#include "Wire.h"
#include "LiquidCrystal.h"
#include "OneButton.h"
#include "OneWire.h"
#include "DallasTemperature.h"

// States
#define S_Monitor 0
#define S_Sleep 1
#define S_SetTemp 2

int state = S_Monitor; // Inital state

// DS18B20 setup
#define TempSensor 10 // PIN one wire device (temps sensor) is pluggend into
OneWire oneWire(TempSensor);
// Pass the OneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

// OneButton setup
OneButton button(9, true); // Tell the OneButton library what PIN the button is on

// Initialize the LCD library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// Other PINs
int FridgeRly = 8;

// Vars
unsigned long Sleep = 0;
unsigned long SleepLength = 300000; // How long we wait before we try to cool the fridge again. To keep from overheating we need to wait at least 3 min

float SetTemp = 35.00; // The defualt temp that we want the fride to be at
float TempC = 0;
float TempF = 0;

bool CoolRun = false;
bool Boot = true;

void setup() {
  lcd.begin(16, 2); // Set up the LCD's number of rows and columns
  
  sensors.begin(); // Start the DallasTemperature library
  
  // PIN Modes
  pinMode(FridgeRly, OUTPUT);
    
  //Serial.begin(9600); // For testing comment out in production
}

void loop() {
  switch(state) {
    case S_Monitor:
      F_Monitor();
    break;
    
    case S_Sleep:
      F_Sleep();
    break;
    
    case S_SetTemp:
      F_SetTemp();
    break;
  }
}

// Functions

void F_Monitor() {
  button.attachPress(F_ButtonPress);
  button.tick();
  
  if (Boot == true) { 
    Sleep = millis();
    state = S_Sleep;
  }
  
  if (TempF <= SetTemp) { 
    lcd.setCursor(0, 1);
    lcd.print("Monitoring...   ");
  }  
  
  F_GetTemp();
  
  F_ShowTemp();
  
  if (TempF > SetTemp) { 
    lcd.setCursor(0, 1);
    lcd.print("Coolng to: ");
    lcd.print(SetTemp);
    digitalWrite(FridgeRly, HIGH);
    CoolRun = true;
  }
  
  else {
    digitalWrite(FridgeRly, LOW);
  }
  
  if (CoolRun == true && TempF <= SetTemp) { 
    Sleep = millis();
    state = S_Sleep;
  }
}

void F_Sleep() {
  button.attachPress(F_ButtonPress);
  button.tick();
  
  digitalWrite(FridgeRly, LOW);
  
  lcd.setCursor(0, 1);
  lcd.print("Sleeping...     ");

  F_GetTemp();
 
  F_ShowTemp();
  
  if (millis() - Sleep > SleepLength) {
    CoolRun = false; 
    Boot = false;
    state = S_Monitor;
  }
}

void F_SetTemp() {
  button.attachClick(F_Increse);
  button.attachPress(F_ButtonPress2);
  button.tick();
  
  lcd.setCursor(0, 0);
  lcd.print("SetTemp: ");
  lcd.print(SetTemp);
  lcd.setCursor(0, 1);
  lcd.print("  Hold to Exit  ");
}

void F_Increse() {
  SetTemp = SetTemp + 1.00;
  
  if (SetTemp > 40.00) {
    SetTemp = 32.00;
  }
}

void F_ButtonPress() {
  state = S_SetTemp;
}

void F_ButtonPress2() {
  state = S_Monitor;
}

void F_GetTemp() {
  sensors.requestTemperatures(); // Tell the sensor to record the temp to its memory
  
  TempC = sensors.getTempCByIndex(0); // Retriev the temp from the sensor's memory and stor it in a local var
  
  TempF = (TempC * 9.0)/ 5.0 + 32.0; // Convert to ferenheiht
}

void F_ShowTemp() {
  lcd.setCursor(0, 0);
  lcd.print("Temp: "); 
  lcd.print(TempF);
  lcd.print("F    ");
  //Serial.print(TempF); Serial.println(" degrees F"); // For testing comment out in production
}
