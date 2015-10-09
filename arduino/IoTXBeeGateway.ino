#include <SPI.h>
#include <Ethernet.h>


// 指定網路卡 MAC 卡號
byte mac[] = {  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
//IPAddress server(10,1,224,2);  // numeric IP for Google (no DNS)
char server[] = "YOUR_IP";    // name address for Google (using DNS)

// 初始化 Ethernet client library
EthernetClient client;
float temp;
const float voltage = 5.0;
boolean isConnected = false;
void setup() {
  // 初始化序列埠
  Serial.begin(9600);
  // 啟用 Ethernet 連線，預設會以 DHCP 取得 IP 位址
  if (Ethernet.begin(mac) == 0) {
    Serial.println("無法取得 IP 位址");
    // 無法取得 IP 位址，不做任何事情
    for(;;)
      ;
  }
  // 輸出 IP 位址
  Serial.print("IP 位址：");
  Serial.println(Ethernet.localIP());
  // if you get a connection, report back via serial:
  if (client.connect(server, 3333)) {
    Serial.println("Connected to IoT server!");
    isConnected = true;

  }
  else {
    // kf you didn't get a connection to the server:
    Serial.println("Connection failed");
  }


}

void loop() {
  if(!isConnected){
    if (client.connect(server, 3333)) {
      Serial.println("Connected to IoT server!");
      isConnected = true;
    }
  }
  if(Serial.available() >= 21) {
    //Serial.println("length:" + String(Serial.available()));
    if(Serial.read() == 0x7E) {
      for(int i = 1; i < 19; i++) {
        byte discardByte = Serial.read();
        //Serial.println("i=" + String(i) + "byte: " + discardByte);
      }
      // read a Byte
      int analogMSB = Serial.read();
      // read a Byte
      int analogLSB = Serial.read();
      int analogReading = analogLSB + (analogMSB * 256);
      //Serial.println("MSB:" + String(analogMSB));
      //Serial.println("LSB:" + String(analogLSB));
      //Serial.println("Reading Analog Value:" + String(analogReading));
      //temp = analogReading / 1023.0 * 1.23;
      temp = (voltage/1024.0) * analogReading;
      //Serial.println("Get "+String(temp)+" V");
      temp = temp * 100;
      //Serial.println("Convert to mV: " + String(temp));
      temp = temp / 10;
      Serial.println("Receiving data");
      Serial.println("Temp: " + String(temp) + " °C");

      // Make a HTTP request:
      Serial.println("Sending data to IoT server...");
      client.println("POST /iot HTTP/1.1");
      client.println("Host: *.*.*.*:3333");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      String data = "temp=" + String(temp);
      client.print(data.length());
      client.print("\n\n");
      client.print(data);


    }
  }
}
