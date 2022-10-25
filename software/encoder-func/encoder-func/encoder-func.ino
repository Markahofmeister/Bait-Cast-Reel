/**
 * File Name: encoder-func.ino 
 * Author: Mark Hofmeister 
 * Created: 10/25/2022
 * Last Modified: 10/25/2022
 * 
 * Sandbox to make encoder function
 */
 
#include "Adafruit_seesaw.h"            //Seesaw library, used for function calls to interact with encoder

#define ENCODER_ADDR 0x36               //define I2C address of encoder as 0x36 
 
#define SS_SWITCH 24                    //Not yet sure what this is required for
//#define SS_NEOPIX 6                     //Required for the neopixel on the encoder board

Adafruit_seesaw encoder;                //Initialize encoder object and an integer to assign the position to 
int32_t encoderPos;

void setup() {

  Serial.begin(115200); //This is a pretty high baud rate 
  while(!Serial) delay(10);

  Serial.println("Looking for encoder object");
  if (! encoder.begin(ENCODER_ADDR)) {
    Serial.println("Couldn't find seesaw on default address, trying again.");
    while(1) delay(10);
  }
  Serial.println("Encoder initialized.");

  encoder.pinMode(SS_SWITCH, INPUT_PULLUP);     //Is this pullup pin native to the encoder module itself? Will a bare ATMEGA vs. Uno have problems with this?

  encoderPos =  encoder.getEncoderPosition();

  Serial.println("Turning on Interrupt pin.");
  delay(10);
  encoder.setGPIOInterrupts((uint32_t)1<<SS_SWITCH, 1);     
  /*
   * What does this do? This seems like we're masking the 24th bit of a register. 
   * There isn't much documentation on this. I think the interrupt is active low. I'm not sure to change the threshold on which it fires. 
   */
  encoder.enableEncoderInterrupt();
  

}

void loop() {
  

}

bool readEncoder() {

  

}
