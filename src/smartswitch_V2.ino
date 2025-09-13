#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

WebServer server(80);
Servo servoOn;  
Servo servoOff; 

// Store HTML in flash memory (saves RAM)
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>IoT Power Control</title>
  <style>
    body {
      margin: 0; height: 100vh; display: flex; flex-direction: column;
      justify-content: center; align-items: center; font-family: sans-serif;
      background-color: #3B1919; color: white; transition: background 0.4s;
    }
    h1 { margin-bottom: 30px; text-align: center; }
    .btn {
      width: 150px; height: 150px; border-radius: 50%; border: none;
      font-size: 2rem; cursor: pointer; display: flex; align-items: center; 
      justify-content: center; color: white; transition: 0.3s;
      box-shadow: 0 6px 12px rgba(0,0,0,0.3);
    }
    .on { background: #253322; box-shadow: 0 0 20px rgba(0,255,0,0.6); }
    .off { background: #5B2B2C; box-shadow: 0 0 20px rgba(255,0,0,0.5); }
    .status { margin-top: 20px; padding: 8px 16px; border-radius: 8px;
      background: rgba(0,0,0,0.3); }
  </style>
</head>
<body>
  <h1>Bedroom Light Control</h1>
  <button id="power" class="btn off">OFF</button>
  <div id="status" class="status">Device is OFF</div>
  <script>
    let isOn = false;
const btn = document.getElementById("power");
const status = document.getElementById("status");

function updateUI() {
  if (isOn) {
    btn.textContent = "ON";
    btn.className = "btn on";
    document.body.style.background = "#212822";
    status.textContent = "Device is ON";
  } else {
    btn.textContent = "OFF";
    btn.className = "btn off";
    document.body.style.background = "#3B1919";
    status.textContent = "Device is OFF";
  }
}

btn.onclick = () => {
  isOn = !isOn;
  updateUI();
  fetch(isOn ? "/on" : "/off");
};

// Just set the UI once on page load, no servo command
updateUI();

  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", htmlPage);
}

void handleOn() {
  Serial.println("Turning ON");
  servoOn.write(40);  
  delay(300);
  servoOn.write(0);   
  server.send(200, "text/plain", "ON");
}

void handleOff() {
  Serial.println("Turning OFF");
  servoOff.write(70); 
  delay(300);
  servoOff.write(0);  
  server.send(200, "text/plain", "OFF");
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // Attach two servos
  servoOn.attach(25);   // Servo for ON
  servoOff.attach(14);  // Servo for OFF
  
  servoOn.write(0);
  servoOff.write(0);

  const char* ssid = "Bedroom_Light";
  const char* password = "mekelganteng25";

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
