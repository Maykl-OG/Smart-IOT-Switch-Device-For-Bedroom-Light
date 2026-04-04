#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>

WebServer server(80);

// Inisialisasi LCD pada alamat I2C 0x27 (ukuran 16 kolom x 2 baris)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 3 Servo untuk 3 Sakelar
Servo myServos[3];  

// Pin ESP32 untuk masing-masing servo (Sakelar 1=25, Sakelar 2=26, Sakelar 3=27)
const int servoPins[3] = {25, 26, 27}; 

// Antarmuka Web (Disimpan di Flash Memory)
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Smart Room Control</title>
  <style>
    body {
      margin: 0; padding: 20px; min-height: 100vh; display: flex; flex-direction: column;
      align-items: center; font-family: sans-serif;
      background-color: #212822; color: white;
    }
    h1 { margin-bottom: 30px; text-align: center; }
    
    .grid-container {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 15px;
      width: 100%;
      max-width: 600px;
      text-align: center;
    }
    
    .col-header {
      font-size: 1.2rem;
      font-weight: bold;
      margin-bottom: 10px;
      color: #ddd;
    }

    .badge {
      display: inline-block;
      padding: 6px 15px;
      border-radius: 20px;
      font-size: 0.9rem;
      font-weight: bold;
      margin-bottom: 20px;
      background: #555; 
      color: white;
      transition: all 0.3s ease;
    }
    .badge.on-state { 
      background: #4CAF50; 
      box-shadow: 0 0 12px rgba(76, 175, 80, 0.6); 
    }
    .badge.off-state { 
      background: #F44336; 
      box-shadow: 0 0 12px rgba(244, 67, 54, 0.6); 
    }

    .btn {
      width: 100%; padding: 20px 0; margin-bottom: 15px;
      border-radius: 12px; border: none; font-size: 1.3rem; font-weight: bold;
      cursor: pointer; color: white; user-select: none;
      transition: all 0.1s ease-in-out;
      position: relative;
    }
    
    .on { background: #388E3C; box-shadow: 0 6px 0 #1B5E20; }
    .off { background: #D32F2F; box-shadow: 0 6px 0 #b71c1c; }

    .btn:active {
      transform: translateY(6px);
      box-shadow: 0 0 0 transparent;
    }

    .status { 
      margin-top: 30px; padding: 12px 24px; border-radius: 8px;
      background: rgba(0,0,0,0.5); font-size: 1.1rem; text-align: center;
    }
  </style>
</head>
<body>
  <h1>Smart Room Lights</h1>
  
  <div class="grid-container">
    <div>
      <div class="col-header">Sakelar 1</div>
      <div id="ind-0" class="badge off-state">OFF</div>
      <button class="btn on" onclick="sendCommand(0, 'on')">ON</button>
      <button class="btn off" onclick="sendCommand(0, 'off')">OFF</button>
    </div>
    <div>
      <div class="col-header">Sakelar 2</div>
      <div id="ind-1" class="badge off-state">OFF</div>
      <button class="btn on" onclick="sendCommand(1, 'on')">ON</button>
      <button class="btn off" onclick="sendCommand(1, 'off')">OFF</button>
    </div>
    <div>
      <div class="col-header">Sakelar 3</div>
      <div id="ind-2" class="badge off-state">OFF</div>
      <button class="btn on" onclick="sendCommand(2, 'on')">ON</button>
      <button class="btn off" onclick="sendCommand(2, 'off')">OFF</button>
    </div>
  </div>

  <div id="status" class="status">Menunggu Perintah...</div>

  <script>
    const statusDiv = document.getElementById("status");

    function sendCommand(id, state) {
      statusDiv.textContent = `Memproses Sakelar ${id + 1} -> ${state.toUpperCase()}...`;
      
      fetch(`/action?id=${id}&cmd=${state}`)
        .then(response => {
          if(response.ok) {
            statusDiv.textContent = `Sakelar ${id + 1} berhasil di-${state.toUpperCase()}`;
            
            const indicator = document.getElementById(`ind-${id}`);
            if (state === 'on') {
              indicator.textContent = 'ON';
              indicator.className = 'badge on-state';
            } else {
              indicator.textContent = 'OFF';
              indicator.className = 'badge off-state';
            }
          } else {
            statusDiv.textContent = "Gagal menghubungi ESP32!";
          }
        })
        .catch(error => {
          statusDiv.textContent = "Error jaringan! Pastikan terhubung ke WiFi ESP32.";
        });
    }
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", htmlPage);
}

void handleAction() {
  if (server.hasArg("id") && server.hasArg("cmd")) {
    int id = server.arg("id").toInt();
    String cmd = server.arg("cmd");

    if (id >= 0 && id < 3) {
      if (cmd == "on") {
        Serial.printf("Sakelar %d -> ON\n", id + 1);
        
        if (id == 0) {
          myServos[id].write(150); // Sakelar 1 ON
        } else if (id == 1) {
          myServos[id].write(50);  // Sakelar 2 ON
        } else if (id == 2) {
          myServos[id].write(40);  // <--- EDIT DERAJAT SAKELAR 3 (ON) DI SINI
        }
      } 
      else if (cmd == "off") {
        Serial.printf("Sakelar %d -> OFF\n", id + 1);
        
        if (id == 0) {
          myServos[id].write(0);  // Sakelar 1 OFF
        } else if (id == 1) {
          myServos[id].write(150); // Sakelar 2 OFF
        } else if (id == 2) {
          myServos[id].write(150); // <--- EDIT DERAJAT SAKELAR 3 (OFF) DI SINI
        }
      }

      // Kirim respon sukses ke web (Logika LCD dihapus agar tetap nampilin IP)
      server.send(200, "text/plain", "OK");
      return;
    }
  }
  server.send(400, "text/plain", "Bad Request");
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // --- INISIALISASI LCD ---
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Inisialisasi awal posisi servo
  for (int i = 0; i < 3; i++) {
    myServos[i].attach(servoPins[i]);
    
    // Set posisi OFF/Initial awal dipisah per sakelar
    if (i == 0) {
      myServos[i].write(50);  // Sakelar 1: Initial/OFF
    } else if (i == 1) {
      myServos[i].write(150); // Sakelar 2: Initial/OFF
    } else if (i == 2) {
      myServos[i].write(150); // <--- EDIT DERAJAT AWAL SAKELAR 3 DI SINI
    }
  }

  const char* ssid = "Smart_Switch";
  const char* password = "HIDDEN12345";

  WiFi.softAP(ssid, password);
  WiFi.setTxPower(WIFI_POWER_19_5dBm);

  Serial.println("\nAccess Point started");
  Serial.print("IP address: "); Serial.println(WiFi.softAPIP());

  // --- MENAMPILKAN STATUS AWAL & IP DI LCD SECARA PERMANEN ---
  lcd.setCursor(0, 0);
  lcd.print("Smart Room Ready");
  lcd.setCursor(0, 1);
  lcd.print("IP: ");
  lcd.print(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/action", handleAction); 

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
