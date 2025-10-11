#define X_STEP_PIN 5
#define X_DIR_PIN 6

#define Y_STEP_PIN 2
#define Y_DIR_PIN 3

void setup() {
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);

  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
}

void rotateBoth(int steps, bool direction) {
  digitalWrite(X_DIR_PIN, direction);
  digitalWrite(Y_DIR_PIN, direction);
  
  for (int i = 0; i < steps; i++) {
    digitalWrite(X_STEP_PIN, HIGH);
    digitalWrite(Y_STEP_PIN, HIGH);
    delayMicroseconds(500);
    digitalWrite(X_STEP_PIN, LOW);
    digitalWrite(Y_STEP_PIN, LOW);
    delayMicroseconds(500);
  }
}

void rotateY(int steps, bool dir){
  digitalWrite(Y_DIR_PIN, dir);
  for (int i = 0; i < steps; i++) {
    digitalWrite(Y_STEP_PIN, HIGH);
    delayMicroseconds(1000);
    digitalWrite(Y_STEP_PIN, LOW);
    delayMicroseconds(1000);
  }
}

void loop() {

  //foward y control////////////////////////////
  rotateY(50, HIGH);     
  delay(500);  
  rotateY(50, LOW);     
  delay(1000);
  /////////////////////////////////////////////
  
  /////right forward///////////////////////////
  rotateBoth(180, HIGH);  
  delay(1000);
  /*rotateY(100, HIGH);     
  delay(500);   //right forward
  rotateY(100, LOW);     
  
  rotateBoth(180, LOW);   
  delay(1000);
  /////////////////////////////////////////////

  //backward y control////////////////////////////
  rotateY(100, LOW);     
  delay(500);  
  rotateY(100, HIGH);     
  delay(1000);
  ///////////////////////////////////////////////

  /////right backward///////////////////////////
  rotateBoth(180, HIGH);  
  
  rotateY(100, LOW);     
  delay(500);   //right backward
  rotateY(100, HIGH);     
  
  rotateBoth(180, LOW);   
  delay(1000);
  /////////////////////////////////////////////*/
  

 
}
