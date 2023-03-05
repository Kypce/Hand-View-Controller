#include <Arduino.h>
#include <SoftwareSerial.h>
#include <AccelStepper.h>

SoftwareSerial Bluetooth(10, 9); // RX, TX
AccelStepper stepper1(1, 3, 2);
AccelStepper stepper2(1, 5, 4);

void setup()
{
  Bluetooth.begin(9600);
  Serial.begin(9600);
  Serial.println("Waiting for command...");
  stepper1.setMaxSpeed(1000);
  stepper2.setMaxSpeed(1000);
  stepper1.setSpeed(0);
  stepper2.setSpeed(0);
}

void loop()
{
  if (Bluetooth.available())
  { // wait for data received
    int data = Bluetooth.read();
    Serial.println(data);
    stepper1.setSpeed((data / 2) * 100);
    stepper2.setSpeed((data / 2) * 100);
  }
  stepper1.runSpeed();
  stepper2.runSpeed();
}