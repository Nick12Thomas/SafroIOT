#include "DHT.h"
#include <Servo.h>
#include <NewPing.h>//Ultrasonic sensor function library. You must install this library
// #include <stdint.h>
// #include <avr/io.h>
// #include <util/twi.h>
// #include <util/delay.h>
// #include <avr/pgmspace.h>
#include <SoftwareSerial.h>
#define maximum_distance 200
#define echoPin 8 // attach pin D8 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
#define BLYNK_PRINT Serial
#define DHTPIN 2 
#define DHTTYPE DHT11 
#define servoPin 9
Servo servo_motor; 
NewPing sonar(trigPin, echoPin, maximum_distance);
SoftwareSerial BTserial(10, 11); //RX, TX
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
//Servo myservo;// Create a new servo object:
int angle = 0;// Create a variable to store the servo position:
//Driver Pins
//  int m1a = 4;
//  int m1b = 5;
//  int m2a = 6;
//  int m2b = 7;
// char val;
//our L298N control pins
boolean goesForward = false;
const int LeftMotorForward = 7;
const int LeftMotorBackward = 6;
const int RightMotorForward = 4;
const int RightMotorBackward = 5;

DHT dht(DHTPIN, DHTTYPE);

const int buzzer = 13;
int smokeA0 = A5;
int sensorPin = A0;
int sensorValue;  
int limit = 300;
int sensorThres = 200;// Your threshold value for gas sensor

void setup() {
  //  pinMode(m1a, OUTPUT);  // Digital pin 4 set as output Pin
  //  pinMode(m1b, OUTPUT);  // Digital pin 5 set as output Pin
  //  pinMode(m2a, OUTPUT);  // Digital pin 6 set as output Pin
  //  pinMode(m2b, OUTPUT);  // Digital pin 7 set as output Pin
   pinMode(RightMotorForward, OUTPUT);
   pinMode(LeftMotorForward, OUTPUT);
   pinMode(LeftMotorBackward, OUTPUT);
   pinMode(RightMotorBackward, OUTPUT);    
   pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
   pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
   pinMode(buzzer, OUTPUT);//Buzzer output
   pinMode(smokeA0, INPUT); //Gas Sensor Input
   pinMode(13, OUTPUT);
   //myservo.attach(servoPin);
   dht.begin();
   BTserial.begin(9600);
   Serial.begin(9600);
}

void loop() {
  // digitalWrite(trigPin, LOW);
  // delayMicroseconds(2);
  // // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  // digitalWrite(trigPin, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(trigPin, LOW);
  // // Reads the echoPin, returns the sound wave travel time in microseconds
  // duration = pulseIn(echoPin, HIGH);
  // // Calculating the distance
  // distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // // Displays the distance on the Serial Monitor
  // Serial.print("Distance: ");
  // Serial.print(distance);
  // Serial.println(" cm");
  float moist_per;
  sensorValue = analogRead(sensorPin); 
  moist_per=(100-((sensorValue/1023.00)*100));
  Serial.println("Soil Moister : ");  
  Serial.print(moist_per); 
 if (sensorValue<limit) {
 digitalWrite(13, HIGH); 
 }
 else {
 digitalWrite(13, LOW); 
 }
  //delay(2000);
  float h = dht.readHumidity();  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
   if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F(" Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("C "));
  Serial.print(f);
  Serial.print(F("F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("C "));
  Serial.print(hif);
  Serial.println(F("F"));

  int analogSensor = analogRead(smokeA0);

  Serial.print("Gas Value: ");
  Serial.println(analogSensor);
  // BTserial.print(distance);  
  //               BTserial.print(",");  
                BTserial.print(analogSensor);  
                BTserial.print(",");  
                BTserial.print(moist_per);  
                BTserial.print(",");  
                BTserial.print(h);  
                BTserial.print(",");  
                BTserial.print(t);  
                BTserial.print(",");  
                BTserial.print(f);  
                BTserial.print(";");  
                delay(20);
  

  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);

  if (distance <= 25){
    moveStop();
    delay(100);
    moveBackward();
    delay(400);
    moveStop();
    delay(300);
    
    if (distance >= distanceLeft){
      turnRight();
      moveStop();
    }
    else{
      turnLeft();
      moveStop();
    }
  }
  else{
    moveForward(); 
  }
    distance = readPing();
}

int lookRight(){  
  servo_motor.write(50);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(115);
  return distance;
}

int lookLeft(){
  servo_motor.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(115);
  return distance;
  delay(100);
}

int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}

void moveStop(){
  
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
}

void moveForward(){

  if(!goesForward){

    goesForward=true;
    
    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(RightMotorForward, HIGH);
  
    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW); 
  }
}

