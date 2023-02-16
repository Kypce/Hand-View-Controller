#include <Arduino.h>

/*
 *   Basic example code for controlling a stepper without library
 *
 *   by Dejan, https://howtomechatronics.com
 */

// defines pins
#define stepPin 5
#define dirPin 4
#define stepPin2 3
#define dirPin2 2

void setup()
{
  // Sets the two pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
}
void loop()
{
  digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for (int x = 0; x < 200; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(700); // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(stepPin, LOW);
    delayMicroseconds(700);
  }
  delay(1000); // One second delay

  digitalWrite(dirPin, LOW); // Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for (int x = 0; x < 400; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
  delay(1000);

  digitalWrite(dirPin2, HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for (int x = 0; x < 200; x++)
  {
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(700); // by changing this time delay between the steps we can change the rotation speedv
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(700);
  }
  delay(1000); // One second delay

  digitalWrite(dirPin2, LOW); // Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for (int x = 0; x < 400; x++)
  {
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(500);
  }
  delay(1000);
}