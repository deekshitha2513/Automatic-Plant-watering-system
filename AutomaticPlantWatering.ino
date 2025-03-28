/*
  Automatic Plant Watering System with Rainwater Harvesting
  ----------------------------------------------------------
  Description:
  This sketch reads a soil moisture sensor (connected to A0) and a rainwater level sensor 
  (connected to A1). When the soil moisture falls below a set threshold, the system checks for 
  sufficient harvested rainwater. If rainwater is available (sensor reading above the designated 
  threshold), it activates a solenoid valve to water the plants using the rainwater. If not, it 
  defaults to the main water supply via a separate solenoid valve.
  
  Hardware Requirements:
    - Arduino Board (e.g., Uno)
    - Soil Moisture Sensor  (Analog pin A0)
    - Rainwater Level Sensor (Analog pin A1)
    - Solenoid Valve for Main Water Supply (Digital pin 7)
    - Solenoid Valve for Rainwater Supply  (Digital pin 8)
  
  Note: 
    - Adjust the threshold values based on your sensor calibrations.
    - Ensure that your valve circuits and sensors are correctly interfaced with the Arduino.
*/

const int soilMoisturePin = A0;      // Soil moisture sensor pin
const int rainwaterLevelPin = A1;    // Rainwater level sensor pin

const int mainValvePin = 7;          // Control pin for the main water supply solenoid valve
const int rainwaterValvePin = 8;     // Control pin for the rainwater solenoid valve

// Threshold values:
// soilMoistureThreshold: Below this value, the soil is considered too dry.
const int soilMoistureThreshold = 500;      

// rainwaterLevelThreshold: Above this value, there is sufficient rainwater stored.
const int rainwaterLevelThreshold = 300;      

// Duration for which the system irrigates (in milliseconds)
const unsigned long irrigationDuration = 5000;  // 5 seconds

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);
  
  // Initialize sensor pins as inputs
  pinMode(soilMoisturePin, INPUT);
  pinMode(rainwaterLevelPin, INPUT);
  
  // Initialize valve control pins as outputs
  pinMode(mainValvePin, OUTPUT);
  pinMode(rainwaterValvePin, OUTPUT);
  
  // Ensure both valves are off at startup
  digitalWrite(mainValvePin, LOW);
  digitalWrite(rainwaterValvePin, LOW);
  
  Serial.println("Automatic Plant Watering System Initialized");
}

void loop() {
  // Read sensor values
  int soilMoistureValue = analogRead(soilMoisturePin);
  int rainwaterLevelValue = analogRead(rainwaterLevelPin);
  
  // Log the readings for monitoring
  Serial.print("Soil Moisture Value: ");
  Serial.println(soilMoistureValue);
  Serial.print("Rainwater Level Value: ");
  Serial.println(rainwaterLevelValue);
  
  // Check if the soil is too dry
  if (soilMoistureValue < soilMoistureThreshold) {
    Serial.println("Soil is dry. Starting irrigation procedure.");
    
    // Determine water source based on stored rainwater availability:
    if (rainwaterLevelValue > rainwaterLevelThreshold) {
      Serial.println("Sufficient harvested rainwater available. Activating rainwater valve.");
      // Activate the rainwater solenoid valve and ensure the main valve is off
      digitalWrite(rainwaterValvePin, HIGH);
      digitalWrite(mainValvePin, LOW);
    } else {
      Serial.println("Insufficient rainwater. Falling back on main water supply.");
      // Activate the main water supply solenoid valve and ensure the rainwater valve is off
      digitalWrite(mainValvePin, HIGH);
      digitalWrite(rainwaterValvePin, LOW);
    }
    
    // Allow irrigation to run for the specified duration
    delay(irrigationDuration);
    
    // Turn both valves off after irrigation
    digitalWrite(mainValvePin, LOW);
    digitalWrite(rainwaterValvePin, LOW);
    Serial.println("Irrigation complete.");
  } 
  else {
    // No irrigation needed because the soil moisture level is sufficient
    Serial.println("Soil moisture is adequate. No watering required.");
    digitalWrite(mainValvePin, LOW);
    digitalWrite(rainwaterValvePin, LOW);
  }
  
  // Delay before the next reading cycle (adjust this interval as needed)
  delay(10000);  // 10 seconds delay
}
