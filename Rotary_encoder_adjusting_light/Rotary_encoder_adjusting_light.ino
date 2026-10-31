// Arduino pins connected to the rotary encoder.
// CLK and DT produce signals that let us detect rotation and direction.
const int CLK_PIN = 2;
const int DT_PIN = 3;

// D11 supports PWM, which lets us control the LED's brightness.
const int LED_PIN = 11;

// Store the current brightness:
// 0 = off, 255 = maximum, 128 = roughly half the PWM range.
int brightness = 128;

// Remember the previous CLK reading so we can detect a change.
int lastCLK;

// setup() runs once when the Arduino starts or resets.
void setup() {
  // Read CLK and DT as digital inputs.
  // INPUT_PULLUP enables internal resistors that hold the inputs HIGH
  // until the encoder connects them to ground, making them LOW.
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);

  // Configure the LED pin to send an output signal.
  pinMode(LED_PIN, OUTPUT);

  // Record CLK's starting state before we begin checking for rotation.
  lastCLK = digitalRead(CLK_PIN);

  // Set the LED to its initial brightness.
  // analogWrite() uses PWM: rapidly switching the LED on and off.
  // Higher values keep it on for a larger fraction of each cycle.
  analogWrite(LED_PIN, brightness);
}

// loop() repeats continuously while the Arduino is running.
void loop() {
  // Read CLK now: the result is either HIGH or LOW.
  int currentCLK = digitalRead(CLK_PIN);

  // Detect a falling edge: CLK was HIGH and is now LOW.
  // Checking this transition prevents repeated adjustments
  // while CLK stays LOW.
  if (lastCLK == HIGH && currentCLK == LOW) {

    // Read DT at the moment CLK falls to determine the direction.
    // In this Wokwi wiring, DT HIGH means clockwise rotation.
    if (digitalRead(DT_PIN) == HIGH) {
      // Add 15 to the stored brightness.
      // This is shorthand for: brightness = brightness + 15;
      brightness += 15;
    } else {
      // DT LOW means counterclockwise rotation.
      // Subtract 15 to make the LED dimmer.
      brightness -= 15;
    }

    // Keep brightness within the valid PWM range.
    // Values below 0 become 0; values above 255 become 255.
    brightness = constrain(brightness, 0, 255);

    // Apply the updated brightness to the LED.
    analogWrite(LED_PIN, brightness); 
  }

  // Save this reading for the next pass through loop().
  // Next time, we compare the new reading with this saved value.
  lastCLK = currentCLK;
}