#include <Wire.h>

//SLAVE CODE

//define variables
int button1_state = 0;

void setup() {
  Wire.begin(5);
  Wire.onReceive(receiveEvent);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  
}

void loop() {
  delay(50);

}

void receiveEvent( int howMany ) {
 //how Many is required by event handler
 while (Wire.available()) {
  button1_state = Wire.read();
  if (button1_state == 5) { // has been triggered
    digitalWrite(13, HIGH);
  }
  else if (button1_state == 0) {
    digitalWrite(13, LOW);
  }
 }
}
