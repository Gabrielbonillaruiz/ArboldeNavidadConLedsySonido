/*
 *  GENIAL
 */
 
#include "TimerOne.h"
#include <TimerFreeTone.h>

#define TONE_PIN 12 // Pin you have speaker/piezo connected to (be sure to include a 100 ohm resistor).

int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262, 0 };
int duration[] = { 250, 125, 125, 250, 250, 250, 250, 250, 250 };


 
void setup()
{
  pinMode(10, OUTPUT);
  Timer1.initialize(250000);         // initialize timer1, and set a 1/2 second period
  //Timer1.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
  pinMode(3,OUTPUT);
}
 
void callback()
{
  digitalWrite(10, digitalRead(10) ^ 1);
}
 
void loop()
{
    for (int thisNote = 0; thisNote < 9; thisNote++) { // Loop through the notes in the array.
    TimerFreeTone(TONE_PIN, melody[thisNote], duration[thisNote]); // Play thisNote for duration.
    if(duration[thisNote]==250)
    {
      Timer1.pwm(9,1024);
      digitalWrite(3,LOW);
    }
    if(duration[thisNote]==125)
    {
      Timer1.pwm(9,512);
      digitalWrite(3,HIGH);
    }
  }
}
 
