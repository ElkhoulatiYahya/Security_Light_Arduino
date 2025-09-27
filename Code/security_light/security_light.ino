// =============================
// Motion Light Control Project
// PIR + LDR + Relay
// =============================

// Relay pin is controlled with D8
// The active wire is connected to Normally Closed and Common
int relay = 8;
volatile byte relayState = LOW;

// PIR Motion Sensor is connected to D2
int PIRInterrupt = 2;

// LDR pin is connected to Analog 0
int LDRPin = A0;

// LDR value
int LDRReading;

// LDR Threshold value (adjust depending on your room lighting)
int LDRThreshold = 300;

// Timer Variables
long lastDebounceTime = 0;
long debounceDelay = 10000; // 10 seconds

// =============================
// Setup
// =============================
void setup() {
  // Pin for relay module set as output
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH); // relay OFF initially (active LOW)

  // PIR motion sensor set as input
  pinMode(PIRInterrupt, INPUT);

  // Attach interrupt to PIR sensor
  attachInterrupt(digitalPinToInterrupt(PIRInterrupt), detectMotion, RISING);

  // Serial communication for debugging
  Serial.begin(9600);
}

// =============================
// Main Loop
// =============================
void loop() {
  // If 10 seconds have passed, turn relay OFF
  if ((millis() - lastDebounceTime) > debounceDelay && relayState == HIGH) {
    digitalWrite(relay, HIGH); // relay OFF
    relayState = LOW;
    Serial.println("OFF");
  }

  delay(50);
}

// =============================
// Interrupt Function
// =============================
void detectMotion() {
  Serial.println("Motion Detected!");
  
  LDRReading = analogRead(LDRPin);
  // Debug: Uncomment if you want to see LDR values
  // Serial.println(LDRReading);

  // Only turn relay ON if the room is dark enough
  if (LDRReading > LDRThreshold) {
    if (relayState == LOW) {
      digitalWrite(relay, LOW); // relay ON
    }
    relayState = HIGH;
    Serial.println("ON");
    lastDebounceTime = millis();
  }
}
