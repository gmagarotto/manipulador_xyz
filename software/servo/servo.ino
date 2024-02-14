/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Arduino.h>
#include <Servo.h>
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int pos_min = 0;    // variable to store the servo position
int pos_max = 180;    // variable to store the servo position
int inc_dec = 2;    // variable to store the servo position

void setup() {
  myservo.attach(PIN_PB5);  // attaches the servo on pin 9 to the servo object
}

/*void loop() {
  for (pos = pos_min; pos < pos_max; pos += inc_dec) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15 ms for the servo to reach the position
  }
  delay(50);
  for (pos  = pos_max; pos > pos_min; pos -= inc_dec) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15 ms for the servo to reach the position
  }
  delay(50);
}
*/
void loop(){
  myservo.write(0 );
  delay(1000);
  myservo.write(150 );
  delay(1000);
}