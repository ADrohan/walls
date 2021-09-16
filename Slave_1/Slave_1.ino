
#include <Wire.h>

//SLAVE CODE
// For prototyping this is implemented on an Arduino Uno

//LED PINS
const byte led1 = 13; // pin 13
const byte led2 = 9; // pin 9

const byte my_address = 9;

// VARIABLES
volatile int button_instructions = 0; // instructions from master
int button1_last_state = 0;
int button2_last_state = 0;
byte led1_state = LOW; // record if leds ore on or off
byte led2_state = LOW;
byte pin_state = HIGH;

unsigned long currentMillis = 0;    // stores the value of millis() in each iteration of loop()
unsigned long led1_previousMillis = 0; // last time LED was updated
unsigned long led2_previousMillis = 0;
long led_longInterval = 5000; // longer no of milliseconds between blinks
long led_shortInterval = 1000; // no of milliseconds between blinks
const int blinkDuration = 500;

void setUpPins() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void powerOffLeds() {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
}

void setup() {
  setUpPins();
  Serial.begin(9600);
  Wire.begin(my_address);
  Wire.onReceive(receiveEvent); 
  powerOffLeds();
}

//=======

//called by interrupt service routine when incoming data arrives

void receiveEvent( int howMany ) { 
  //how Many is required by event handler
  while (Wire.available()) {
    button_instructions = Wire.read();
  }  
}


void update_led1_blinkState() {
  
  if (led1_state == LOW) {
    // if the led is off,wait for  to expire before turning it on
    if (currentMillis - led1_previousMillis >= led_shortInterval) {
      // time is up so turn LED on
       led1_state = HIGH;
       // save the time the change was made
       led1_previousMillis += led_shortInterval;
    }
  }
  else {
    // if LED is on, wait for interval to expire before turning it on
    if (currentMillis - led1_previousMillis >= blinkDuration) {
      // time is up so turn LED off
       led1_state = LOW;
       //save the time the change was made
       led1_previousMillis += blinkDuration;
    } 
  }    
}

//=======

void update_led2_blinkState() {
  if (led2_state == LOW) {
    if (currentMillis - led2_previousMillis >= led_longInterval) {
       led2_state = HIGH;
       led2_previousMillis += led_longInterval;
    }
  }
  else {
    if (currentMillis - led2_previousMillis >= blinkDuration) {
       led2_state = LOW;
       led2_previousMillis += blinkDuration;
    } 
  }    
}

//========

void activateBlink() {
      // this is the code that actually switches the LEDs on and off
  digitalWrite(led1, led1_state);
  digitalWrite(led2, led2_state);
}

//=======

// currently each slave can receive a button instruction to eiter run or reset 
// I would like to implement different user selected run modes via a potentioneter. 


void loop() {
  delay(50);
  currentMillis = millis();
  
  switch (button_instructions) {
     button1_last_state = button_instructions;
     button2_last_state = button_instructions;
     
    case 0: // reset has been received
      powerOffLeds();
      break;

    case 1: // control leds on pin 13
      if (button1_last_state == 0 && button_instructions == 1)
      {
        update_led1_blinkState();
        update_led2_blinkState(); 
        activateBlink();
      }
      break;
      
    case 2: // control leds on pin 9
      if (button2_last_state == 0 && button_instructions == 2)
      {
        if (led2_state == LOW) // is off
        {
         analogWrite(led2,50);
        }
        else
        {
          digitalWrite(led2, LOW);
        }
      }
      break;
  }
}
