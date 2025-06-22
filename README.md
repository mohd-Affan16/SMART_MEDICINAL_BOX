# 💊 Smart Medicine Reminder Box

An IoT-based project built using **ESP8266**, **RTC module**, **IR sensor**, and **ThingSpeak**. This system reminds users to take their medicine on time and tracks if the medicine has been taken.

---

## 📌 Features

- Connects to Wi-Fi automatically.
- Gets medicine reminder time from ThingSpeak.
- Activates buzzer if medicine is not taken.
- Detects medicine pickup using IR sensor.
- Sends status (taken/not taken) to ThingSpeak.

---

## 🔧 Components Used

- ESP8266 NodeMCU
- DS1307 RTC module
- IR sensor
- Buzzer
- 16x2 LCD (optional)
- ThingSpeak (Cloud Platform)
- Transistor, resistors, breadboard, jumper wires

---

## 🔌 Circuit Diagram
[Upload your circuit image and add it here using this format]
```markdown
![Circuit Diagram](images/circuit.jpg)
🚀 How to Run
Clone the repository.

Open with PlatformIO in VS Code.

Add your Wi-Fi credentials and ThingSpeak keys in the code.

Connect NodeMCU to your laptop via USB.

Click Upload in PlatformIO to flash the code.

🌐 ThingSpeak Setup
Create 1 private channel with 2 fields:

Field 1: Medicine Time (format: HH:MM)

Field 2: Status (1 = Taken, 0 = Not Taken)
