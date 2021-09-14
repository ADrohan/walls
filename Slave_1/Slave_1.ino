#include <Wire.h>

//SLAVE CODE
// For prototyping this is implemented on an Arduino Uno

// VARIABLES
int button_instructions = 0; // instructions from master
int button1_state = 0;
int button2_state = 0;
int button1_last_state = 0;
int button2_last_state = 0;
int led1_state = 0;
int led2_state = 0;
const byte my_address = 9;

//LED PINS
const byte led1 = 13; // pin 13
const byte led2 = 9; // pin 9

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

void loop() {
  delay(50);
}

//called by interrupt service routine when incoming data arrives
void receiveEvent( int howMany ) { 
  //how Many is required by event handler
  while (Wire.available()) {
    button_instructions = Wire.read();
  }
// currently each slave can receive a button instruction to eiter run or reset 
// I would like to implement different user selected run modes via a potentioneter. 


  switch (button_instructions) {

    case 0: // reset has been received
      powerOffLeds();
      led1_state = 0;
      led2_state = 0;
      button1_last_state = button_instructions;
      button2_last_state = button_instructions;
      break;

    case 1: // control leds on pin 13
      if (button1_last_state == 0 && button_instructions == 1)
      {
        if (led1_state == 0)
        {
          digitalWrite(led1, HIGH);
          led1_state = 1;
        }
        else
        {
          digitalWrite(led1, LOW);
          led1_state = 0;
        }
      }
      button1_last_state = button_instructions;
      break;
      
    case 2: // control leds on pin 9
      if (button2_last_state == 0 && button_instructions == 2)
      {
        if (led2_state == 0)
        {
          digitalWrite(led2, HIGH);
          led2_state = 1;
        }
        else
        {
          digitalWrite(led2, LOW);
          led2_state = 0;
        }
      }
      button2_last_state = button_instructions;
      break;
  }
}
