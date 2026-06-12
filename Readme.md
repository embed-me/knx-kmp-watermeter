# 🎵 KNX UP-Buzzer Project

The **UP-Buzzer** is a custom KNX-compatible buzzer project designed to fill the gap in the market for simple and functional KNX buzzers. By combining existing open-source hardware with custom components, this project ensures flexibility, ease of use, and extensibility.


## 🚀 Overview

This project integrates:
- 🖨️ Custom PCB
- 🛠️ Firmware
- 🏠 3D-printable housing
- 🧰 Software configuration file

Together, these components enable a KNX-compatible buzzer with additional (untested!) features.



## PCB

The PCB combines available KNX hardware with custom components for the buzzer functionality. Below is an overview of the key parts:

### **BCU (Bus Coupling Unit)**

The BCU interfaces with the KNX bus using the **NCN5130** chip. This project uses the open-source [NanoBCU](https://github.com/OpenKNX/OpenKNX/wiki/NanoBCU) to simplify development.

> 🛒 You can purchase the NanoBCU hardware here: [NanoBCU Standard](https://muster.ing-dom.de/Busankoppler/NanoBCU-Standard.html).

### **MCU (Microcontroller Unit)**

The microcontroller is based on the **RP2040**, an open-source solution provided by [OpenKNX-UP1](https://github.com/OpenKNX/OpenKNX/wiki/OpenKNX-UP1).
To program the board, use the [UP1-Progger](https://github.com/OpenKNX/OpenKNX/wiki/UP1-Progger).

> 🛒 Both the UP1 controller and programmer are available [here](https://muster.ing-dom.de/OpenKNX-UP1/OpenKNX-UP1-Controller2040.html).

### **Buzzer, Proximity Sensor, and Binary Inputs**

The schematic includes:
- A **buzzer** for alert functionality.
- Support for a **proximity sensor** for advanced triggering.
- **Binary inputs** for potential-free connections.

These additional features are currently untested but are included for enthusiasts to explore.

> 🛒 I ordered mine at [JLCPCB](https://jlcpcb.com/).

📂 For schematics, PCB layouts, and Gerber files, see the [hardware documentation](hardware/Readme.md).



## Housing

The housing is designed to fit into a standard flush-mounted box and can be 3D-printed.

📂 Get the 3D model and instructions in the [housing documentation](housing/Readme.md).



## Firmware

The custom firmware is built to be:
- 🔧 Extensible
- ✅ Understandable
- 🔓 Easy to modify

⚠️ Note: Unit tests are not included (yet!), but the code is designed with maintainability in mind.

📂 For build instructions, see the [firmware documentation](firmware/Readme.md).



## 🛠️ Software

The project includes a KNX **product database** for configuration within **ETS**. This allows you to set up and use the device as seamlessly as you would with any commercial product.

📂 Learn how to modify and deploy the product database in the [software documentation](software/Readme.md).



## 📜 License

This project is licensed under **MIT License**. For details, check out the [LICENSE file](LICENSE).

---

Enjoy building and using the **UP-Buzzer**! 🎉