/**
 * File Name: src.ino 
 * Author: Mark Hofmeister, Maya Román, John McDonough
 * Created: 10/25/2022
 * Last Modified: 11/7/2022
 * 
 * Source code uploaded to Bait-Cast-Reel ATMEGA328p.
 */
 
#include "Adafruit_seesaw.h"            //Seesaw library, used for function calls to interact with encoder
#include <Adafruit_LEDBackpack.h>       //Used to communicate I2C to Adafruit's "backpack" for 7-seg display. 
#include <Adafruit_Sensor.h>
#include <Adafruit_MMA8451.h>
#include <math.h>

 /*
  * Define cap-touch global variables 
  */
    #define capTouchInput 3               //pin 5
    #define capTouchLED 5                 //pin 11
    
/*
 * Define IMU global variables & object
 */
    #define IMULED 6
    Adafruit_MMA8451 IMU = Adafruit_MMA8451();

/*
 * Define encoder global variables/object 
 */
    #define ENCODER_ADDR 0x36               //define I2C address of encoder as 0x36 
     
    #define SS_SWITCH 24                    //Not yet sure what this is required for
    //#define SS_NEOPIX 6                     //Required for the neopixel on the encoder board

    #define encoderLED 7
    
    Adafruit_seesaw encoder;                //Initialize encoder object and an integer to assign the position to 
    
    int32_t encoderPos = 0;                 //variable to store position, initialized to 0;
    int32_t lastPos = 0;                    //variable to store position from last encoder measurement, initialized to 0;
    int32_t lastFullRot = 0;                //variable to store to encoder position when it crosses the full rotation threshold
    int8_t fullTurn = 24;                   //One full rotation of the encoder is 24 steps. 

/*
 * Define 7-seg global variables & object
 */
    #define SEVSEG_ADDR 0x70                //define global I2C address of 7seg backpack 
    Adafruit_7segment sevSeg = Adafruit_7segment();   //initialize sevSeg object to interact with I2C backpack 
    int scoreCount = 0;                               //score to display and increment as user successfully inputs
  
  int inputWindow = 5000;                  //Start the input window length at 1000ms, which is the time allowed for a user input. Decreases with every iteration. 
  int inputWindowDec = 5;                  //integer to decrement the inputWindow by each iteration 

  /**
   * Define Audio global variables/pins
   */
   #define audioOut 9
   #define audioOut_LOW 10
   #define baitFreq 100
   #define castFreq 200
   #define reelFreq 300
   uint8_t audioDuration = 2000;   

void setup() {

  Serial.begin(9600);                 //Baud rate may need to go to 115200
  while(!Serial) delay(10);           //Wait for serial monitor to initialize

  

  /*
   * Initialize cap. touch Pins
  */
      pinMode(capTouchInput, INPUT);
      pinMode(capTouchLED, OUTPUT);

  /*
   * Initialize IMU object & interrupts
  */
      pinMode(IMULED, OUTPUT);
      if(!IMU.begin()) {
        while(1);
      }
      IMU.setRange(MMA8451_RANGE_4_G);
 
  
  /*
   * Initialize encoder object & interrupts
  */
      Serial.println("Looking for encoder object");
      if (!encoder.begin(ENCODER_ADDR)) {
        Serial.println("Couldn't find encoder on default address, trying again.");
        while(1) delay(10);
      }
    
      encoder.pinMode(SS_SWITCH, INPUT_PULLUP);     //Is this pullup pin native to the encoder module itself? Will a bare ATMEGA vs. Uno have problems with this?
      encoderPos =  encoder.getEncoderPosition();
 
      pinMode(encoderLED, OUTPUT);

  /*
   * Initialize 7-seg object 
  */
      sevSeg.begin(SEVSEG_ADDR);
      sevSeg.print(scoreCount);
      sevSeg.writeDisplay();

  /**
   * Initialize audio pins
   */
      pinMode(audioOut, OUTPUT);
      pinMode(audioOut_LOW, OUTPUT);
      digitalWrite(audioOut_LOW, LOW);
  /**
   * Check LEDs and audio
   */
      digitalWrite(capTouchLED,HIGH); 
      digitalWrite(encoderLED,HIGH); 
      digitalWrite(IMULED,HIGH); 
//      tone(audioOut, baitFreq, audioDuration);
//      delay(audioDuration);
//      tone(audioOut, castFreq, audioDuration);
//      delay(audioDuration);
//      tone(audioOut, reelFreq, audioDuration);
      delay(2000);
      digitalWrite(capTouchLED,LOW); 
      digitalWrite(encoderLED,LOW); 
      digitalWrite(IMULED,LOW); 
}

