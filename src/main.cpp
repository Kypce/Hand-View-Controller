#include <Arduino.h>
#include <SoftwareSerial.h>
#include <AccelStepper.h>
#include <ezButton.h>
#include <array>

// Define "home_switches" for resetting the spinner and pins

/* GENERAL INFO STUFF AND THINGS :D
 *
 *  To activate x pin location, will shift rod to avoid accidental activation, rotate to position, then shift back same amount
 *  Distance from pin to off switch = 5 mm
 *  Distance from pin to pin (from shifting) = 29 mm
 *
 *  MOTOR MOVEMENT INFO
 *     Rotation => Same sign for both moveTo() functions
 *       Negative rotates clockwise while positive rotaes it counter-clockwise (from perspective of holding device)
 *    Side-to-side => one moveTo() function for a motor
 *       Negative moves it "up" while positive moves it "down" (from perspective of holding it)
 *
 *
 *  DEGREES FROM RESET TO ALL PIN POSITIONS (spinnning only couter-clockwise)
 *    Reset =>  0 degrees
 *    Pin 0 =>  25 degrees
 *    Pin 1 =>  70 degrees
 *    Pin 2 => 115 degrees
 *    Pin 3 => 125 degrees
 *    Pin 4 => 170 degrees
 *    Pin 5 => 215 degrees
 *    Pin 6 => 225 degrees
 *    Pin 7 => 270 degrees
 *    Pin 8 => 315 degrees
 *    Reset => 360 degrees
 *
 *
 *  DEGREES FROM RESET TO ALL PIN POSITIONS (spinnning only clockwise)
 *    Reset => 360 degrees
 *    Pin 0 => 335 degrees
 *    Pin 1 => 290 degrees
 *    Pin 2 => 245 degrees
 *    Pin 3 => 235 degrees
 *    Pin 4 => 190 degrees
 *    Pin 5 => 145 degrees
 *    Pin 6 => 135 degrees
 *    Pin 7 =>  90 degrees
 *    Pin 8 =>  45 degrees
 *    Reset =>   0 degrees
 */

// GLOBAL VARIABLE DECLARATIONS START ------------------------------------------------------
// Sets up the size of the system being used (3 x 3 grid)
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

// Keeps track of current position (last initiated pin locations)
// -1 represents reset position
int currPos = 0;

// From reset[0], the amount of degrees to get to position (spinning counter-clockwise)
int resetCounter[] = {0, 25, 70, 115, 125, 170, 215, 225, 270, 315};
// From reset[0], the amount of degrees to get to position (spinning clockwise)
int resetClock[] = {0, 45, 90, 135, 145, 190, 235, 245, 290, 335};

