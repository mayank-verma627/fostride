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

#define PUL_PIN 15  // y control for the bin
#define DIR_PIN 2

#define PUL_PIN2 23  // z control for the bin
#define DIR_PIN2 19

#define delay_ms 300
#define delay_ms2 300

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
  Serial.println("Bin Ready To Use");
}

void loop() {
  //moveBoth(HIGH, 300);
  test();
  if(lidar()==true){
    Serial.println("object detected");
    Serial2.println(true);
    while(Serial2.available()){
      int received = Serial2.parseInt();      
      int a=convert(received);
      delay(30);
      Serial.print("We received: ");Serial.print(received);Serial.print("||");Serial.print(a);Serial.println();
      //differential_control(a);
    }
  }
  else{
    Serial2.println(false);
  }
}

void test(){
  differential_control(1);
  delay(2000);
  differential_control(2);
  delay(2000);
  differential_control(3);
  delay(2000);
  differential_control(4);
  delay(2000);
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
  moveSteps(PUL_PIN, DIR_PIN, HIGH, 150);
  delay(1500);
  moveSteps(PUL_PIN, DIR_PIN, LOW, 150);
  }

  else if(value==2){
  Serial.println("Bin 2");
  moveBoth(HIGH, 300);
  moveSteps(PUL_PIN, DIR_PIN, HIGH, 150);
  moveSteps(PUL_PIN, DIR_PIN, LOW, 150);
  moveBoth(LOW, 300);
  }

  else if(value==3){
  Serial.println("Bin 3");
  moveSteps(PUL_PIN, DIR_PIN, LOW, 150);
  moveSteps(PUL_PIN, DIR_PIN, HIGH, 150);
  }

  else {
  Serial.println("Bin 4");
  moveBoth(HIGH, 300);
  moveSteps(PUL_PIN, DIR_PIN, LOW, 150);
  moveSteps(PUL_PIN, DIR_PIN, HIGH, 150);
  moveBoth(LOW, 300);
  }
}



void moveSteps(int a, int b,bool c, int steps) {
  digitalWrite(b, c);
  for (int i = 0; i < steps; i++) {
    digitalWrite(a, HIGH);
    delayMicroseconds(500);
    digitalWrite(b, LOW);
    delayMicroseconds(500);
  }
}


void moveBoth(bool a, int steps){
  digitalWrite(DIR_PIN, a);
  digitalWrite(DIR_PIN2, a);
  for (int i = 0; i < steps; i++) {
    digitalWrite(PUL_PIN, HIGH);
    digitalWrite(PUL_PIN2, HIGH);
    delayMicroseconds(500);
    digitalWrite(PUL_PIN, LOW);
    digitalWrite(PUL_PIN2, LOW);
  }
  delay(1000);
}
