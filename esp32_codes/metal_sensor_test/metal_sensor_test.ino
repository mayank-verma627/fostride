#define pin 27 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("code starting");
  pinMode(pin, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(pin));
}
