/**
 * File Name: encoder-func.ino 
 * Author: Mark Hofmeister 
 * Created: 10/25/2022
 * Last Modified: 10/25/2022
 * 
 * Sandbox to make encoder function. 
 */
 
#include "Adafruit_seesaw.h"            //Seesaw library, used for function calls to interact with encoder

#define ENCODER_ADDR 0x36               //define I2C address of encoder as 0x36 
 
#define SS_SWITCH 24                    //Not yet sure what this is required for
//#define SS_NEOPIX 6                     //Required for the neopixel on the encoder board

Adafruit_seesaw encoder;                //Initialize encoder object and an integer to assign the position to 

int32_t encoderPos = 0;                 //variable to store position, initialized to 0;
int32_t lastPos = 0;                    //variable to store position from last encoder measurement, initialized to 0;
int32_t lastFullRot = 0;                //variable to store to encoder position when it crosses the full rotation threshold
int8_t fullTurn = 24;                   //One full rotation of the encoder is 24 steps. 

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

  Serial.println("Encoder Interrupt Initialized.");

}

void loop() {

  encoderPos = encoder.getEncoderPosition();
  Serial.print("Encoder Pos: ");
  Serial.println(encoderPos);
  Serial.print("Last Pos : ");
  Serial.println(lastPos);
  bool rot = readEncoderRotation();
  Serial.print("Rotated? : ");
  Serial.println(rot);
  delay(1000);

}

bool readEncoderRotation() {

  bool fullRotation = false;  
  encoderPos = encoder.getEncoderPosition();

  if(encoderPos < lastPos) {      //prevent a backwards rotation 
    Serial.println("Enter backwards rotation loop");
    encoderPos = lastPos;
  }

  int threshold = lastFullRot + fullTurn;
  
  if(encoderPos >= threshold) {
    fullRotation = true;
    lastFullRot = encoderPos;
  }
  lastPos = encoderPos;
  
  return fullRotation;

}
