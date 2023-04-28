#include <Arduino.h>
#include <SoftwareSerial.h>
#include <AccelStepper.h>
#include <ezButton.h>

// Define "home_switches" for resetting the spinner and pins

// Rotation => Same sign for both moveTo() functions
// Negative rotates clockwise while positive rotaes it counter-clockwise (from perspective of holding device)
// Side-to-side => one moveTo() function for a motor
// Negative moves it "up" while positive moves it "down" (from perspective of holding it)

// *** GLOBAL VARIABLE DECLARATIONS
const int x = 3;
const int y = 3;
const int quadrantTotal = x * y;

// mm for rotation
// moveTo(800) == full rotation
long stepsMoveX = 16.98;
// mm for side-to-side
// This moves it 1 mm when used in moveTo()
long stepsMoveY = 274;

// Size 16 array for the states of the pins (0 = off, 1 = half, 2 = full)
int pinStates[quadrantTotal];

// Used to break the loop in void loop() and go to the doNothing() function
boolean go = true;

// Setup bluetooth connection and stepper motors 1 and 2
SoftwareSerial Bluetooth(0, 1); // RX, TX
AccelStepper stepperTop(1, 2, 3);
AccelStepper stepperBottom(1, 4, 5);
ezButton limitSwitch(6);

// Will reset the spinner to the "first position"
void resetSpinner()
{
  stepperTop.setSpeed(1200);
  stepperBottom.setSpeed(600);

  Serial.println("Stepper motors are homing to start position . . .");
  while (true)
  {
    limitSwitch.loop();
    if (limitSwitch.isPressed())
    {
      Serial.println("The limit switch: UNTOUCHED -> TOUCHED");
      break;
    }
    stepperTop.runSpeed();
    stepperBottom.runSpeed();
  }

  stepperTop.setCurrentPosition(0);
  stepperBottom.setCurrentPosition(0);
  stepperTop.setSpeed(2000);
  stepperBottom.setSpeed(2000);

  stepperTop.moveTo(-560);
  stepperBottom.moveTo(-560);
  while (stepperTop.currentPosition() != -560 || stepperBottom.currentPosition() != -560)
  {
    stepperTop.run();
    stepperBottom.run();
  }

  stepperTop.setCurrentPosition(0);
  stepperBottom.setCurrentPosition(0);

  stepperTop.moveTo(-15 * stepsMoveY);
  while (stepperTop.currentPosition() != -15 * stepsMoveY)
  {
    stepperTop.run();
  }

  stepperTop.setCurrentPosition(0);
  stepperBottom.setCurrentPosition(0);

  stepperTop.moveTo(800);
  stepperBottom.moveTo(800);
  while (stepperTop.currentPosition() != 800 || stepperBottom.currentPosition() != 800)
  {
    stepperTop.run();
    stepperBottom.run();
  }

  stepperTop.setCurrentPosition(0);
  stepperBottom.setCurrentPosition(0);

  stepperTop.moveTo(15 * stepsMoveY);
  while (stepperTop.currentPosition() != 15 * stepsMoveY)
  {
    stepperTop.run();
  }

  stepperTop.setCurrentPosition(0);
  stepperBottom.setCurrentPosition(0);
}

// Off reset, to get to first ramp 30 degrees clockwise; then to get between pins rotate 100 degrees each
// To activate x pin location, will shift rod to avoid accidental activation, rotate to position, then shift back same amount

// Distance from pin to off switch = 5 mm
// Distance from pin to pin (from shifting) = 29 mm

void offset(long len)
{
  stepperTop.moveTo(len * stepsMoveY);
  while (stepperTop.currentPosition() != len * stepsMoveY)
  {
    stepperTop.run();
  }

  stepperTop.setCurrentPosition(0);
}

