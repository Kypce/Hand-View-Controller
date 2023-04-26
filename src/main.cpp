#include <Arduino.h>
#include <SoftwareSerial.h>
#include <AccelStepper.h>
#include <ezButton.h>

// Define "home_switches" for resetting the spinner and pins

// *** GLOBAL VARIABLE DECLARATIONS
// Setup the pin grid size
const int x = 4;
const int y = 4;
const int quadrantTotal = x * y;

long stepsMoveX = 16.98;
long stepsMoveY = 273.97;

// Size 16 array for the states of the pins (0 = off, 1 = half, 2 = full)
int pinStates[quadrantTotal];

// Setup bluetooth connection and stepper motors 1 and 2
SoftwareSerial Bluetooth(0, 1); // RX, TX
AccelStepper stepperTop(1, 2, 3);
AccelStepper stepperBottom(1, 4, 5);
ezButton limitSwitch(6);

// Will reset the spinner to the "first position"
void resetSpinner()
{
  stepperTop.setSpeed(500);
  stepperBottom.setSpeed(500);

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
}

// Will reset all pins to their off position
void resetPins()
{
  stepperTop.setSpeed(100);
  stepperBottom.setSpeed(100);

  // Sequence to reset all pins
  // TODO

  // Reset back to start position
}

// Reset x/y position variables and pinStates' values`
void resetPositionValues()
{
  for (int i = 0; i < quadrantTotal; i++)
    pinStates[i] = 0;
}

// Will move to state change pins and initiate them
void updatePinStates(int *stateChanges)
{
  Serial.println("Updating pins to reflect the current environment . . .");
}

// Will compare the current pin states to the new pin states and return an array of the updated positions
int *pinStateChanges(int *currStates, int *newStates)
{
  int stateChanges[16];
  int index = 0;

  Serial.println("Obtaining the pin positions that have a state change . . .");
  for (int i = 0; i < sizeof(currStates); i++)
  {
    if (currStates[i] != newStates[i])
    {
      stateChanges[index] = i;
      index++;
    }
  }

  return stateChanges;
}

void setup()
{
  Bluetooth.begin(9600);
  Serial.begin(9600);
  Serial.println("Waiting for command...");
  stepperTop.setMaxSpeed(1000);
  stepperBottom.setMaxSpeed(1000);
  stepperTop.setAcceleration(6000);
  stepperBottom.setAcceleration(6000);
  stepperTop.setSpeed(0);
  stepperBottom.setSpeed(0);

  // Call reset() functions to reset position and corresponding values
  resetSpinner();
  // resetPins();
  // resetPositionValues();
}

void loop()
{
  stepperTop.moveTo(-1369.85);
  stepperBottom.moveTo(1369.85);
  stepperTop.run();
  stepperBottom.run();
}