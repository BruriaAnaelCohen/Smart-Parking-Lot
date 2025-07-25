# 🚗 Smart-Parking-Lot
Smart-Parking-Lot System using ARDUINO UNO board

🚗 Smart Parking Log (Arduino Uno)
An interactive Smart Parking Lot system using Arduino Uno, built to detect vehicle presence, manage gate access, track parking duration, and calculate payment — all with a touch screen interface and real-time clock.

## 🧠 System Features

🟢 Initialize System – Sets all parking spots to free, LEDs turn green.

🚘 Vehicle Detection – Ultrasonic sensor opens the gate automatically when a car approaches and there's space.

🅿️ Parking Spot Monitoring – Light sensors detect if a car is parked.

LED turns red when occupied.

Entry time is stored.

💸 Exit & Payment – When a car leaves:

LED turns green

Time is calculated

Price is shown based on duration

📺 Touchscreen UI – Displays status, buttons for actions, and payment info.

⏱️ RTC Module – Accurate tracking of parking durations.

🔧 Components Used
Arduino Uno

LDR sensors (for spot detection)

Ultrasonic sensor (for gate trigger)

Servo motor (gate control)

RTC DS1307 (Real Time Clock)

TFT Touchscreen (ILI9341)

RGB LEDs (Red/Green per parking spot)

📁 Project Structure

💰 Price Logic
Set rate: 34.90/hour

Calculated automatically on exit

Total profit shown when parking is closed
