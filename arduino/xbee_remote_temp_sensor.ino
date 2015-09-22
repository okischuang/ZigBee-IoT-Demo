float temp;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() >= 21) {
    // 0x7E is a frame start delimiter field which is always 0x7E
    if(Serial.read() == 0x7E) {
      for(int i = 1; i < 19; i++) {
        byte discardByte = Serial.read();
      }
      int analogMSB = Serial.read();
      int analogLSB = Serial.read();
      int analogReading = analogLSB + (analogMSB * 256);
      temp = analogReading / 1023.0 * 1.23;
      temp = temp - 0.5;
      temp = temp / 0.01;
      temp = temp * 9/5 + 32;
      Serial.print(temp);
      Serial.println("degrees C");
    }
  }
}
