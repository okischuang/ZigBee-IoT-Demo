# ZigBee-IoT-Demo
It's a simple demo project for using xbee with arduino to make a IoT device

# Steps

1. Using X-CTU software to set up XBee

  configure coordinator and router with same PAN ID
  Set coordinator as API mode, router as AT mode
  Set Channel Verification to 1 [Enabled] for router
  Set D3 in I/O setting as ADC for analog input for router
  Set I/O sampling Rate as 1388 in hex (5000ms) in I/O sampling setting for router

2. Writing Arduino code and upload to coordinator and router. github

3. Following my Wiring layout and put things together

4. Connect to internet for your Ethernet shield

5. node server.js

6. Open DHT data monitoring page
