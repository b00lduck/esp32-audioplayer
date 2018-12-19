# esp32-audioplayer
ESP32 based MP3 audio player with RFID control and case made from laser cutted plywood

!!! WORK IN PROGRESS !!!
!!! not functional yet !!!

## Used electronic hardware components:
* ESP32 NodeMCU module
* RFID-RC522 module (i.e. https://www.ebay.de/itm/172603664748)
* VS1053B module with SD-slot (i.e. http://www.ebay.de/itm/272761135002)
* Mifare classic 13.56MHz RFID cards (clones will do the job too, i.e. http://www.ebay.de/itm/122525501095)
* Adafruit PowerBoost 500 Charger module
* 3.7V LiPo battery 
* Class-D amplifier module
* SD-Card (i.e. 4GB)
* two Visaton FR7 (4 ohms) speakers
* Custom main board (see kicad folder for layout)
* Custom panel board (see kicad folder for layout)
* 1x 220uF 35V
* 1x 1uF 63V
* 4x 100nF 
* 2x 47nF
* 1x resistor ladder 4x 4.7k
* 1x 10R 
* 1x 10kR
* 1x 22kR
* 4x WSL10 (2x5 header 2.54mm with notch)
* 2x 2-pin header 2.54mm
* 4x 4-pin header 2.54mm
* some wire for airwires
* some insulated wire 1mm², red and black (for internal connections)
* 1x USB 2 Micro-B connector
* 1x power switch
* 1x USB 2 micro cable (for internal connection)
* optional: MEMS gyroscope module
* optional: 0.96 inch I2C OLED display

## Wood parts:
* 6 sheets, one for every side of the case
* 2 distance rings for the speakers

## Metal parts:
* 2x 100x75x0.8mm aluminium sheet

## 3D-Printed parts:
* 8x inner edge reinforcements
* 4x M3x3 standoff
* 8x M3*8 standoff (for main board)
* 4x M3x11 standoff (for MP3 board)
* 1x USB receptacle holder
* 1x SD-Card slot box
* 1x SD-Card slot cover

## Screws
* 4x M3x25 DIN 7991/ISO 10642 (countersunk allen bolt)
* 32x M3 DIN 934/ISO 4042 (nut)
* 8x M3 DIN 125/ISO 7089 (washer)
* 8x M3x20 DIN 912/ISO 4762 (cylinder head allen bolt)
* 2x M2x10 DIN 912/ISO 4762 (cylinder head allen bolt) for amp
* 2x M2 DIN 934/ISO 4042 (nut)
* 2x M2 DIN 125/ISO 7089 (washer)
* 8x M3x?? DIN 7991/ISO 10642 (countersunk allen bolt) for speakers
* 24x M3x15 DIN 7991/ISO 10642 (countersunk allen bolt) for inner corners

## Used software:
* VSCode with PlatformioIDE

## Used Arduino Libraries:
* SdFat 1.0.7
* MFRC522 1.3.6

## Port mapping
see src/config.h

