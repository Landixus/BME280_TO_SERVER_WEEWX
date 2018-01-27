#include <stdint.h>
#include "SparkFunBME280.h"
#include "Wire.h"
#include "SPI.h"
#include <ESP8266WiFi.h>
#include <WiFiRestClient.h>

#define USE_SERIAL Serial

//Global sensor object
BME280 mySensor;
const char* host = "192.168.0.39"; //Your weewx server IP
const char* route = "/test.php?"; //Your php insert script

void setup()
{
  mySensor.settings.commInterface = I2C_MODE;
  mySensor.settings.I2CAddress = 0x76;  // sometimes 0x77
  mySensor.settings.runMode = 3; //Normal mode
  mySensor.settings.tStandby = 5;
  mySensor.settings.filter = 0;
  mySensor.settings.tempOverSample = 1;
  mySensor.settings.pressOverSample = 1;
  mySensor.settings.humidOverSample = 1;

#define BME280_ADDRESS 0x76  //sometime 0x77
unsigned long int hum_raw,temp_raw,pres_raw;
signed long int t_fine;

  Serial.begin(115200);
  Serial.print("Program Started\n");
  Serial.print("Starting BME280... result of .begin(): 0x");
  
  //Calling .begin() causes the settings to be loaded
  delay(100);  BME280 requires 2ms to start up.
  Serial.println(mySensor.begin(), HEX);
  Serial.println();
    
delay(200);
const char* ssid     = "YOUR_SSID";
const char* password = "YOUR WIFI PASSWORD";
  delay(10);

  // connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

delay(1000);
}
void loop() {
  
  Serial.print("Temperature: ");
  Serial.print(mySensor.readTempC(),2);
  Serial.println(" degrees C");

  Serial.print("Pressure: ");
  Serial.print(mySensor.readFloatPressure(), 2);
  Serial.println(" Pa");

  Serial.print("%RH: ");
  Serial.print(mySensor.readFloatHumidity(), 2);
  Serial.println(" %");
  
  Serial.println();
  WiFiRestClient restClient( host );

  Serial.print( "Posting to http://" );
  Serial.print( host );
  Serial.println( route );

char str_temp[6];
char str_humid[6];
char str_press[8];

dtostrf(mySensor.readTempC(), 3, 1, str_temp);
dtostrf(mySensor.readFloatHumidity(), 2, 0, str_humid);
dtostrf((mySensor.readFloatPressure()/100), 4, 2, str_press);

  char buf[256];
  sprintf(buf, "%s&%s&%s", str_temp, str_humid, str_press);
  //sprintf(buf, str_temp, str_humid, str_press);
  Serial.print( buf );
  Serial.println( " " );
  
  int statusCode = restClient.post( route, buf);  

  Serial.print ("Status received: " );
  Serial.println( statusCode );
  
  Serial.println( "Waiting 30 seconds" );
  delay(30000);
  }