void loop() {

  /*
   * Random number generation:
   * 1 - Bait - calls readBait() function
   * 2 - Cast - calls readCast() function
   * 3 - Reel - calls readReel() function
   */
  int randomNumber = floor(random(1,4));           //generates a random number in the range of 1 to 3  
   if(randomNumber == 1){
      //call cap_touch function
      digitalWrite(capTouchLED,HIGH);              // associated LED turns on 
      //tone(audioOut, baitFreq, audioDuration);    //Play associated tone 
      //delay(audioDuration);
      //send out audio command via talkie
      bool success_val = false;
      unsigned int current_time = millis(); //gets the number of milliseconds that program has been running

      while((millis()-current_time) <= inputWindow){//differencing time to see amount of time passed 
          success_val = readCapTouch();

          if(success_val){
            break;
          }
      }

      digitalWrite(capTouchLED, LOW); //turn LED off
      seven_seg(success_val);                              //Increment counter and display    
   } // end captouch if statement
   else if(randomNumber == 2){             //Cast It
      
      digitalWrite(IMULED, HIGH);

      //tone(audioOut, castFreq, audioDuration);            //Play associated tone
      //delay(audioDuration);
      unsigned int current_time2 = millis();
      bool success_val2 = false;

      while((millis()-current_time2) <= inputWindow){

        success_val2 = readIMU();
        
        if(success_val2){
          break;
        }
      }

      digitalWrite(IMULED, LOW);//turn off indicator led  
      seven_seg(success_val2);//Increment counter  
    } // end cast else if
    else if(randomNumber == 3){             //Reel It 
      
      digitalWrite(encoderLED, HIGH);
      tone(audioOut, reelFreq, audioDuration);          //Play associated tone
      delay(audioDuration);
      unsigned long current_time3 = millis();
      bool success_val3 = false;

      while((millis()-current_time3) <= inputWindow){
        success_val3 = readReel();

        if(success_val3){
          break;
        }
      }

      digitalWrite(encoderLED, LOW); //turn off indicator led  
      seven_seg(success_val3);      //Increment counter  
    } // end reel else if
      
    //default:            //Should never reach default, but you know. 
    else{
      while(1){
      }
    }

  inputWindow -= inputWindowDec;                  //Decrement inputWindow by inputWindowDec value (ms)
  
  delay(500);                                     //Delay 100ms before generating a new random action
}


/* readCapTouch()
 * Params: None
 * Retrns: Bool indicating whether the cap. touch was triggered
 * 
 */
bool readCapTouch(){
  int compare = 0;
  bool capOut; 
  
  while(compare < 1000){                
    if(digitalRead(capTouchInput) == LOW){
      capOut = true; 
    }else if(digitalRead(capTouchInput) == HIGH){
      capOut = false; 
      break;
    }
    delay(1);
    compare ++;
  }
  return capOut;
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

  if(encoderPos > lastPos) {      //prevent a backwards rotation
    //Serial.println("Enter backwards rotation conditional");
    encoderPos = lastPos;
  }

  int threshold = lastFullRot - fullTurn;     //Threshold to find a full rotation is the last full rotation threshold minus one full rotation
  
  if(encoderPos <= threshold) {               //If we have passed the threshold of rotation, set flag
    fullRotation = true;
    lastFullRot = encoderPos;               //Also update the lastFullRot
  }
  lastPos = encoderPos;                     
  
  return fullRotation;

}
/* readIMU()
 * Params: None
 * Retrns: Bool indicating whether the IMU has detected an input
 * 
 * Function to determine if the IMU has detected an input
 * 
 */
bool readIMU(){//function for IMU
  
  // Get a new sensor event
  sensors_event_t event; 
  IMU.getEvent(&event);
  uint8_t zaccel = event.acceleration.z;

  if(zaccel > 20){
    return true;
    }
  //no input detected
  return false;
}


/* seven_seg(success_val)
 * Params: bool to indicate a successul user input
 * Retrns: none
 * 
 * Function to write the output of the seven segment display depending on a successul user input. 
 * Increments if successul, displays score if not. 
 */
void seven_seg(bool success_val){ 
  if(success_val){                  
    scoreCount++;                             //increment count variable   
    sevSeg.print(scoreCount);
    sevSeg.writeDisplay();
    if (scoreCount >= 99) {                   //User has won. 
      while (1) {
        sevSeg.blinkRate(1);                  // Indicate winning with alternating between blinking and not blinking 
        delay(2000);
        sevSeg.blinkRate(0);
        delay(2000);
      }
      deadLoop();                             //Should never reach here, but you know. 
    }
    delay(10);                                //Might have to adjust delays

  }
  else{                                   
    sevSeg.print(scoreCount);
    sevSeg.blinkRate(1);                      //Blinks the 7seg to indicate game over. 0 = no blink, 1,2,3 = varied blink rates. 
    sevSeg.writeDisplay();
    deadLoop();
  }
}

/**
 * dead Loop to prevent incrementing of 7-seg and LED lighting after user failure
 * params: none
 * returns: none
 */
void deadLoop() {

  while(1) {
    
  }
  
}