// Array of current settings of each pin location (0 = off, 1 = full)
int currSett[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Array of next settings of each pin location
int nextSett[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Array that will contain which indices have a different value from currSett[] to nextSett[]
int pinChanges[9];

// Keeps track of index for pinChanges array
int j = 0;

// Setup bluetooth connection and stepper motors 1 and 2
SoftwareSerial Bluetooth(0, 1); // RX, TX
AccelStepper stepperTop(1, 2, 3);
AccelStepper stepperBottom(1, 4, 5);
ezButton limitSwitch(6);
// GLOBAL VARIABLE DECLARATIONS END --------------------------------------------------------

// Give the degrees we want and it converts to steps
long degreesToSteps(long deg)
{
  return deg * 2.22222222;
}

// Moves the rod "len" amount
void upDownMove(long len)
{
  stepperTop.moveTo(len * stepsMoveY);
  while (stepperTop.currentPosition() != len * stepsMoveY)
  {
    stepperTop.run();
  }

  stepperTop.setCurrentPosition(0);
}

// Rotates rod "len" amount
void rotateRod(long len)
{
  stepperTop.moveTo(len);
  stepperBottom.moveTo(len);
  while (stepperTop.currentPosition() != len || stepperBottom.currentPosition() != len)
  {
    stepperTop.run();
    stepperBottom.run();
  }

  stepperTop.setCurrentPosition(0);
  stepperBottom.setCurrentPosition(0);
}

// Updates currSett[] with new state
void updateSett(int update)
{
  if (currSett[update] == 0)
    currSett[update] = 1;
  else
    currSett[update] = 0;
}

// Checking for state changes from currSett[] to nextSett[] and storing indices of changes in pinChanges[]
void checkState()
{
  for (unsigned int i = 0; i < sizeof(currSett) / sizeof(int); i++)
  {
    Serial.printf("currSett[%i]: %i; nextSett[%i]: %i\n", i, currSett[i], i, nextSett[i]);
    if (currSett[i] != nextSett[i])
    {
      pinChanges[j] = i;
      Serial.printf("Pin setting update at %i\n", i);
      j++;
    }
  }
}

// Resetting pinChanges[] back to -1
void resetPinChanges()
{
  for (unsigned int i = 0; i < sizeof(pinChanges) / sizeof(int); i++)
    pinChanges[i] = -1;
  j = 0;
}

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

  // Rotates rod to "empty" slot on rod
  rotateRod(-560);

  // Moves rod up "15 mm"
  upDownMove(-15);

  // Rotates rod 360 degrees
  rotateRod(800);

  // Moves rod down "15 mm"
  upDownMove(5);
}

// Will move given pin location from newPos
void moveTo(int pos)
{
  int diff;
  int steps;

  switch (pos)
  {
  case 1:
    Serial.println("Moving to position 0");
    if (pos > currPos)
      diff = abs(resetCounter[currPos] - resetCounter[pos]);
    else
      diff = -1 * (resetClock[currPos] - resetClock[pos]);

    upDownMove(-10);

    rotateRod(degreesToSteps(diff));

    upDownMove(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);

    updateSett(pos - 1);
    break;

  case 2:
    Serial.println("Moving to position 1");
    if (pos > currPos)
      diff = abs(resetCounter[currPos] - resetCounter[pos]);
    else
      diff = -1 * (resetClock[currPos] - resetClock[pos]);

    upDownMove(-10);

    rotateRod(degreesToSteps(diff));

    upDownMove(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);

    updateSett(pos - 1);
    break;

  case 3:
    Serial.println("Moving to position 2");
    if (pos > currPos)
    {
      diff = abs(resetCounter[currPos] - resetCounter[pos]);
      steps = degreesToSteps(diff) - 20;
    }
    else
    {
      diff = -1 * (resetClock[currPos] - resetClock[pos]);
      steps = degreesToSteps(diff) + 20;
    }

    upDownMove(-10);

    rotateRod(steps);

    upDownMove(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);

    updateSett(pos - 1);
    break;

  case 4:
    Serial.println("Moving to position 3");
    if (pos > currPos)
      diff = abs(resetCounter[currPos] - resetCounter[pos]);
    else
      diff = -1 * (resetClock[currPos] - resetClock[pos]);

    upDownMove(-10);

    rotateRod(degreesToSteps(diff));

    upDownMove(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);

    updateSett(pos - 1);
    break;

  case 5:
    Serial.println("Moving to position 4");
    if (pos > currPos)
      diff = abs(resetCounter[currPos] - resetCounter[pos]);
    else
      diff = -1 * (resetClock[currPos] - resetClock[pos]);

    upDownMove(-10);

    rotateRod(degreesToSteps(diff));

    upDownMove(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);

    updateSett(pos - 1);
    break;

  case 6:
    Serial.println("Moving to position 6");
    if (pos > currPos)
    {
      diff = abs(resetCounter[currPos] - resetCounter[pos]);
      steps = degreesToSteps(diff) - 20;
    }
    else
    {
      diff = -1 * (resetClock[currPos] - resetClock[pos]);
      steps = degreesToSteps(diff) + 20;
    }

    upDownMove(-10);

    rotateRod(steps);

    upDownMove(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);

    updateSett(pos - 1);
    break;

  case 7:
    Serial.println("Moving to position 7");
    if (pos > currPos)
      diff = abs(resetCounter[currPos] - resetCounter[pos]);
    else
      diff = -1 * (resetClock[currPos] - resetClock[pos]);

    upDownMove(-10);

    rotateRod(degreesToSteps(diff));

    upDownMove(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);

    updateSett(pos - 1);
    break;

  case 8:
    Serial.println("Moving to position 8");
    if (pos > currPos)
      diff = abs(resetCounter[currPos] - resetCounter[pos]);
    else
      diff = -1 * (resetClock[currPos] - resetClock[pos]);

    upDownMove(-10);

    rotateRod(degreesToSteps(diff));

    upDownMove(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);

    updateSett(pos - 1);
    break;

  case 9:
    Serial.println("Moving to position 9");
    if (pos > currPos)
    {
      diff = abs(resetCounter[currPos] - resetCounter[pos]);
      steps = degreesToSteps(diff) - 20;
    }
    else
    {
      diff = -1 * (resetClock[currPos] - resetClock[pos]);
      steps = degreesToSteps(diff) + 20;
    }

    upDownMove(-10);

    rotateRod(steps);

    upDownMove(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);

    updateSett(pos - 1);
    break;

  default:
    Serial.println("Invalid position");
    break;
  }

  Serial.printf("Changing currPos from %i to %i\n", currPos, pos);
  currPos = pos;
}

// Running through pinChanges[] updating pin settings at state changes pins
void updatePins()
{
  for (int i = 0; i < j; i++)
  {
    if (pinChanges[i] < 0)
      break;
    moveTo(pinChanges[i] + 1);
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

  // DELETE LATER --------------------------------------------------------------------------------------------------------------------------------------------
  upDownMove(-5);
  // ---------------------------------------------------------------------------------------------------------------------------------------------------------

  // Call reset() functions to reset position and corresponding values
  resetSpinner();
}

void loop()
{
  if (Bluetooth.available())
  {
    // Receiving data from iPhone and pushing into nextSett[] array
    int data = Bluetooth.read();
    Serial.printf("Received %i\n", data);
    for (unsigned int i = 0; i < sizeof(currSett) / sizeof(int); i++)
    {
      delay(50);
      if (Bluetooth.available())
      {
        data = Bluetooth.read();
        nextSett[i] = data;
      }
    }

    checkState();

    updatePins();

    resetPinChanges();
  }
}