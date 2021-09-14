#include <Wire.h>

//MASTER CODE
// For prototyping this is mplemented on an Arduino Uno 
// using 4.7kiloohm pull up resistors

// MASTER and SLAVE ADDRESSES
const byte my_address = 8;
const byte slave1_address = 9;

//BUTTON PINS
const byte button1 = 2;
const byte button2 = 4;
const byte reset_button = 7;

//BUTTON 1 VARIABLES
int button1_state = 0;
int button1_last_state = 0;
int val_b1 = 1; // variable to store if switch is high or low when we read it
//this value wil be high to start with
unsigned long t_b1 = 0;
unsigned long t_0_b1 = 0; // indicate the start of something
unsigned long bounce_delay_b1 = 10; // delay to be certain the switch is not bouncing

//BUTTON 2 VARIABLES
int button2_state = 0;
int button2_last_state = 0;
int val_b2 = 1; // variable to store if switch is high or low when we read it
unsigned long t_b2 = 0;
unsigned long t_0_b2 = 0; // indicate the start of something
unsigned long bounce_delay_b2 = 10; // delay to be certain the switch is not bouncing


// RESET BUTTON VARIABLES
int reset_button_state = 0;
int reset_button_last_state = 0;
int val_reset_button = 1; // variable to store if switch is high or low when we read it
unsigned long t_reset_button = 0;
unsigned long t_0_reset_button = 0; // indicate the start of something
unsigned long bounce_delay_reset_button = 10; // delay to be certain the switch is not bouncing


// SOUND OFF BUTTON
// another button will be required to stop any audio playing
// voice acivated sound tracks with mycroft on pi communication via Xbee

//POTENTIOMETER
// idea here is to use a pot to control play modes, by mapping the pot value to a mode
// and EITHER setting mode value before transmission OR Transmit mode value too
//to the slave that corresponds with the button number pressed
// e.g if button 1 is pressed, pot value is read, mode is set, slave 1 receives data.

void setup() {
  Serial.begin(9600);
  Wire.begin(my_address);
  pinMode(button1, INPUT_PULLUP); // with input pullup the switch read high when open and low when closed
  pinMode(button2, INPUT_PULLUP);
  pinMode(reset_button, INPUT_PULLUP);
}


