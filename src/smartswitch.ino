#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

WebServer server(80);
Servo servoOn;  
Servo servoOff; 

String htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Control</title>
  <style>
    body {
      text-align: center;
      background-color: #0d0d0d;
      margin: 0;
      padding: 20px;
      color: white;
    }
    h1 { 
      color: #fff; 
      font-size: 2em; 
      margin-bottom: 30px;
    }
    .button-container {
      display: flex;
      justify-content: center;
      flex-wrap: wrap;
      gap: 40px;
    }
    button {
      font-size: 28px;
      width: 180px;
      height: 180px;
      border: none;
      border-radius: 50%;
      cursor: pointer;
      transition: transform 0.2s ease, box-shadow 0.2s ease;
      background: linear-gradient(135deg, #4CAF50, #2E8B57);
      color: white;
      position: relative;
      overflow: hidden;
      display: flex;
      align-items: center;
      justify-content: center;
    }
    button.off {
      background: linear-gradient(135deg, #f44336, #8B0000);
    }
    @media (hover:hover) {
      button:hover {
        transform: scale(1.08);
        box-shadow: 0 0 25px rgba(255, 255, 255, 0.4);
      }
    }
    button:active::after {
      content: "";
      position: absolute;
      top: 50%;
      left: 50%;
      width: 300%;
      height: 300%;
      background: radial-gradient(circle, rgba(138,43,226,0.6) 0%, rgba(25,25,112,0.4) 40%, transparent 80%);
      transform: translate(-50%, -50%);
      animation: galaxy 0.6s ease-out;
      border-radius: 50%;
      pointer-events: none;
    }
    @keyframes galaxy {
      from { opacity: 1; transform: translate(-50%, -50%) scale(0.2); }
      to { opacity: 0; transform: translate(-50%, -50%) scale(1); }
    }
  </style>
</head>
<body>
  <h1>Remote WebAPP Bedroom Light</h1>
  <div class="button-container">
    <button onclick="fetch('/on')" class="on">ON</button>
    <button onclick="fetch('/off')" class="off">OFF</button>
  </div>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleOn() {
  Serial.println("On button pressed");
  servoOn.write(40);  
  delay(500);
  servoOn.write(0);   
  delay(500);
  server.send(200, "text/plain", "ON");
}

void handleOff() {
  Serial.println("Off button pressed");
  servoOff.write(70); 
  delay(500);
  servoOff.write(0);  
  delay(500);
  server.send(200, "text/plain", "OFF");
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Attach two servos
  servoOn.attach(25);   // Servo for ON button
  servoOff.attach(14);  // Servo for OFF button
  
  servoOn.write(0);
  servoOff.write(0);

  const char* ssid = "Bedroom_Light";
  const char* password = "HIDDEN";

  WiFi.softAP(ssid, password);
  WiFi.setTxPower(WIFI_POWER_19_5dBm);

  Serial.println("Access Point started");
  Serial.print("SSID: "); Serial.println(ssid);
  Serial.print("Password: "); Serial.println(password);
  Serial.print("IP address: "); Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