void moveBackward(){

  goesForward=false;

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
}

void turnRight(){

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
  delay(900);
  
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
 
  
  
}

void turnLeft(){

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);

  delay(900);
  
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);

  //  while (Serial.available() > 0)
  // {
  // val = Serial.read();
  // Serial.println(val);
  // }

  // if( val == 'F') // Forward
  //   {
  //     digitalWrite(m1a, HIGH);
  //     digitalWrite(m1b, LOW);
  //     digitalWrite(m2a, HIGH);
  //     digitalWrite(m2b, LOW);  
  //   }
  // else if(val == 'B') // Backward
  //   {
  //     digitalWrite(m1a, LOW);
  //     digitalWrite(m1b, HIGH);
  //     digitalWrite(m2a, LOW);
  //     digitalWrite(m2b, HIGH); 
  //   }
  
  //   else if(val == 'L') //Left
  //   {
  //   digitalWrite(m1a, LOW);
  //   digitalWrite(m1b, LOW);
  //   digitalWrite(m2a, HIGH);
  //   digitalWrite(m2b, LOW);
  //   }
  //   else if(val == 'R') //Right
  //   {
  //   digitalWrite(m1a, HIGH);
  //   digitalWrite(m1b, LOW);
  //   digitalWrite(m2a, LOW);
  //   digitalWrite(m2b, LOW); 
  //   }
    
  // else if(val == 'S') //Stop
  //   {
  //   digitalWrite(m1a, LOW);
  //   digitalWrite(m1b, LOW);
  //   digitalWrite(m2a, LOW);
  //   digitalWrite(m2b, LOW); 
  //   }
  // else if(val == 'I') //Forward Right
  //   {
  //   digitalWrite(m1a, HIGH);
  //   digitalWrite(m1b, LOW);
  //   digitalWrite(m2a, LOW);
  //   digitalWrite(m2b, LOW);
  //   }
  // else if(val == 'J') //Backward Right
  //   {
  //   digitalWrite(m1a, LOW);
  //   digitalWrite(m1b, HIGH);
  //   digitalWrite(m2a, LOW);
  //   digitalWrite(m2b, LOW);
  //   }
  //  else if(val == 'G') //Forward Left
  //   {
  //   digitalWrite(m1a, LOW);
  //   digitalWrite(m1b, LOW);
  //   digitalWrite(m2a, HIGH);
  //   digitalWrite(m2b, LOW);
  //   }
  // else if(val == 'H') //Backward Left
  //   {
  //   digitalWrite(m1a, LOW);
  //   digitalWrite(m1b, LOW);
  //   digitalWrite(m2a, LOW);
  //   digitalWrite(m2b, HIGH); 
  //   }
  // Clears the trigPin condition
  // Checks if it has reached the threshold value
  // if (analogSensor > sensorThres)
  // {
  //   tone(buzzer, 1000, 200);
  // }
  // else
  // {
  //   noTone(buzzer);
  // }
   //Tell the servo to go to a particular angle:
  // myservo.write(90);
  // delay(1000);
  // myservo.write(180);
  // delay(1000);
  // myservo.write(0);
  // delay(1000);

  // //Sweep from 0 to 180 degrees:
  // for (angle = 0; angle <= 180; angle += 1) {
  //   myservo.write(angle);
  //   delay(15);
  // }

  // // And back from 180 to 0 degrees:
  // for (angle = 180; angle >= 0; angle -= 1) {
  //   myservo.write(angle);
  //   delay(30);
  // }
  //delay(1000);
                
}
