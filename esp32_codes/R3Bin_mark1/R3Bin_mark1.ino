//Error 100- Failed to start Serial2 with R pi 
//Error 101- Failed UltraSonic Sensor

#include <AccelStepper.h>
#include <MultiStepper.h>

#include <Wire.h>
#include <Adafruit_VL53L0X.h>

#define TRIG_PIN 34
#define ECHO_PIN 35

#define SDA_PIN 21
#define SCL_PIN 22
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

const float THRESHOLD_CM = 25.0;

#define ESP32_RX2 16
#define ESP32_TX2 17

#define PUL_PIN 15
#define DIR_PIN 2

#define PUL_PIN2 23  
#define DIR_PIN2 19

#define delay_ms 50
#define delay_ms2 200
AccelStepper motor1(AccelStepper::DRIVER, PUL_PIN, DIR_PIN);
AccelStepper motor2(AccelStepper::DRIVER, PUL_PIN2, DIR_PIN2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Welcome to R3Bin");
  delay(1000);
  Serial.println("Bin Setting Up....Please Wait");
  delay(1000);
  Serial2.begin(115200, SERIAL_8N1, ESP32_RX2, ESP32_TX2);
 
  delay(5000);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(PUL_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  pinMode(PUL_PIN2, OUTPUT);
  pinMode(DIR_PIN2, OUTPUT);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!lox.begin()) {
    Serial.println("Failed to boot VL53L0X. Check wiring.");
    while (1);
  }
  Serial.println("VL53L0X ready");

  motor1.setMaxSpeed(1000);
  motor1.setAcceleration(500);

  motor2.setMaxSpeed(1000);
  motor2.setAcceleration(500);
  //checkComponents();
  Serial.println("Bin Ready To Use");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(lidar()==true){
    Serial.println("object detected");
    Serial2.println(true);
    //delay(500);
    while(Serial2.available()){
      int received = Serial2.parseInt();      //received=received-48;
      int a=convert(received);
      delay(30);
      Serial.print("We received: ");Serial.print(received);Serial.print("||");Serial.print(a);Serial.println();
      differential_control(a);
    }
  }
  else{
    Serial2.println(false);
  }
}

int convert(int a){
  int val;
  if(a>10){
    val=a%10;
  }
  else{
    val=a;
  }
  return val;
}

bool ultrasonic(){
  bool flag;
  
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = (duration / 2.0) / 29.1;
  Serial.println(distance);
  return distance<THRESHOLD_CM?true:false;
 }

bool lidar(){
  bool flag;
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {
    Serial.print("Distance (mm): ");
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println("Out of range");
  }
  if(measure.RangeMilliMeter<250){
    flag=true;
  }
  else{
    flag=false;
  }

  return flag;
}

void differential_control(int value){
  if(value==1){
    Serial.println("Bin 1");
  moveMotor1(150);
  delay(delay_ms);

  moveMotor1(-150);
  delay(delay_ms2);
  }

  else if(value==2){
        Serial.println("Bin 2");
  moveBoth(300);
  delay(delay_ms);

  moveMotor1(150);
  delay(delay_ms);

  moveMotor1(-150);
  delay(delay_ms);

  moveBoth(-300);
  delay(delay_ms2);
  }

  else if(value==3){
        Serial.println("Bin 3");
  moveMotor1(-150);
  delay(delay_ms);

  moveMotor1(150);
  delay(delay_ms2);
  }

  else {
        Serial.println("Bin 4");
  moveBoth(300);
  delay(delay_ms);

  moveMotor1(-150);
  delay(delay_ms);

  moveMotor1(150);
  delay(delay_ms);

  moveBoth(-300);
  delay(delay_ms2);
  }
}

void moveMotor1(int steps) {
  motor1.move(steps);
  while (motor1.distanceToGo() != 0) {
    motor1.run();
  }
}

void moveMotor2(int steps) {
  motor2.move(steps);
  while (motor2.distanceToGo() != 0) {
    motor2.run();
  }
}

void moveBoth(int steps) {
  motor1.move(steps);
  motor2.move(steps);

  while (motor1.distanceToGo() != 0 || motor2.distanceToGo() != 0) {
    motor1.run();
    motor2.run();
  }
}

bool checkComponents(){
  /*if(!Serial2.begin(115200)){
    Serial.println("Error 100");
  }*/

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); 

  if(duration==0){
    Serial.println("Error 101");
  }

}
