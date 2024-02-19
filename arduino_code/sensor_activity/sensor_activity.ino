//r
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Arduino.h>
#include <EEPROM.h> // For storing calibration values
// Initialize the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD I2C address (0x27 for a common address)

#define DHT_PIN 4
#define DHT_TYPE DHT11
DHT dht(DHT_PIN,DHT_TYPE);

//declaring constant for utrasonic
const int TRIG_PIN  = 7;  // Arduino pin connected to Ultrasonic Sensor's TRIG pin
const int ECHO_PIN  = 6;  // Arduino pin connected to Ultrasonic Sensor's ECHO pin
// Define the analog pin to which the pH sensor is connected
const int analogPin = A0;
//const int RELAY_PIN = A5; // Arduino pin connected to Relay's pin
const int DISTANCE_THRESHOLD = 15; // centimeters
// Define variables for pH calibration
float pH7CalibrationValue = 1100.0; // Adjust this value based on your sensor and calibration results
float pH4CalibrationValue = 1600.0; // Adjust this value based on your sensor and calibration results

// Define a variable for storing the pH value
float pHValue = 0.0;

//declaring a float 
float duration,distance_cm;
float readtemp,Dht,readhumid; 
void setup() 
{
    // Initialize the LCD
  lcd.init();
  lcd.backlight();
  
  dht.begin();
  Serial.begin (9600);        // initialize serial port
  pinMode(TRIG_PIN, OUTPUT);  // set arduino pin to output mode
  pinMode(ECHO_PIN, INPUT);   // set arduino pin to input mode
}
void loop() 
{

 
    // Read the analog value from the pH sensor
  int rawValue = analogRead(analogPin);

  // Convert the raw value to pH using calibration values
  pHValue = map(rawValue, pH7CalibrationValue, pH4CalibrationValue, 7.0, 4.0);

  // Print the pH value to the serial monitor
  Serial.print("The PH value is ");
  Serial.println(pHValue, 2); // Print with 2 decimal places

  // Optional: Display pH value on LCD
  lcd.setCursor(0, 1);
  lcd.println("Roborift club");
  lcd.clear();
  lcd.println("pH:");
  lcd.println(pHValue, 1); // Print with 2 decimal places
 readDTH11_Sesnor();
  delay(1000); // Delay for 1 second before taking the next reading
  // generate 10-microsecond pulse to TRIG pin
    digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  // calculate the distance
  distance_cm = 0.017 * duration;
    // print the value to Serial Monitor
    Serial.println("Roborift club presents ");
    delay(500);
    Serial.println("Hydroponic Farming system");
    delay(500);
  Serial.print("The water level is ");
   delay(500);
  Serial.print("");
   delay(500);
  Serial.print(distance_cm);
   delay(500);
  Serial.println(" cm   ");
    Serial.print(" The Temperature is:");
    Serial.print("");
    Serial.print(readtemp);
    Serial.println(" °C");
    Serial.print(" The Humidity level is:");
    Serial.print("");
    Serial.print(readhumid);
    Serial.println(" h");
      lcd.setCursor(12, 0); // Position the cursor for temperature
      lcd.println("Temperature is:");
  lcd.println(readtemp);
  lcd.println(" °C");

 // lcd.setCursor(10, 1); // Position the cursor for humidity
   lcd.println("Humidity is:");
   // lcd.print("Water lavel is:");
  lcd.println(readhumid);
  lcd.print(" %");
lcd.println("Water lavel is:");
lcd.println(distance_cm);
  delay(2000); // Update every 2 seconds
    return;
 }
void readDTH11_Sesnor()
{

  // Reading temperature or humidity takes about 250 milliseconds!
//  humudity_value = dht.readHumidity();
  // Read temperature as Celsius (the default)
  readtemp = dht.readTemperature();
  readhumid=dht.readHumidity();
  // Check if any reads failed and exit early (to try again).
  if (isnan(readtemp)|| isnan(readhumid)) {
    Serial.println(("Failed to read from DHT sensor!"));
      lcd.setCursor(0, 0);
    lcd.print("DHT11 Error");
    lcd.setCursor(0, 1);
    lcd.print("Reading Failed");
    delay(4000);
    return;
 }
}
  // Function to calibrate the pH sensor
void calibratepH()
{
  // Calibrate at pH 7.00 buffer solution
  Serial.println("Place the pH sensor in pH 7.00 buffer solution.");
  delay(2000); // Wait for the sensor to stabilize
  int rawValue7 = analogRead(analogPin);
  pH7CalibrationValue = rawValue7;

  // Calibrate at pH 4.00 buffer solution
  Serial.println("Place the pH sensor in pH 4.00 buffer solution.");
  delay(2000); // Wait for the sensor to stabilize
  int rawValue4 = analogRead(analogPin);
  pH4CalibrationValue = rawValue4;

  // Store calibration values in EEPROM (optional)
  EEPROM.put(0, pH7CalibrationValue);
  EEPROM.put(4, pH4CalibrationValue);
}
