/**
 * File Name: src.ino 
 * Author: Mark Hofmeister 
 * Created: 10/25/2022
 * Last Modified: 10/30/2022
 * 
 * Source code uploaded to Bait-Cast-Reel ATMEGA328p.
 */
 
#include "Adafruit_seesaw.h"            //Seesaw library, used for function calls to interact with encoder

/*
 * Define encoder global variables/object 
 */
    #define ENCODER_ADDR 0x36               //define I2C address of encoder as 0x36 
     
    #define SS_SWITCH 24                    //Not yet sure what this is required for
    //#define SS_NEOPIX 6                     //Required for the neopixel on the encoder board
    
    Adafruit_seesaw encoder;                //Initialize encoder object and an integer to assign the position to 
    
    int32_t encoderPos = 0;                 //variable to store position, initialized to 0;
    int32_t lastPos = 0;                    //variable to store position from last encoder measurement, initialized to 0;
    int32_t lastFullRot = 0;                //variable to store to encoder position when it crosses the full rotation threshold
    int8_t fullTurn = 24;                   //One full rotation of the encoder is 24 steps. 

/*
 * Define IMU global variables & object
 */

/*
 * Define 7-seg global variables & object
 */

 /*
  * Define cap-touch global variables 
  */


  int inputWindow = 1000;                  //Start the input window length at 1000ms, which is the time allowed for a user input. Decreases with every iteration. 
  int inputWindowDec = 5;                  //integer to decrement the inputWindow by each iteration 

void setup() {

  Serial.begin(9600);                 //Baud rate may need to go to 115200
  while(!Serial) delay(10);           //Wait for serial monitor to initialize

  /*
   * Initialize encoder object & interrupts
  */
      Serial.println("Looking for encoder object");
      if (!encoder.begin(ENCODER_ADDR)) {
        Serial.println("Couldn't find encoder on default address, trying again.");
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
       * It seems to fire on enery rotation 
       */
       encoder.enableEncoderInterrupt();
  
       Serial.println("Encoder Interrupt Initialized.");

  /*
   * Initialize IMU object & interrupts
  */

  /*
   * Initialize 7-seg object 
  */

}

void loop() {

  /*
   * Random number generation:
   * 1 - Bait - calls readBait() function
   * 2 - Cast - calls readCast() function
   * 3 - Reel - calls readReel() function
   */
  int randomNumber = floor(random(1,4));           //generates a random number in the range of 1 to 3

  switch(randomNumber) {                           //Decide what to do with random number 
    
    case 1:             //Bait It
      for(int i = 0; i < inputWindowDec; i++) {
        /*if(readCapTouch()) {
          break;
        }*/
        delay(1);                                   // This is not a perfect timer, but it'll be close. 
      }
      break;
    case 2:             //Cast It

      break;
    case 3:             //Reel It 

      break;
    default:            //Should never reach default, but you know. 

      break;
  }

  inputWindow -= inputWindowDec;                  //Decrement inputWindow by inputWindowDec value (ms)
  
  delay(100);                                     //Delay 100ms before generating a new random action
   
}

/* readReel()
 * Params: None
 * Retrns: Bool indicating whether the encoder was rotated one full rotation
 * 
 * Function to determine if the encoder was rotated through one full rotation since the last full rotation.
 * 
 * Also disregards negative rotation.
 */
bool readReel() {

  bool fullRotation = false;                    //initialize return flag to false
  encoderPos = encoder.getEncoderPosition();    //find new encoder position 

  if(encoderPos < lastPos) {      //prevent a backwards rotation
    Serial.println("Enter backwards rotation conditional");
    encoderPos = lastPos;
  }

  int threshold = lastFullRot + fullTurn;     //Threshold to find a full rotation is the last full rotation threshold plus one full rotation
  
  if(encoderPos >= threshold) {               //If we have passed the threshold of rotation, set flag
    fullRotation = true;
    lastFullRot = encoderPos;               //Also update the lastFullRot
  }
  lastPos = encoderPos;                     
  
  return fullRotation;

}
