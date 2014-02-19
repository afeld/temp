// from https://github.com/adafruit/DHT-sensor-library/tree/26e2bc1d793fba7f6db41a9f7ce0499b0ab3c622
// and https://github.com/adafruit/Light-and-Temp-logger/blob/ab6b49f22e7f6210ded222e647d6429ee936611f/lighttemplogger.pde
//
// wire sensor according to http://learn.adafruit.com/dht/connecting-to-a-dhtxx-sensor :
// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

#include "DHT.h"
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

// CS pin for SD card
const int chipSelect = 10;

// the temperature sensor
DHT dht(DHTPIN, DHTTYPE);

RTC_DS1307 rtc;


void setup() {
  Serial.begin(9600); 

  dht.begin();


  // make sure that the default chip select pin is set to
  // output, even if it's not used
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (SD.begin(chipSelect)) {
    Serial.println("card initialized.");  
  } else {
    Serial.println("Card failed, or not present");
  }
  
  
  #ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  rtc.begin();

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop() {
  // Reading temperature takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float temp = dht.readTemperature(true);

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(temp)) {
    Serial.println("Failed to read from DHT");
  } else {
    String dataString = "";

    DateTime now = rtc.now();
    dataString += now.month();
    dataString += '/';
    dataString += now.day();
    dataString += '/';
    dataString += now.year();
    dataString += ' ';
    dataString += now.hour();
    dataString += ':';
    dataString += now.minute();
    dataString += ':';
    dataString += now.second();

    dataString += ',';
    char tempStr[5];
    dtostrf(temp, 5, 2, tempStr);
    dataString += tempStr;

    Serial.println(dataString);
    
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
  
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
    }  
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    } 
  }
}
