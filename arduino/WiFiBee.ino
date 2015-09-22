#include <SoftwareSerial.h>

#define DEBUG true

SoftwareSerial esp8266(2,3);
float temp;
int readyInt = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  esp8266.begin(57600);

  sendData("AT+RST\r\n",2000,DEBUG);
  delay(2000);
  sendData("AT+CWMODE=3\r\n",1000,DEBUG);
  delay(2000);
  sendData("AT+CWJAP=\"piggyhome\",\"qaz12345\"\r\n",3000,DEBUG);
  delay(2000);
}

void loop() {

  // put your main code here, to run repeatedly:
  if(esp8266.available()) {
    if(readyInt <= 0) {
      sendData("AT+CIFSR\r\n",2000,DEBUG);
      Serial.println("esp8266 is ready");
      readyInt = readyInt + 1;
    }
     int temp = 0;
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
      temp = temp * 5/9 + 32;
      Serial.print(temp);
      Serial.println("degrees C");
      
      
      String postData = "POST /iot HTTP/1.1\r\nHost: 192.168.1.103:3000\r\nContent-Type: application/x-www-form-urlencoded\r\nCon­tent-Length: 20\r\n\r\ntemp=" + String(temp) + "\r\n\r­\n";
      boolean ok = sendPostData(postData);
      if(ok){
        Serial.println("Report successfully!");
      }
      else{
        Serial.println("Failed!");
      }
      
    }
  }
  }
 
}

boolean sendPostData(String data) {
  String response = "";
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG);
  delay(2000);
  sendData("AT+CIPSTART=4,\"TCP\",\"192.168.1.103\",3000",2000,DEBUG);
  delay(2000);
  String sendCmd = "AT+CIPSEND=4," + data.length();
  sendData(sendCmd,1000,DEBUG);
  delay(2000);
  if (esp8266.find(">")) {
    Serial.println("Sending packet...");
    sendData(data,3000,DEBUG);
    delay(2000);
    while (esp8266.available()) {
      String tmpResp = esp8266.readString();
      Serial.println(tmpResp);
    }
    // close the connection
    sendData("AT+CIPCLOSE\r\n",1000,DEBUG);
    //return true;
  }
  
  
}

String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    esp8266.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}

boolean httpPOST(String data, String server, int port, String uri, String* response) {
    // initiate TCP connection
    String tcpStart = "AT+CIPSTART=\"TCP\",\"" + server + "\"," + port;
    Serial.println(tcpStart);
    // prepare the data to be posted
    String postRequest =
        "POST " + uri + " HTTP/1.1\r\n" +
        "Host: " + server + ":" + port + "\r\n" +
        "Accept: *" + "/" + "*\r\n" +
        "Content-Length: " + data.length() + "\r\n" +
        "Content-Type: application/x-www-form-urlencoded\r\n" +
        "\r\n" +
        data;

    // notify ESP8266 about the lenght of TCP packet
    String sendCmd = "AT+CIPSEND=" + postRequest.length();


    esp8266.println(tcpStart);
    if (esp8266.find("OK")) {
        Serial.println("TCP connection OK");
        esp8266.println(sendCmd);
        if (esp8266.find(">")) {
            Serial.println("Sending packet...");
            esp8266.println(postRequest);
            if (esp8266.find("SEND OK")) {
                Serial.println("Packet sent");
                while (esp8266.available()) {
                    String tmpResp = esp8266.readString();
                    response = &tmpResp;
                }
                // close the connection
                esp8266.println("AT+CIPCLOSE");
                return true;
            }
            else {
                Serial.println("An error occured while sending packet");
                return false;
            }
        }
        else {
            Serial.println("ESP8266 is not listening for incoming data");
            return false;
        }
    }
    else {
        Serial.println("Cannot initiate TCP connection");
        return false;
    }
}
