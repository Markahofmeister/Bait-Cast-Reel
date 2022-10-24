#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>

#define SS_SWITCH        24
#define SS_NEOPIX        6

#define SEESAW_ADDR          0x36
#define CAP_IN 3
#define ENC 8

//initialize objects for the IMU, seven segment and encoder
Adafruit_MMA8451 mma = Adafruit_MMA8451();
Adafruit_7segment matrix = Adafruit_7segment();
Adafruit_seesaw ss;
int32_t encoder_position;

void setup() {
  #ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment Backpack Test");
  #endif
  matrix.begin(0x70);
  
  //initialize accelerometer
  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }
  mma.setRange(MMA8451_RANGE_2_G);


  matrix.print(2222, DEC);
  matrix.writeDisplay();
  delay(10);
  
  
  //initialize encoder
  if (! ss.begin(SEESAW_ADDR)) {
    Serial.println("Couldn't find seesaw on default address");
    while(1) delay(10);
  }
  encoder_position = ss.getEncoderPosition();

  pinMode(CAP_IN,INPUT);

}

void loop() {
  sensors_event_t event; 
  mma.getEvent(&event);
   
  int zaccel = event.acceleration.z; 
  int button = digitalRead(CAP_IN);
  int32_t new_position = ss.getEncoderPosition();

  
  // did we move arounde?
  if (encoder_position != new_position) {
    matrix.println(new_position);         // display new position
    matrix.writeDisplay();
    encoder_position = new_position;      // and save for next round
    delay(10);
  }
  
  if(zaccel >10 || zaccel < 9){
     matrix.println(zaccel);
    matrix.writeDisplay();    
    delay(10);
  }
    while(button == 0){
    matrix.print(0000, DEC);
    matrix.writeDisplay();
    delay(10);
  }



}
