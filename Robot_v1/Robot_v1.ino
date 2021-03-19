// This is the sketch for my robot in IT 

void setup() {
  // Set up the serial monitor
  Serial.begin(9600);

  // Initialise the ultrasonic sensor
  distanceSetup();
}

void loop() {
  // The main code for the robot

  // Testing code for ultrasonic sensor
  delay(100);

  Serial.print("Ping: ");
  Serial.print(getDistance());
  Serial.println("cm");
}
