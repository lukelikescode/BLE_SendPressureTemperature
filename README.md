![alt text](<ReadMe Files/myBanner.png>)
![GitHub Downloads (all assets, all releases)](https://img.shields.io/github/downloads/lukelikescode/XIAO_ESP32S3_BLEServer_SendAnalogData/total)

## Usage
This is a beginner-friendly project for familiarizing newbies with the XIAO ESP32S3 platform. The two main functions of this code are:
* Sense analog data (here, we use the [Adafruit BMP 180](https://www.adafruit.com/product/1603) pressure/temp sensor)
* Send analog data via bluetooth low energy (BLE) protocol

## Hardware

* [XIAO ESP32S3](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html)
* [Adafruit BMP180](https://www.adafruit.com/product/1603)
* Breadboard & Jumpers (optional)

## Setup  
1. Setting up the XIAO is relatively straightforward if you read through Seeed Studio's ["Getting Started"](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/) page. 

2. Wire up your hardware as shown in the circuit diagram below. I used a breadboard, but to each their own. 
![alt text](<ReadMe Files/CircuitDiagram.png>)

3. Download the Arduino files in this repo and then upload to the XIAO.

     *Note: uploading code is where I encountered most trouble with the XIAO. Holding down the "B" while plugging the board in will put the XIAO into bootloader mode and will solve your issue 90% of the time.*

4. Download the nRF app ([iOS](https://apps.apple.com/us/app/nrf-connect-for-mobile/id1054362403),[Android](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=en_US&gl=US)) to your mobile device. 

5. You should be able to see whatever you named your device (via code block below) in the nRF app (screenshot below).

```
BLEDevice::init("XIAO ESP32"); // this is the device name
```

![alt text](<ReadMe Files/AppScreenshots.png>)
