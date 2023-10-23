# Smart Home Embedded Project

This project aims to create a fully functional smart home system based on the Atmega32 microcontroller. The system comprises five different Electronic Control Units (ECUs) to provide a comfortable and convenient home automation experience. Each ECU serves a specific purpose and contributes to the overall functionality of the smart home system.

## ECUs

### 1. Control System

The control system consists of an Atmega32 microcontroller, LCD, and keypad. It serves as the central control unit for the entire project, allowing users to control various aspects of the smart home system. 

#### System Modes

The system operates in two modes: admin mode and guest mode. Upon initial boot, the user is prompted to set up the system and enter passwords for both modes. The system is designed to be secure and resistant to hacking attempts.

- Admin Mode: Provides full access to system controls.
- Guest Mode: Offers limited access, allowing users to control only the garage door.

#### System Control

In admin mode, the user has access to the following options:

1. Room: Control individual room settings, including lights and air conditioning.
2. Garage: Control the garage door and manually close it if necessary.
3. Display: Adjust the clock and set the time.

#### Room Control

Within the Room option, the user can control the lighting and air conditioning of each room. Additionally, the AC system allows the user to set the desired temperature and turn the AC on or off.

#### Security Measures

To ensure system security, the control system implements the following measures:

- EEPROM Protection: If a hacker attempts to erase the EEPROM memory to reset the passwords, the system will be permanently blocked. Users must contact vendors for assistance in restoring system functionality.
- Password Attempts: If the user enters an incorrect password three consecutive times, the system will be temporarily blocked for 20 seconds. After ten consecutive incorrect attempts, the system will be permanently blocked, requiring vendor intervention to restore access.

### 2. Display System

The display system utilizes an Atmega32 microcontroller, LCD, 6 seven-segment displays, a 32.768KHz crystal, and a keypad. It provides visual feedback by displaying the status of various systems and the current time.

#### Time Display

The seven-segment displays accurately represent the time in the format HH:MM:SS. If the time is not adjusted through the control system, the displays will default to 00:00:00 and begin counting. Users can adjust the time through the admin mode's Display section.

### 3. Room System

The room system is responsible for controlling room lighting and air conditioning. It includes an Atmega32 microcontroller, LM35 temperature sensors, LEDs for room lighting, and a motor for the AC fan.

#### AC Control

The AC system operates as follows: 

- If the user turns on the AC without setting the temperature, the default temperature will be set to 18°C.
- The microcontroller compares the user-set temperature with the temperature measured by the LM35 sensors.
- The AC will only turn on if the user has activated it and the user-set temperature is lower than the measured temperature.

### 4. Garage System

The garage system enables control over the garage door. It consists of an Atmega32 microcontroller, LCD, and a servo motor. Users can open and close the garage door from both admin and guest modes.

### 5. CPU (Central Processing Unit)

The CPU, powered by the Atmega32 microcontroller, acts as the central hub, facilitating communication between the different systems. It utilizes SPI (Serial Peripheral Interface) communication protocol to exchange data with the ECUs. The CPU periodically sends an interrupt signal to initiate data exchange with a specific ECU, optimizing resource usage.
