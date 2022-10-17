/*
 * Test-A.ino 
 * Created 10/16/2022
 * Testing communication protocols and nature of peripherals
 * 
 * Adapted from : 
 *        @file     Adafruit_MMA8451.h
          @author   K. Townsend (Adafruit Industries)
          @license  BSD (see license.txt)
 */


#include <Wire.h>             //Include Wire.h library for I2C communication 
#include <Adafruit_MMA8451.h> //Include Adafuit libraries for IMU 
#include <Adafruit_Sensor.h>

int delayTime = 1000;   //measured in ms

Adafruit_MMA8451 imu = Adafruit_MMA8451();    //instantiate IMU

void setup(void) {
  Serial.begin(9600);         //begin Serial monitor 

  //Try to initialize IMU 
  if (! imu.begin()) {
    Serial.println("Couldn't start IMU");
    while (1);
  } else {
    Serial.println("IMU Initialized");
  }

  //Set IMU's range - can be changed to 4 or 8 g
  imu.setRange(MMA8451_RANGE_2_G);
  
  Serial.print("Range = "); Serial.print(2 << imu.getRange());  
  Serial.println("G");
  
}

void loop() {
  
}

bool readIMU() {

delay(delayTime); //Change 
  
  // Read the raw data in 14-bit counts
  imu.read();
  Serial.print("X:\t"); Serial.print(imu.x); 
  Serial.print("\tY:\t"); Serial.print(imu.y); 
  Serial.print("\tZ:\t"); Serial.print(imu.z); 
  Serial.println();

  /* Get a new sensor event */ 
  sensors_event_t event; 
  imu.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: \t"); Serial.print(event.acceleration.x); Serial.print("\t");
  Serial.print("Y: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
  Serial.print("Z: \t"); Serial.print(event.acceleration.z); Serial.print("\t");
  Serial.println("m/s^2 ");
  
  /* Get the orientation of the sensor */
  uint8_t o = imu.getOrientation();
  
  switch (o) {
    case MMA8451_PL_PUF: 
      Serial.println("Portrait Up Front");
      break;
    case MMA8451_PL_PUB: 
      Serial.println("Portrait Up Back");
      break;    
    case MMA8451_PL_PDF: 
      Serial.println("Portrait Down Front");
      break;
    case MMA8451_PL_PDB: 
      Serial.println("Portrait Down Back");
      break;
    case MMA8451_PL_LRF: 
      Serial.println("Landscape Right Front");
      break;
    case MMA8451_PL_LRB: 
      Serial.println("Landscape Right Back");
      break;
    case MMA8451_PL_LLF: 
      Serial.println("Landscape Left Front");
      break;
    case MMA8451_PL_LLB: 
      Serial.println("Landscape Left Back");
      break;
    }
  

return true;

}
