# Smart IOT Switch Device For Bedroom Light With ESP32
### Overview
This project implements an IoT-based Smart Switch using the ESP32 microcontroller. The system enables users to remotely control a traditional wall switch through a servo motor mechanism, eliminating the need to physically interact with the switch.

The motivation originated from a personal challenge: difficulty falling asleep because the light was left on. To address this, I leveraged ChatGPT to generate the initial source code, then iteratively debugged and optimized it to meet the specific requirements of this project.

### Features
- Local Web Server (ESP32 Access Point)
- Users can connect to the ESP32’s WiFi network and access the control interface via 192.168.4.1.
- Password Authentication ensures secure access to the control page.
- Web-Based Control Interface with dedicated ON and OFF buttons.
- Servo Motor Control to physically actuate the light switch.
- Fully Offline Operation — no internet connection is required, as all communication is handled locally.

### System Workflow
1. ESP32 is configured as a WiFi Access Point.
2. The user connects to the ESP32 WiFi network.
3. Upon entering the correct password, the user gains access to the control page.
4. The control page (at 192.168.4.1) provides ON/OFF buttons for device control.
5. The ESP32 commands a servo motor, which mechanically toggles the wall switch.

### Benefits
- Convenience: Remote switching without modifying home electrical wiring.
- Cost-Effective: Uses a servo motor as a physical actuator instead of specialized relay modules.
- Improved User Experience: Enables seamless control from bed or any location within WiFi range.
- Foundation for Smart Home Systems: Demonstrates a simple yet practical IoT home automation solution.
