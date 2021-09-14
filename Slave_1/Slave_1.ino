#include <Wire.h>

//SLAVE CODE

//define variables
int x = 0;

void setup() {
  Wire.begin(5);
  Wire.onReceive(receiveEvent);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);

}

void receiveEvent( int howMany ) {
 //how Many is required by event handler
 while (Wire.available()) {
  //char c = Wire.read();
  x = Wire.read();
  if (x = 1) {
  //if (c == 'H') {
    digitalWrite(13, HIGH);
  }
  //else if (c == 'L') {
   else if (x = 0) {
    digitalWrite(13, LOW);
  }
 }
}
