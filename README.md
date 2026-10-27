# Getting-Started-with-ESP-IDF
Introduction to using ESP-IDF on ESP microcontroller for Embedded system and IoT development (ESP-IDF Firmware Development Guide for Beginners).

Welcome to the **Getting-Started-with-ESP-IDF**! This repository is designed to take you from absolute zero to building advanced, connected Embedded Systems and IoT projects using the official **Espressif IoT Development Framework (ESP-IDF)**.

Instead of relying on Arduino wrappers, this guide focuses on bare-metal concepts, register abstractions, and real-time operating systems using pure C.

---

## 🚀 Learning Roadmap & Code Examples

The projects in this repository are structured sequentially. It is highly recommended to follow them in order:

### 🎒 Module 1: Core Peripherals & Bare-Metal Basics
*   **Blink_LED** – The "Hello World" of hardware. Learn GPIO configuration, output states, and basic hardware delays.
*   **Button_Input** – Reading digital inputs, polling methods, and handling mechanical button debouncing.
*   **Hardware_Interrupts** – Moving away from polling. Configuring ISR (Interrupt Service Routine) handlers to catch button presses instantly.

### ⏱️ Module 2: Communication Protocols
*   **UART_Console** – Setting up Universal Asynchronous Receiver-Transmitter for serial debugging, transmitting text, and reading user input.
*   **I2C_Sensors** – Mastering the Inter-Integrated Circuit bus. Scanning for addresses and reading data from basic sensors (e.g., MPU6050, BMP280).

### 🔄 Module 3: FreeRTOS (Real-Time Operating System)
*   **RTOS_Tasks** – Creating, prioritizing, and managing independent execution threads.
*   **Task_Notification_Queues** – Passing data and synchronization triggers safely between distinct hardware tasks.

### 🌐 Module 4: Wi-Fi & Advanced Networking
*   **WiFi_Station_STA** – Connecting your ESP32 to a local home Wi-Fi network and obtaining an IP address via DHCP.
*   **WiFi_Access_Point_AP** – Hosting your own local wireless network network so other devices (like phones) can connect directly to the ESP32.
*   **10_WiFi_Promiscuous_Sniffer** – Going deep into packet analysis. Dropping network abstractions to capture raw 802.11 Wi-Fi frames directly out of the air.

---

## 🛠️ Prerequisites & Tooling

To run the examples in this repository, you will need:

1.  **Hardware:** An ESP32, ESP32-S3, or ESP32-C3 development board.
2.  **Development Environment:** VS Code with the official **ESP-IDF Extension** installed, or the standalone ESP-IDF Command Line Tools.
3.  **ESP-IDF Version:** Optimized for **ESP-IDF v5.x** (Stable).

---

## 💻 How to Clone and Run a Project

### 1. Clone the Repository
```bash
git clone https://github.com
cd Getting-Started-with-ESP-IDF
```

### 2. Set the Target Chip
Change directories into the specific module you want to try, then set your microcontroller target (e.g., standard esp32, esp32c3, esp32s3):
```bash
cd Blink_LED
idf.py set-target esp32
```

### 3. Build, Flash, and Monitor
Plug your ESP32 dev board into your computer's USB port and run:
```bash
idf.py build flash monitor
```
*Tip: To exit the serial monitor screen, press `Ctrl` + `]`.*

---

## 📝 Contributions & Feedback
If you spot a bug, want to improve an explanation, or wish to submit an alternative driver library, feel free to open an **Issue** or submit a **Pull Request**! 

Happy Hacking!🚀🚀 Happy Cracking!🚀🚀🚀 Happy Coding🚀🚀🚀🚀