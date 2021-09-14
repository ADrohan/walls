#include <Wire.h>

//MASTER CODE
// using 4.7kiloohm pull up resistors

//define buttons
const int button1 = 2; 

//define state of buttons
int button1_state = 0;
int button1_last_state = 0;
int val_b1 = 0; // variable to store if switch is high or low when we read it
unsigned long t_b1 = 0;
unsigned long t_0_b1 = 0; // indicate the start of something
unsigned long bounce_delay_b1 = 10; // delay we must see to be certain the switch is not bouncing


void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(button1, INPUT); 
}

void loop() {
  SM_b1();
  //read state of buttons
 // button1_state = digitalRead(button1);
  while (Serial.available ()) {
    Serial.read();
  }
                                                   
 // if (button1_state != button1_last_state){                     
 //   button1_last_state = button1_state;
    Serial.println("Start");
    Wire.beginTransmission(5);
    Serial.println("Beginning transmission");
    Wire.write(button1_state);
    Serial.print(button1_state);
    Serial.println("Sent Data");
    Wire.endTransmission();
    Serial.println("Ended transmission");
 // }
}

// button state machine
void SM_b1() {
  val_b1 = digitalRead(button1);  // read pin
  button1_last_state = button1_state;  // record state

  switch (button1_state) {
    case 0: //RESET
      button1_state = 1;
      break;
    case 1: //START PROCESS wait for button to register as LOW, then proceed to GO
      if (val_b1 == LOW) {
        button1_state = 2;
      }
      break;
    case 2: //GO
      // start a timer and proceed to WAIT
      t_0_b1 = millis();
      button1_state = 3;
      break;
    case 3: //WAIT
      // wait until a certain time threshold has elapsed to allow for rejecting bounces,
      // if a bounce occurs RESET or proceed to ARMED
      t_b1 = millis(); //keep a check on the time elapsed since timer start
      if (t_b1 = t_0_b1 > bounce_delay_b1) { //current time - previouse time > threshold
        button1_state = 4;
      }
      if (val_b1 == HIGH) {
        button1_state = 0;
      }
      break;
    case 4: //ARMED
      //If switch goes HIGH, check for long pause to determine if to proceed to TRIGGERED or HOLD.
      t_b1 = millis();
      if (val_b1 == HIGH) {
        button1_state = 5;
      }
      break;
    case 5: //TRIGGERED
      button1_state = 0; //RESET the State Machine
      break;
  }
}
  
