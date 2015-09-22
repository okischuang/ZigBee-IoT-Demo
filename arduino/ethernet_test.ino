#include <SPI.h>
#include <Ethernet.h>

// 指定網路卡 MAC 卡號
byte mac[] = {  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
//IPAddress server(10,1,224,2);  // numeric IP for Google (no DNS)
char server[] = "10.1.224.2";    // name address for Google (using DNS)

// 初始化 Ethernet client library
EthernetClient client;
float temp;

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
  if (client.connect(server, 3000)) {
    Serial.println("connected");
    
   
  }
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }

  
}

void loop() {
  if(Serial.available() >= 21) {
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
      Serial.println("degrees C received from XBee");
      // Make a HTTP request:

      client.println("POST /iot HTTP/1.1");
      client.println("Host: 10.1.224.2:3000");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      String data = "temp=" + String(temp);
      client.print(data.length());
      client.print("\n\n");
      client.print(data);

      
    }
  }
}
