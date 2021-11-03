# Arduino-Radio
A basic FM radio with frequency display using an Arduino UNO and RDA5807M radio module.

## Components
* Arduino UNO
* RDA5807M Radio Module
* PAM8403 2-Channel Digital Audio Amplifier Module
* 4Ω 3-Watt Speakers
* 1602 Serial LCD Module
* SPST Pushbutton Switches

## Schematic
![image](https://user-images.githubusercontent.com/77598913/140165909-e8308d82-905d-4a68-ab45-5453e7f420be.png)

## Known Issues
* Noise is present on the I2C bus (possibly due to lack of pull-up resistors on the I2C bus)
* RDA5807M is surface mount and needed significant modification to use in a DIP package
* Power issues when volume is increased above ~50%

## Possible Updates
* Switch to using an Arduino Nano and design a PCB instead of using a breadboard
* Radio Data System (RDS) implementation
* Design an external +5V power supply to power the Arduino and audio amplifier instead of powering the amplifier from the Arduino
