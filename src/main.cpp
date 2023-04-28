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
const int x = 4;
const int y = 4;
const int quadrantTotal = x * y;

// mm for rotation
// moveTo(800) == full rotation
long stepsMoveX = 16.98;
// mm for side-to-side
// This moves it 1 mm when used in moveTo()
long stepsMoveY = 273.97;

// Size 16 array for the states of the pins (0 = off, 1 = half, 2 = full)
int pinStates[quadrantTotal];

// Used to break the loop in void loop() and go to the doNothing() function
boolean go = true;

// Setup bluetooth connection and stepper motors 1 and 2
SoftwareSerial Bluetooth(0, 1); // RX, TX
AccelStepper stepperTop(1, 2, 3);
AccelStepper stepperBottom(1, 4, 5);
ezButton limitSwitch(6);

// Literally does nothing, used to psuedo-break out of void loop()
void doNothing()
{
  return;
}

// Will reset the spinner to the "first position"
void resetSpinner()
{
  stepperTop.setSpeed(800);
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
  stepperTop.setSpeed(-600);
  unsigned long starttime = millis();
  unsigned long endtime = starttime;
  while ((endtime - starttime) <= 200)
  {
    stepperTop.moveTo(0.1);
    stepperTop.run();
    endtime = millis();
  }
  stepperTop.setCurrentPosition(0);
  stepperBottom.setCurrentPosition(0);
  stepperTop.setSpeed(2000);
  stepperBottom.setSpeed(2000);
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
/*int *pinStateChanges(int *currStates, int *newStates) {
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
*/

// Moves side-to-side "len" mm amount
void sideMM(long len)
{
  unsigned long starttime = millis();
  unsigned long endtime = starttime;
  while ((endtime - starttime) <= 3000)
  {
    // code here
    // Moving len mm side-to-side
    stepperTop.moveTo(stepsMoveY * len);
    stepperTop.run();
    endtime = millis();
  }

  delay(1000);

  starttime = millis();
  endtime = starttime;
  while ((endtime - starttime) <= 3000)
  {
    // code here
    stepperTop.moveTo(-stepsMoveY * len);
    stepperTop.run();
    endtime = millis();
  }

  delay(1000);
}

// Rotates rod back and forth len amount
void rotate(long len)
{
  unsigned long starttime = millis();
  unsigned long endtime = starttime;
  while ((endtime - starttime) <= 3000)
  {
    // code here
    // Rotate len amount
    stepperTop.moveTo(-len);
    stepperBottom.moveTo(-len);
    stepperTop.run();
    stepperBottom.run();
    endtime = millis();
  }

  delay(1000);

  starttime = millis();
  endtime = starttime;
  while ((endtime - starttime) <= 3000)
  {
    // code here
    stepperTop.moveTo(len);
    stepperBottom.moveTo(len);
    stepperTop.run();
    stepperBottom.run();
    endtime = millis();
  }

  delay(1000);
}

// From reset, moves to "pos" position
// ****NOTE: DID NOT CODE FROM RESET AS RESET WAS NOT INITIATED/FOUND YET****
void moveToPos(int pos)
{
  unsigned long starttime;
  unsigned long endtime;

  switch (pos)
  {
  // rotate 270, move 9 mm
  case 1:
    Serial.println("Going to position 1!");
    starttime = millis();
    endtime = starttime;
    while ((endtime - starttime) <= 3000)
    {
      // Serial.println("1st while");
      stepperTop.moveTo(-270);
      stepperBottom.moveTo(-270);
      stepperTop.run();
      stepperBottom.run();
      endtime = millis();
    }
    starttime = millis();
    endtime = starttime;
    while ((endtime - starttime) <= 3000)
    {
      // Serial.println("2nd while");
      stepperTop.moveTo(-stepsMoveY * 9);
      stepperTop.run();
      endtime = millis();
    }
    break;

  default:
    Serial.println("Unknown input given!");
    break;
  }

  Serial.println("Ended moveToPos function!");
  return;
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
  // resetPins();
  // resetPositionValues();
}

void loop()
{
  // sideMM(10);
  // rotate(400);
}