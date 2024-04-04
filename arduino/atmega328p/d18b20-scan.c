// http://ur4uqu.com/

/*
A4->sda A5->scl
*/
 
#include <OneWire.h>
#include <DallasTemperature.h>
 
#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 20 column and 4 rows
 
// Data wire is plugged into port 4 on the Arduino
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
 
int numberOfDevices; // Number of temperature devices found
 
DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address
 
void setup(void) {
  // start serial port
  Serial.begin(9600);
 
  lcd.init(); // initialize the lcd
  lcd.backlight();
 
  // Start up the library
  sensors.begin();
 
  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();
 
  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");
 
  // Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++) {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tempDeviceAddress);
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }
}
 
String tolcd = ""; 
 
void loop(void) { 
  sensors.requestTemperatures(); // Send the command to get temperatures
  tolcd = "";
  lcd.clear();
 
  // Loop through each device, print out temperature data
  for(int i=0;i<numberOfDevices; i++) {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i)){
 
    // Output the device ID
    Serial.print("Temperature for device: ");
    Serial.println(i,DEC);
 
    // Print the data
    float tempC = sensors.getTempC(tempDeviceAddress);
    printAddress(tempDeviceAddress); Serial.print(" ");
    Serial.print("Temp C: ");
    Serial.println(tempC);
 
    if (i<4) { 
      tolcd += String(i)+"="+String(tempC)+"   "; 
    }
 
//    Serial.print(" Temp F: ");
//    Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
    }   
  }
 lcd.setCursor(0, 0); lcd.print(tolcd);
  delay(5000);
}
 
// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}