// Will move just before given pin location from newPos
void moveTo(int newPos)
{
  switch (newPos)
  {
  case 0:
    Serial.println("Moving to position 0");
    offset(-5);

    stepperTop.moveTo(65);
    stepperBottom.moveTo(65);
    while (stepperTop.currentPosition() != 65 || stepperBottom.currentPosition() != 65)
    {
      stepperTop.run();
      stepperBottom.run();
    }

    delay(2000);
    resetSpinner();
    break;

  case 1:
    Serial.println("Moving to position 1");
    offset(-5);

    stepperTop.moveTo(140);
    stepperBottom.moveTo(140);
    while (stepperTop.currentPosition() != 140 || stepperBottom.currentPosition() != 140)
    {
      stepperTop.run();
      stepperBottom.run();
    }

    delay(2000);
    resetSpinner();
    break;

  case 2:
    Serial.println("Moving to position 2");
    offset(-5);

    stepperTop.moveTo(240);
    stepperBottom.moveTo(240);
    while (stepperTop.currentPosition() != 240 || stepperBottom.currentPosition() != 240)
    {
      stepperTop.run();
      stepperBottom.run();
    }

    delay(2000);
    resetSpinner();
    break;

  case 3:
    Serial.println("Moving to position 3");
    offset(-5);

    stepperTop.moveTo(300);
    stepperBottom.moveTo(300);
    while (stepperTop.currentPosition() != 300 || stepperBottom.currentPosition() != 300)
    {
      stepperTop.run();
      stepperBottom.run();
    }

    delay(2000);
    resetSpinner();
    break;

  case 4:
    Serial.println("Moving to position 4");
    offset(-5);

    stepperTop.moveTo(400);
    stepperBottom.moveTo(400);
    while (stepperTop.currentPosition() != 400 || stepperBottom.currentPosition() != 400)
    {
      stepperTop.run();
      stepperBottom.run();
    }

    delay(2000);
    resetSpinner();
    break;

  case 5:
    Serial.println("Moving to position 5");
    offset(-5);

    stepperTop.moveTo(470);
    stepperBottom.moveTo(470);
    while (stepperTop.currentPosition() != 470 || stepperBottom.currentPosition() != 470)
    {
      stepperTop.run();
      stepperBottom.run();
    }

    delay(2000);
    resetSpinner();
    break;

  case 6:
    Serial.println("Moving to position 6");
    offset(-5);

    stepperTop.moveTo(500);
    stepperBottom.moveTo(500);
    while (stepperTop.currentPosition() != 500 || stepperBottom.currentPosition() != 500)
    {
      stepperTop.run();
      stepperBottom.run();
    }

    delay(2000);
    resetSpinner();
    break;

  case 7:
    Serial.println("Moving to position 7");
    offset(-5);

    stepperTop.moveTo(620);
    stepperBottom.moveTo(620);
    while (stepperTop.currentPosition() != 620 || stepperBottom.currentPosition() != 620)
    {
      stepperTop.run();
      stepperBottom.run();
    }

    delay(2000);
    resetSpinner();
    break;

  case 8:
    Serial.println("Moving to position 8");
    offset(-5);

    stepperTop.moveTo(680);
    stepperBottom.moveTo(680);
    while (stepperTop.currentPosition() != 680 || stepperBottom.currentPosition() != 680)
    {
      stepperTop.run();
      stepperBottom.run();
    }

    delay(2000);
    resetSpinner();
    break;
  }
}

void setup()
{
  Bluetooth.begin(9600);
  Serial.begin(9600);
  Serial.println("Waiting for command...");
  stepperTop.setMaxSpeed(4000);
  stepperBottom.setMaxSpeed(4000);
  stepperTop.setAcceleration(6000);
  stepperBottom.setAcceleration(6000);
  stepperTop.setSpeed(0);
  stepperBottom.setSpeed(0);

  // Call reset() functions to reset position and corresponding values
  resetSpinner();
}

int i = 0;

void loop()
{

  if (i < 9)
  {
    moveTo(i);
    i++;
    go = false;
  }
}