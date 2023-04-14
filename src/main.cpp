#include <Arduino.h>
#include <SoftwareSerial.h>
#include <AccelStepper.h>

// Define "home_switches" for resetting the spinner and pins
#define x_home_switch 7
#define y_home_switch 8

// *** GLOBAL VARIABLE DECLARATIONS
// Setup the pin grid size
const int x = 3;
const int y = 3;
const int quadrantNumbers = x * y;

// Size 16 array for the states of the pins (0 = off, 1 = half, 2 = full)
int pinStates[quadrantNumbers];

// Setup the current x/y position
int currX;
int currY;
// *** END OF GLOBAL DECLARATIONS

// Setup bluetooth connection and stepper motors 1 and 2
SoftwareSerial Bluetooth(10, 9); // RX, TX
AccelStepper stepper1(1, 3, 2);
AccelStepper stepper2(1, 5, 4);

// Will reset the spinner to the "first position"
void resetSpinner() {
  stepper1.setSpeed(100);
  stepper2.setSpeed(100);
  pinMode(x_home_switch, INPUT_PULLUP);
  pinMode(y_home_switch, INPUT_PULLUP);

  Serial.println("Stepper motors are homing to start position . . .");
  while(digitalRead(x_home_switch)) {
    //TODO
  }

  while(digitalRead(y_home_switch)) {
    //TODO
  }
}

// Will reset all pins to their off position
void resetPins() {
  stepper1.setSpeed(100);
  stepper2.setSpeed(100);

  //Sequence to reset all pins
  //TODO

  // Reset back to start position
  resetSpinner();
}

// Reset x/y position variables and pinStates' values`
void resetPositionValues() {
  currX = 0;
  currY = 0;
  for(int i = 0; i < sizeof(pinStates); i++)
    pinStates[i] = 0;
}

// Will move to state change pins and initiate them
void updatePinStates(int *stateChanges) {
  //TODO

  Serial.println("Updating pins to reflect the current environment . . .");
}

// Will compare the current pin states to the new pin states and return an array of the updated positions
int *pinStateChanges(int *currStates, int *newStates) {
  int stateChanges[16];
  int index = 0;

  Serial.println("Obtaining the pin positions that have a state change . . .");
  for(int i = 0; i < sizeof(currStates); i++) {
    if(currStates[i] != newStates[i]) {
      stateChanges[index] = i;
      index++;
    }
  }

  return stateChanges;
}

// Setup function for . . . setup . . .
void setup() {
  stepper1.setMaxSpeed(1000);
  stepper2.setMaxSpeed(1000);
  stepper1.setSpeed(0);
  stepper2.setSpeed(0);

  Bluetooth.begin(9600);
  Serial.begin(9600);
  Serial.println("Waiting for command...");

  // Call reset() functions to reset position and corresponding values
  resetSpinner();
  resetPins();
  resetPositionValues();
}

void loop() {
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