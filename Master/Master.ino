#include <Wire.h>

//MASTER CODE
// using 4.7kiloohm pull up resistors

//define buttons
const int button1 = 2; 

//define state of buttons
int buttonState1 = 0;

//define variables
int x = 0;


void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {

  //read state of buttons
  buttonState1 = digitalRead(button1);
  //while (Serial.available ()) {
   // char c = Serial.read();

  //  if (c == 'H') {
      if(buttonState1 == HIGH) {
      x = 1;  
      Wire.beginTransmission(5); // arbibary number can e anything between 1 to 127
      //Wire.write('H');
      Wire.write(x);
      Wire.endTransmission();
    }
  //  else if (c == 'L') {
      else if (buttonState1 == LOW) {
      x = 0;
      Wire.beginTransmission(5);
      //Wire.write('L');
      Wire.write (x);
      Wire.endTransmission();
    }
  }
