/* This demo shows how to display the CCS811 readings on an Adafruit I2C OLED. 
 * (We used a Feather + OLED FeatherWing)
 */
 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>
#include "Adafruit_CCS811.h"

Adafruit_CCS811 ccs811;
Adafruit_SSD1306 display = Adafruit_SSD1306();

void setup() {  
  Serial.begin(115200);

  if(!ccs811.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(500);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  //calibrate temperature sensor
  while(!ccs811.available());
  float temp = ccs811.calculateTemperature();
  ccs811.setTempOffset(temp - 25.0);
  
  Serial.println("IO test");

  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
}


void loop() {
  display.setCursor(0,0);
  if(ccs811.available()){
    display.clearDisplay();
    float temp = ccs811.calculateTemperature();
    if(!ccs811.readData()){
      display.print("eCO2: ");
      Serial.print("eCO2: ");
      float eCO2 = ccs811.geteCO2();
      display.print(eCO2);
      Serial.print(eCO2);
      
      display.print(" ppm\nTVOC: ");
      Serial.print(" ppm, TVOC: ");      
      float TVOC = ccs811.getTVOC();
      display.print(TVOC);
      Serial.print(TVOC);
      
      Serial.print(" ppb   Temp:");
      display.print(" ppb\nTemp: ");
      Serial.println(temp);
      display.println(temp);
      display.display();
    }
    else{
      Serial.println("ERROR!");
      while(1);
    }
  }
  delay(500);
}

