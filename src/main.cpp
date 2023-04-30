#include <Arduino.h>
#include <SoftwareSerial.h>
#include <AccelStepper.h>
#include <ezButton.h>

// Define "home_switches" for resetting the spinner and pins

/* MOTOR MOVEMENT INFO
 *   Rotation => Same sign for both moveTo() functions
 *      Negative rotates clockwise while positive rotaes it counter-clockwise (from perspective of holding device)
 *   Side-to-side => one moveTo() function for a motor
 *      Negative moves it "up" while positive moves it "down" (from perspective of holding it)
 */

/* DEGREES FROM RESET TO ALL PIN POSITIONS (spinnning only couter-clockwise)
 *  Reset =>  0 degrees
 *  Pin 0 =>  25 degrees
 *  Pin 1 =>  70 degrees
 *  Pin 2 => 115 degrees
 *  Pin 3 => 125 degrees
 *  Pin 4 => 170 degrees
 *  Pin 5 => 215 degrees
 *  Pin 6 => 225 degrees
 *  Pin 7 => 270 degrees
 *  Pin 8 => 315 degrees
 *  Reset => 360 degrees
 */

/* DEGREES FROM RESET TO ALL PIN POSITIONS (spinnning only clockwise)
 *  Reset => 360 degrees
 *  Pin 0 => 335 degrees
 *  Pin 1 => 290 degrees
 *  Pin 2 => 245 degrees
 *  Pin 3 => 235 degrees
 *  Pin 4 => 190 degrees
 *  Pin 5 => 145 degrees
 *  Pin 6 => 135 degrees
 *  Pin 7 =>  90 degrees
 *  Pin 8 =>  45 degrees
 *  Reset =>   0 degrees
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

// Used to break the loop in void loop() and go to the doNothing() function
boolean go = true;

// From reset[0], the amount of degrees to get to position (spinning counter-clockwise)
int resetCounter[] = {0, 25, 70, 115, 125, 170, 215, 225, 270, 315};
// From reset[0], the amount of degrees to get to position (spinning clockwise)
int resetClock[] = {0, 45, 90, 135, 145, 190, 235, 245, 290, 335};

// Setup bluetooth connection and stepper motors 1 and 2
SoftwareSerial Bluetooth(0, 1); // RX, TX
AccelStepper stepperTop(1, 2, 3);
AccelStepper stepperBottom(1, 4, 5);
ezButton limitSwitch(6);
// GLOBAL VARIABLE DECLARATIONS END --------------------------------------------------------

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
  stepperTop.moveTo(-560);
  stepperBottom.moveTo(-560);
  while (stepperTop.currentPosition() != -560 || stepperBottom.currentPosition() != -560)
  {
    stepperTop.run();
    stepperBottom.run();
  }

  stepperTop.setCurrentPosition(0);
  stepperBottom.setCurrentPosition(0);

  // Moves rod up "15 mm"
  stepperTop.moveTo(-15 * stepsMoveY);
  while (stepperTop.currentPosition() != -15 * stepsMoveY)
  {
    stepperTop.run();
  }

  stepperTop.setCurrentPosition(0);
  stepperBottom.setCurrentPosition(0);

  // Rotates rod 360 degrees
  stepperTop.moveTo(800);
  stepperBottom.moveTo(800);
  while (stepperTop.currentPosition() != 800 || stepperBottom.currentPosition() != 800)
  {
    stepperTop.run();
    stepperBottom.run();
  }

  stepperTop.setCurrentPosition(0);
  stepperBottom.setCurrentPosition(0);

  // Moves rod down "15 mm"
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

// Give the degrees we want and it converts to steps
long degreesToSteps(long deg)
{
  return deg * 2.22222222;
}

// Moves the rod "len" amount
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
  int val;

  switch (newPos)
  {
  // Moves to position 0
  case 0:
    Serial.println("Moving to position 0");
    offset(-5);

    val = degreesToSteps(30);
    stepperTop.moveTo(val);
    stepperBottom.moveTo(val);
    while (stepperTop.currentPosition() != val || stepperBottom.currentPosition() != val)
    {
      stepperTop.run();
      stepperBottom.run();
    }

    delay(2000);
    resetSpinner();
    break;

  // Moves to position 1
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

  // Moves to position 2
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

  // Moves to position 3
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

  // Moves to position 4
  case 4:
    Serial.println("Moving to position 4");
    offset(-5);

    val = degreesToSteps(170);
    stepperTop.moveTo(val);
    stepperBottom.moveTo(val);
    while (stepperTop.currentPosition() != val || stepperBottom.currentPosition() != val)
    {
      stepperTop.run();
      stepperBottom.run();
    }

    delay(2000);
    resetSpinner();
    break;

  // Moves to position 5
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

  // Moves to position 6
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

  // Moves to position 7
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

  // Moves to position 8
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

void newMoveTo(int pos)
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

    offset(-10);

    stepperTop.moveTo(degreesToSteps(diff));
    stepperBottom.moveTo(degreesToSteps(diff));
    while (stepperTop.currentPosition() != degreesToSteps(diff) || stepperBottom.currentPosition() != degreesToSteps(diff))
    {
      stepperTop.run();
      stepperBottom.run();
    }

    offset(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);
    break;

  case 2:
    Serial.println("Moving to position 1");
    if (pos > currPos)
      diff = abs(resetCounter[currPos] - resetCounter[pos]);
    else
      diff = -1 * (resetClock[currPos] - resetClock[pos]);

    offset(-10);

    stepperTop.move(degreesToSteps(diff));
    stepperBottom.moveTo(degreesToSteps(diff));
    while (stepperTop.currentPosition() != degreesToSteps(diff) || stepperBottom.currentPosition() != degreesToSteps(diff))
    {
      stepperTop.run();
      stepperBottom.run();
    }

    offset(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);
    break;

  case 3:
    Serial.println("Moving to position 2");
    if (pos > currPos)
    {
      diff = abs(resetCounter[currPos] - resetCounter[pos]);
      steps = degreesToSteps(diff) + 7;
    }
    else
    {
      diff = -1 * (resetClock[currPos] - resetClock[pos]);
      steps = degreesToSteps(diff) - 13;
    }

    offset(-10);

    stepperTop.moveTo(steps);
    stepperBottom.moveTo(steps);
    while (stepperTop.currentPosition() != steps || stepperBottom.currentPosition() != steps)
    {
      stepperTop.run();
      stepperBottom.run();
    }

    offset(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);

    break;

  case 4:
    Serial.println("Moving to position 3");
    if (pos > currPos)
      diff = abs(resetCounter[currPos] - resetCounter[pos]);
    else
      diff = -1 * (resetClock[currPos] - resetClock[pos]);

    offset(-10);

    stepperTop.moveTo(degreesToSteps(diff));
    stepperBottom.moveTo(degreesToSteps(diff));
    while (stepperTop.currentPosition() != degreesToSteps(diff) || stepperBottom.currentPosition() != degreesToSteps(diff))
    {
      stepperTop.run();
      stepperBottom.run();
    }

    offset(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);

    break;

  case 5:
    Serial.println("Moving to position 4");
    if (pos > currPos)
      diff = abs(resetCounter[currPos] - resetCounter[pos]);
    else
      diff = -1 * (resetClock[currPos] - resetClock[pos]);

    offset(-10);

    stepperTop.moveTo(degreesToSteps(diff));
    stepperBottom.moveTo(degreesToSteps(diff));
    while (stepperTop.currentPosition() != degreesToSteps(diff) || stepperBottom.currentPosition() != degreesToSteps(diff))
    {
      stepperTop.run();
      stepperBottom.run();
    }

    offset(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);
    break;

  case 6:
    Serial.println("Moving to position 6");
    if (pos > currPos)
    {
      diff = abs(resetCounter[currPos] - resetCounter[pos]);
      steps = degreesToSteps(diff) + 7;
    }
    else
    {
      diff = -1 * (resetClock[currPos] - resetClock[pos]);
      steps = degreesToSteps(diff) - 13;
    }

    offset(-10);

    stepperTop.moveTo(steps);
    stepperBottom.moveTo(steps);
    while (stepperTop.currentPosition() != steps || stepperBottom.currentPosition() != steps)
    {
      stepperTop.run();
      stepperBottom.run();
    }

    offset(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);

    break;

  case 7:
    Serial.println("Moving to position 7");
    if (pos > currPos)
      diff = abs(resetCounter[currPos] - resetCounter[pos]);
    else
      diff = -1 * (resetClock[currPos] - resetClock[pos]);

    offset(-10);

    stepperTop.moveTo(degreesToSteps(diff));
    stepperBottom.moveTo(degreesToSteps(diff));
    while (stepperTop.currentPosition() != degreesToSteps(diff) || stepperBottom.currentPosition() != degreesToSteps(diff))
    {
      stepperTop.run();
      stepperBottom.run();
    }

    offset(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);

    break;

  case 8:
    Serial.println("Moving to position 8");
    if (pos > currPos)
      diff = abs(resetCounter[currPos] - resetCounter[pos]);
    else
      diff = -1 * (resetClock[currPos] - resetClock[pos]);

    offset(-10);

    stepperTop.moveTo(degreesToSteps(diff));
    stepperBottom.moveTo(degreesToSteps(diff));
    while (stepperTop.currentPosition() != degreesToSteps(diff) || stepperBottom.currentPosition() != degreesToSteps(diff))
    {
      stepperTop.run();
      stepperBottom.run();
    }

    offset(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);

    break;

  case 9:
    Serial.println("Moving to position 9");
    if (pos > currPos)
    {
      diff = abs(resetCounter[currPos] - resetCounter[pos]);
      steps = degreesToSteps(diff) + 7;
    }
    else
    {
      diff = -1 * (resetClock[currPos] - resetClock[pos]);
      steps = degreesToSteps(diff) - 13;
    }

    offset(-10);

    stepperTop.moveTo(steps);
    stepperBottom.moveTo(steps);
    while (stepperTop.currentPosition() != steps || stepperBottom.currentPosition() != steps)
    {
      stepperTop.run();
      stepperBottom.run();
    }

    offset(10);
    stepperTop.setCurrentPosition(0);
    stepperBottom.setCurrentPosition(0);

    break;

  default:
    Serial.println("Invalid position");
    break;
  }

  Serial.printf("Changing currPos from %i to %i\n", currPos, pos);
  currPos = pos;
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

  // DELETE LATER --------------------------------------------------------------------------------------------------------------------------------------------
  offset(-5);

  resetSpinner();
}

int i = 0;

void loop()
{

  /*if (i < 9)
  {
    moveTo(i);
    i++;
    go = false;
  }*/

  if (Bluetooth.available())
  {
    int data = Bluetooth.read();
    Serial.printf("Received %i\n", data);
    newMoveTo(data + 1);
  }
}