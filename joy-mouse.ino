#include <Mouse.h>

// Define Pins

const int startEmulation = 4;      // switch to turn on and off mouse emulation
const int mouseLeftButton = 2;    // input pin for the mouse left Button
const int joystickX = A1;         // joystick X axis
const int joystickY = A0;         // joystick Y axis

// parameters for reading the joystick:
int cursorSpeed = 10;               // output speed of X or Y movement
int responseDelay = 5;        // response delay of the mouse, in ms
int threshold = cursorSpeed/5;      // resting threshold
int lowerThreshold = cursorSpeed/2;      // resting threshold
int upperThreshold = 3*cursorSpeed/4;      // resting threshold
int center = cursorSpeed/2;         // resting position value

boolean mouseIsActive = true;    // whether or not to control the mouse
int lastSwitchState = LOW;        // previous switch state

void setup() {
	pinMode(startEmulation, INPUT_PULLUP);   // the switch pin
	pinMode(mouseLeftButton, INPUT_PULLUP);  // the left mouse button pin

	Mouse.begin();  // take control of the mouse
}

void loop() {
	// read the switch:
	int switchState = digitalRead(startEmulation);

	// if it's changed and it's high, toggle the mouse state:
	if (switchState != lastSwitchState) {
		if (switchState == LOW) {
			mouseIsActive = !mouseIsActive;
		}
	}

	// save switch state for next loop:
	lastSwitchState = switchState;

	// read and scale the two axes:
	int xReading = readAxis(A1,10);
	int yReading = readAxis(A0,0);

	// if the mouse control state is active, move the mouse:
	if (mouseIsActive) {
		Mouse.move(xReading, yReading, 0); // (x, y, scroll mouse wheel)
	}

	// read the mouse button and click or not click:
	// if the mouse button is pressed:
	if (digitalRead(mouseLeftButton) == LOW) {
		// if the mouse is not pressed, press it:
		if (!Mouse.isPressed(MOUSE_LEFT)) {
			Mouse.press(MOUSE_LEFT);
			delay(100); // delay to enable single and double-click
		}
	}

	// else the mouse button is not pressed:
	else {
		// if the mouse is pressed, release it:
		if (Mouse.isPressed(MOUSE_LEFT)) {
			Mouse.release(MOUSE_LEFT);
		}
	}

	delay(responseDelay);
}

/*distance = 0;
	}
 else if((abs(distance) > lowerThreshold) && (abs(distance) < upperThreshold)) {
    distance = 3*distance/2;
 }
 else if(abs(distance) > upperThreshold) {
    distance = 3*distance;
 }
 else if(abs(distance) == cursorSpeed) {
    distance = 5*distance;
 }
 distance = distance/2;
  reads an axis (0 or 1 for x or y) and scales the
   analog input range to a range from 0 to
 */

int readAxis(int thisAxis, int offset) {
  // read the analog input:
  int reading = analogRead(thisAxis);
 reading = reading + offset;

  // map the reading from the analog input range to the output range:
  reading = map(reading, 0, 1023, 0, cursorSpeed);

  // if the output reading is outside from the
  // rest position threshold,  use it:
  int distance = reading - center;

  if (abs(distance) < lowerThreshold) {
    distance=0;
  }
    //

	// return the distance for this axis:
	return distance;
}
