#include <Servo.h>

const int moisture_pin = A0;
const int temp_pin = A1;
const int ldr_pin = A2;
const int servo_pin = 3;
const int led_red_pin = 11;
const int led_blue_pin = 10;
const int led_green_pin = 9;

// value which differentiates wet and dry soil
const int min_moisture = 420;

// servo valve
Servo servo_valve;

// angle at which valve is opened
const int servo_open = 0;
// angle at which valve is closed
const int servo_close = 90;

// 0 => close
// 1 => open
int servo_status = 0;

// set values for ideal temperatures for different times
const int day_ideal_temp_lower = 21;
const int day_ideal_temp_upper = 27;

const int night_ideal_temp_lower = 15;
const int night_ideal_temp_upper = 20;

// current ideal temperature
int ideal_temp_lower = 0;
int ideal_temp_upper = 1;

// value which differentiates day and night
const int twilight = 530;

void setup()
{
  Serial.begin(9600);
  pinMode(moisture_pin, INPUT);
  servo_valve.attach(servo_pin);
  pinMode(temp_pin, INPUT);
  pinMode(ldr_pin, INPUT);
  pinMode(led_red_pin, OUTPUT);
  pinMode(led_blue_pin, OUTPUT);
  pinMode(led_green_pin, OUTPUT);
}

void loop()
{
  const int moisture = analogRead(moisture_pin);
  const int light = analogRead(ldr_pin);

  // TMP36 sensor specific logic
  const float voltage = (analogRead(temp_pin) * 5.0) / 1024.0;
  const float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset

  Serial.println("=====");
  Serial.print("Moisture: ");
  Serial.println(moisture);
  Serial.print("Light: ");
  Serial.println(light);
  Serial.print("Temperature (C): ");
  Serial.println(temperatureC);

  // soil moisture
  Serial.print("Valve Servo: ");
  if (moisture < min_moisture) {
    Serial.println("Open");
    // avoid sending unnecesary commands to servo
    if (servo_status == 0) {
      servo_status = 1;
      servo_valve.write(servo_open);
    }
  } else {
    Serial.println("Close");
    // avoid sending unnecesary commands to servo
    if (servo_status == 1) {
      servo_status = 0;
      servo_valve.write(servo_close);
    }
  }
  
  // light
  Serial.print("Time of day: ");
  if (light < twilight) {
    Serial.println("Night");
    ideal_temp_lower = night_ideal_temp_lower;
    ideal_temp_upper = night_ideal_temp_upper;
  } else {
    Serial.println("Day");
    ideal_temp_lower = day_ideal_temp_lower;
    ideal_temp_upper = day_ideal_temp_upper;
  }

  // temperature
  Serial.print("RGB LED: ");
  if (temperatureC < ideal_temp_lower) {
    Serial.println("Blue");
    digitalWrite(led_red_pin, LOW);
    digitalWrite(led_blue_pin, HIGH);
    digitalWrite(led_green_pin, LOW);
  } else if (temperatureC >= ideal_temp_lower && temperatureC < ideal_temp_upper) {
    Serial.println("Green");
    digitalWrite(led_red_pin, LOW);
    digitalWrite(led_blue_pin, LOW);
    digitalWrite(led_green_pin, HIGH);
  } else {
    Serial.println("Red");
    digitalWrite(led_red_pin, HIGH);
    digitalWrite(led_blue_pin, LOW);
    digitalWrite(led_green_pin, LOW);
  }
  
  delay(1000);
}