/* YourDuino Multiple DS18B20 Temperature Sensors on 1 wire
  Connections:
  DS18B20 Pinout (Left to Right, pins down, flat side toward you)
  - Left   = Ground
  - Center = Signal (Pin 2):  (with 3.3K to 4.7K resistor to +5 or 3.3 )
  - Right  = +5 or +3.3 V

   Questions: terry@yourduino.com 
   V1.01  01/17/2013 ...based on examples from Rik Kretzinger
   
/*-----( Import needed libraries )-----*/
// Get 1-wire Library here: http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <OneWire.h>

//Get DallasTemperature Library here:  http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
#include <DallasTemperature.h>

// include the library code:
#include <LiquidCrystal.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define ONE_WIRE_BUS_PIN 8

/*-----( Declare objects )-----*/
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS_PIN);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/*-----( Declare Variables )-----*/
// Assign the addresses of your 1-Wire temp sensors.
// See the tutorial on how to obtain these addresses:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html

DeviceAddress Probe01 = { 0x28, 0xFF, 0x70, 0x83, 0x73, 0x15, 0x02, 0xD0 }; 
DeviceAddress Probe02 = { 0x28, 0xFF, 0x64, 0x8C, 0x73, 0x15, 0x01, 0xCB };


void setup()   /****** SETUP: RUNS ONCE ******/
{
  // start serial port to show results
  Serial.begin(9600);
  Serial.print("Initializing Temperature Control Library Version ");
  Serial.println(DALLASTEMPLIBVERSION);
  
  // Initialize the Temperature measurement library
  sensors.begin();
  
  // set the resolution to 10 bit (Can be 9 to 12 bits .. lower is faster)
  sensors.setResolution(Probe01, 10);
  sensors.setResolution(Probe02, 10);
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Temperature");
  lcd.setCursor(0, 1);
  lcd.print("Control");
}//--(end setup )---

void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  delay(1000);
  Serial.println();
  Serial.print("Number of Devices found on bus = ");  
  Serial.println(sensors.getDeviceCount());   
  Serial.print("Getting temperatures... ");  
  Serial.println();   
  
  float temp1;
  float temp2;
  // Command all devices on bus to read temperature  
  sensors.requestTemperatures();  
  temp1 = getTemperature(Probe01);
  temp2 = getTemperature(Probe02);
  
  lcd.clear();
  //lcd.setCursor(0, 0);
  Serial.print("Probe 01 temperature is:   ");
  //lcd.print("Probe 01=  ");
  lcd.print(temp1);
  Serial.println();
  lcd.setCursor(1, 0);

  lcd.setCursor(7, 0);
  lcd.print("| ");
  //lcd.setCursor(0, 1);
  Serial.print("Probe 02 temperature is:   ");
  //lcd.print("Probe 02= ");
  lcd.print(temp2);
  Serial.println();
  lcd.setCursor(0, 1);
  
  if (temp1 == -127 || temp2 == -127) {
    lcd.print("ERROR");
  }
  else {
    lcd.print("FAN: ");
    if (temp1-temp2 < 0.75) 
    {
      lcd.print("OFF");
    }
    else if (temp1-temp2 < 2) 
    {
      lcd.print("Speed 1");
    }
    else 
    {
      lcd.print("Speed 2");
    }
  }  
  
}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/
float getTemperature(DeviceAddress deviceAddress)
{

   float tempC = sensors.getTempC(deviceAddress);

   if (tempC == -127.00) 
   {
     Serial.print("Error getting temperature  ");
     //lcd.print("ERROR");
   } 
   else
   {
     Serial.print("C: ");
     Serial.print(tempC);
     Serial.print(" F: ");
     Serial.print(DallasTemperature::toFahrenheit(tempC));
   }
   
   return tempC;
}// End printTemperature
//*********( THE END )***********
