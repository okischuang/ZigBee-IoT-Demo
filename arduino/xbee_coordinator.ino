void setup() {
  // coordinator
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() >0) {
    Serial.write(Serial.read());
  }
  Serial.println("I'm C!");
  delay(3000);
}
