// from https://github.com/adafruit/DHT-sensor-library/tree/26e2bc1d793fba7f6db41a9f7ce0499b0ab3c622
// and https://github.com/adafruit/Light-and-Temp-logger/blob/ab6b49f22e7f6210ded222e647d6429ee936611f/lighttemplogger.pde

#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT22   // DHT 22  (AM2302)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); 

  dht.begin();
}

void loop() {
  // Reading temperature takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float temp = dht.readTemperature(true);

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(temp)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print(temp);
    Serial.println(" *F");
  }
}
