
#include <Servo.h>

// Ultrasonic Sensor Pins
const int trigPin1 = 2;
const int echoPin1 = 3;

const int trigPin2 = 4;
const int echoPin2 = 5;

// Servo Motor Pins
const int servoPin1 = 9;
const int servoPin2 = 10;

// Buzzer Pin
const int buzzerPin = 8;

// Detection distance in centimeters
const int detectionDistance = 20;

// Create Servo objects
Servo gateServo1;
Servo gateServo2;

// Servo positions (adjust according to gate mounting)
const int gate1Open = 0;
const int gate1Closed = 90;

const int gate2Open = 90;
const int gate2Closed = 0;

// System states
bool gateClosed = false;

// Function to measure distance
long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    return -1;
  }

  long distance = duration * 0.034 / 2;

  return distance;
}

// Function to check train detection
bool trainDetected(int trigPin, int echoPin) {
  long distance = getDistance(trigPin, echoPin);

  if (distance > 0 && distance <= detectionDistance) {
    return true;
  }

  return false;
}

// Function to close railway gate
void closeGate() {
  Serial.println("Train detected!");
  Serial.println("Closing Railway Gate");

  tone(buzzerPin, 1000, 500);

  gateServo1.write(gate1Closed);
  gateServo2.write(gate2Closed);

  delay(1000);

  gateClosed = true;

  Serial.println("Railway Gate Closed");
}

// Function to open railway gate
void openGate() {
  Serial.println("Train crossed the railway gate!");
  Serial.println("Opening Railway Gate");

  tone(buzzerPin, 1500, 500);

  gateServo1.write(gate1Open);
  gateServo2.write(gate2Open);

  delay(1000);

  gateClosed = false;

  Serial.println("Railway Gate Open");
}

void setup() {
  Serial.begin(9600);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(buzzerPin, OUTPUT);

  gateServo1.attach(servoPin1);
  gateServo2.attach(servoPin2);

  // Initially keep the gates open
  gateServo1.write(gate1Open);
  gateServo2.write(gate2Open);

  Serial.println("Automatic Railway Gate System Started");
}

void loop() {

  // Detect approaching train using first sensor
  if (!gateClosed) {

    if (trainDetected(trigPin1, echoPin1)) {
      closeGate();
    }

  }

  // Detect train after crossing using second sensor
  else {

    if (trainDetected(trigPin2, echoPin2)) {
      openGate();
    }

  }

  delay(100);
}
