# Edge Impulse firmware for Espressif ESP32-S3-EYE

Edge Impulse enables developers to create the next generation of intelligent device solutions with embedded Machine Learning. This repository contains the Edge Impulse firmware for the Espressif ESP32S3 based development boards, specifically ESP32-S3-EYE. These devicee supports Edge Impulse device features, including ingestion and inferencing.

## Requirements

### Hardware

- Espressif ESP32-S3-EYE based development boards. Using with other boards is possible, but code modifications is needed. For more on that read.

### Building the application
Then from the firmware folder execute:
```bash
get_idf
clear && idf.py build
```
```get_idf``` is an alias for export.sh script that sets up ESP IDF environment variables. Read more about it [here](https://docs.espressif.com/projects/esp-idf/en/v4.4/esp32/get-started/index.html#step-4-set-up-the-environment-variables).

### Flash

Connect the ESP32 board to your computer.

Run:
   ```bash
   idf.py -p /dev/ttyUSB0 flash monitor
   ```

Where ```/dev/ttyUSB0``` needs to be changed to actual port where ESP32 is connected on your system.

### Serial connection

Use screen, minicom or Serial monitor in Arduino IDE to set up a serial connection over USB. The following UART settings are used: 115200 baud, 8N1.