// BUTTON 1 STATE MACHINE
void stateMachine_button1() {
  val_b1 = digitalRead(button1);  // read pin
  button1_last_state = button1_state;  // record state

  switch (button1_state) {
    case 0: //RESET
      button1_state = 1;
      break;
    case 1: //START PROCESS wait for button to register as HIGH, then proceed to GO
      if (val_b1 == HIGH) {
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
      if (val_b1 == LOW) {
        button1_state = 0;
      }
      break;
    case 4: //ARMED
      //If switch goes LOW, proceed to TRIGGERED
      t_b1 = millis();
      if (val_b1 == LOW) {
        button1_state = 5;
      }
      break;
    case 5: //TRIGGERED
      button1_state = 0; //RESET the State Machine
      break;
  }
}

//BUTTON 2 STATE MACHINE
void stateMachine_button2() {
  val_b2 = digitalRead(button2);  // read pin
  button2_last_state = button2_state;  // record state

  switch (button2_state) {
    case 0: //RESET
      button2_state = 1;
      break;
    case 1: //START PROCESS wait for button to register as HIGH, then proceed to GO
      if (val_b2 == HIGH) {
        button2_state = 2;
      }
      break;
    case 2: //GO
      // start a timer and proceed to WAIT
      t_0_b2 = millis();
      button2_state = 3;
      break;
    case 3: //WAIT
      // wait until a certain time threshold has elapsed to allow for rejecting bounces,
      // if a bounce occurs RESET or proceed to ARMED
      t_b2 = millis(); //keep a check on the time elapsed since timer start
      if (t_b2 = t_0_b2 > bounce_delay_b2) { //current time - previouse time > threshold
        button2_state = 4;
      }
      if (val_b2 == LOW) {
        button2_state = 0;
      }
      break;
    case 4: //ARMED
      //If switch goes HIGH, check for long pause to determine if to proceed to TRIGGERED or HOLD.
      t_b2 = millis();
      if (val_b2 == LOW) {
        button2_state = 5;
      }
      break;
    case 5: //TRIGGERED
      button2_state = 0; //RESET the State Machine
      break;
  }
}

// RESET BUTTON STATE MACHINE
void stateMachine_reset_button() {
  val_reset_button = digitalRead(reset_button);  // read pin
  reset_button_last_state = reset_button_state;  // record state

  switch (reset_button_state) {
    case 0: //RESET
      reset_button_state = 1;
      break;
    case 1: //START PROCESS wait for button to register as HIGH, then proceed to GO
      if (val_reset_button == HIGH) {
        reset_button_state = 2;
      }
      break;
    case 2: //GO
      // start a timer and proceed to WAIT
      t_0_reset_button = millis();
      reset_button_state = 3;
      break;
    case 3: //WAIT
      // wait until a certain time threshold has elapsed to allow for rejecting bounces,
      // if a bounce occurs RESET or proceed to ARMED
      t_reset_button = millis(); //keep a check on the time elapsed since timer start
      if (t_reset_button = t_0_reset_button > bounce_delay_reset_button) { //current time - previous time > threshold
        reset_button_state = 4;
      }
      if (val_reset_button == LOW) {
        reset_button_state = 0;
      }
      break;
    case 4: //ARMED
      //If switch goes HIGH, check for long pause to determine if to proceed to TRIGGERED or HOLD.
      t_reset_button = millis();
      if (val_reset_button == LOW) {
        reset_button_state = 5;
      }
      break;
    case 5: //TRIGGERED
      reset_button_state = 0; //RESET the State Machine
      break;
  }
}


void loop() {
  stateMachine_button1(); //button state machine
  stateMachine_button2(); //button state machine
  stateMachine_reset_button(); // reset button state machine

  while (Serial.available ()) {
    Serial.read();
  }

  if (button1_state == 5) { // and pot mode is 1
    int x = 1; // else if button state is 5 and potmode is 2 int x = different value
    Serial.println("Start");
    Wire.beginTransmission(slave1_address);
    Serial.println("Beginning transmission");
    Serial.println();
    Wire.write(x);

    Serial.print("Sent button 1 Data:" );
    Serial.println (x);

    /* CHECK IF TRANSMISSION WAS A SUCCESS
      wire.end transmission will return:
        0 on successful write to slave,
        1 if data exceeds buffer length,
        2 on NACK on address transmit,
        3 on NACK on Data transmit,
        4 as other errors
    */
    byte err = Wire.endTransmission();
    if (Wire.endTransmission () == 0 ) {
      Serial.println("Ended transmission");
    }
    else Serial.print (err);
  }

  if (button2_state == 5) {
    int x = 2;
    Serial.println("Start");
    Wire.beginTransmission(slave1_address);
    Serial.println("Beginning transmission");
    Wire.write(x);
    Serial.print("Sent button 2 Data");
    Serial.println(x);
     byte err = Wire.endTransmission();
    if (Wire.endTransmission () == 0 ) {
      Serial.println("Ended transmission");
    }
    else Serial.print (err);
  }


  // BROADCAST MESSAGE address 0 is broadcast address
  // I would like to implement a reset button to broadcast to all slaves
  // for the moment I will just address the One slave I have connected
  
  if (reset_button_state == 5) {
    int x = 0;
    Serial.println("Start");
    Wire.beginTransmission(slave1_address);
    Serial.println("Beginning transmission");
    Wire.write(x);
    Serial.print("Sent reset button Data = ");
    Serial.println(x);
    byte err = Wire.endTransmission();
    if (Wire.endTransmission () == 0 ) {
      Serial.println("Ended transmission");
    }
    else Serial.print (err);
  }


  // MONITOR CURRENT BUTTON STATES IN SERIAL MONITOR
  if ((button1_state != button1_last_state) | (button2_state != button2_last_state) | (reset_button_state != reset_button_last_state)) {
    Serial.print ("Switch button 1 state = ");
    Serial.println(button1_state);
    Serial.print("Switch button 2 state = ");
    Serial.println(button2_state);
    Serial.print ("Switch reset button state = ");
    Serial.println (reset_button_state);
    ;
  }
}
