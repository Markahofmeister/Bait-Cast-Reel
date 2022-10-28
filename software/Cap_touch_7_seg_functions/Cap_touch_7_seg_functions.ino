//initializing libraries for talkie and 7-seg display
#include <Talkie.h>
#include <TalkieUtils.h>
#include <Vocab_US_Large.h>
#include <Vocab_Special.h>
#include <Adafruit_LEDBackpack.h>

//initializing pins for components 
#define cap_touch_input 3//pin 5
#define cap_touch_LED 5//pin 11

//initializing objects
Adafruit_7segment matrix = Adafruit_7segment();

static int count = 0;//will be incremented through function calls
static int timer = 10000;

void setup() {
  //setup code for 7-seg disp
   #ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment Backpack Test");
  #endif
  matrix.begin(0x70);

  //setup code for cap touch
  pinMode(cap_touch_input, INPUT);
  pinMode(cap_touch_LED, OUTPUT);

  //initializing 7-seg to zero
  matrix.print(count);
  matrix.writeDisplay();

}

void loop() {
  //initializing variables
  bool success_val;//will be used in function passes
  //test  
  success_val = cap_touch();
  seven_seg(success_val);

  timer = timer - 1000;
  if(timer == 0){
    timer = 10000;
  }
  delay(1000);//1 second delay


}

//Function Land 

void seven_seg(bool success_val){ //7-seg function - passes in a bool and static int
  if(success_val == true){//if tasks were successfull it will increment the count value and display the new value on 7-seg
    //incrementing count variable
    count++;
    matrix.print(count);
    matrix.writeDisplay();
    delay(10);//Might have to adjust delays

  }
  else{//if one of that tasks was not a success it will display the previous score count 
    matrix.print(count);
    matrix.writeDisplay();
    delay(10);//Might have to adjust delays
  }
}

bool cap_touch(){//cap-touch function passes out bool and brings in count variable;

  //initializations
  digitalWrite(cap_touch_LED,HIGH);//As soon as the function is called associated LED turns on 
  //send out audio command via talkie
  delay(timer);//have some type of delay after visual and audio commands are send out 
  
  if(digitalRead(cap_touch_input) == LOW){
     digitalWrite(cap_touch_LED,LOW);//turns off LED
    return true;
   }
   

  digitalWrite(cap_touch_LED,LOW);//turns off LED
  return false; 
}