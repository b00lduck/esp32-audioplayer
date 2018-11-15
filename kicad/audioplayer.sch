EESchema Schematic File Version 4
LIBS:audioplayer-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L audioplayer:VS1053_SD_breakout U5
U 1 1 59B6EFEA
P 8450 1950
F 0 "U5" H 8500 2450 50  0000 C CNN
F 1 "VS1053/SD_breakout" V 8450 1850 50  0000 C CNN
F 2 "audioplayer:vs1053_breakout_upside_down" V 8550 1850 50  0001 C CNN
F 3 "" V 8550 1850 50  0001 C CNN
	1    8450 1950
	1    0    0    -1  
$EndComp
Entry Wire Line
	6400 1750 6500 1650
Entry Wire Line
	6400 1850 6500 1750
Entry Wire Line
	6400 1950 6500 1850
Entry Wire Line
	6400 3550 6500 3650
Wire Wire Line
	7550 1650 6500 1650
Text Label 6500 1650 0    60   ~ 0
MISO
Text Label 6500 1750 0    60   ~ 0
MOSI
Text Label 6500 1850 0    60   ~ 0
SCK
Text Label 6500 3650 0    60   ~ 0
SCK
Text Label 6500 3450 0    60   ~ 0
MOSI
Text Label 6500 3550 0    60   ~ 0
MISO
$Comp
L power:GND #PWR019
U 1 1 59B71063
P 8400 2800
F 0 "#PWR019" H 8400 2550 50  0001 C CNN
F 1 "GND" H 8400 2650 50  0000 C CNN
F 2 "" H 8400 2800 50  0000 C CNN
F 3 "" H 8400 2800 50  0000 C CNN
	1    8400 2800
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR017
U 1 1 59B7117B
P 8600 3500
F 0 "#PWR017" H 8600 3350 50  0001 C CNN
F 1 "+3.3V" H 8600 3640 50  0000 C CNN
F 2 "" H 8600 3500 50  0000 C CNN
F 3 "" H 8600 3500 50  0000 C CNN
	1    8600 3500
	1    0    0    -1  
$EndComp
Entry Wire Line
	6400 3350 6500 3450
Entry Wire Line
	6400 3450 6500 3550
$Comp
L RF_Module:ESP32-nodeMCU U2
U 1 1 5BB7525F
P 2900 2300
F 0 "U2" H 5300 3250 50  0000 C CNN
F 1 "ESP32-nodeMCU" V 4900 2400 50  0000 C CNN
F 2 "audioplayer:esp32-nodemcu" V 4650 100 50  0001 C CNN
F 3 "" V 4650 100 50  0001 C CNN
	1    2900 2300
	1    0    0    -1  
$EndComp
Entry Wire Line
	6300 2650 6400 2750
Entry Wire Line
	6300 2550 6400 2650
Entry Wire Line
	6300 3000 6400 3100
Text Label 6100 3000 0    50   ~ 0
MOSI
Text Label 6100 2650 0    50   ~ 0
MISO
Text Label 6150 2550 0    50   ~ 0
SCK
$Comp
L power:+5V #PWR08
U 1 1 5BB77311
P 4850 750
F 0 "#PWR08" H 4850 600 50  0001 C CNN
F 1 "+5V" H 4850 890 50  0000 C CNN
F 2 "" H 4850 750 50  0000 C CNN
F 3 "" H 4850 750 50  0000 C CNN
	1    4850 750 
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR09
U 1 1 5BB776F7
P 5050 750
F 0 "#PWR09" H 5050 600 50  0001 C CNN
F 1 "+3.3V" H 5050 900 50  0000 C CNN
F 2 "" H 5050 750 50  0000 C CNN
F 3 "" H 5050 750 50  0000 C CNN
	1    5050 750 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5BB77F8D
P 4850 3650
F 0 "#PWR012" H 4850 3400 50  0001 C CNN
F 1 "GND" H 4850 3500 50  0000 C CNN
F 2 "" H 4850 3650 50  0000 C CNN
F 3 "" H 4850 3650 50  0000 C CNN
	1    4850 3650
	1    0    0    -1  
$EndComp
Entry Wire Line
	6950 2900 7050 3000
Entry Wire Line
	6950 2800 7050 2900
Text Label 7050 3150 0    50   ~ 0
I²C
Text Label 6400 2100 0    50   ~ 0
SPI
Text Label 6800 2900 0    50   ~ 0
SCL
Text Label 6800 2800 0    50   ~ 0
SDA
$Comp
L audioplayer:OLED U3
U 1 1 5BD9DCD6
P 5100 5000
F 0 "U3" H 5400 5300 50  0000 C CNN
F 1 "OLED" H 5350 5200 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 5100 5000 50  0001 C CNN
F 3 "" H 5100 5000 50  0001 C CNN
	1    5100 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 4950 5550 4950
Wire Wire Line
	5550 5050 5750 5050
Entry Wire Line
	5750 4950 5850 5050
Entry Wire Line
	5750 5050 5850 5150
Text Label 5600 5050 0    50   ~ 0
SDA
Text Label 5600 4950 0    50   ~ 0
SCL
$Comp
L power:+3.3V #PWR010
U 1 1 5BDAD9AE
P 5150 4600
F 0 "#PWR010" H 5150 4450 50  0001 C CNN
F 1 "+3.3V" H 5150 4740 50  0000 C CNN
F 2 "" H 5150 4600 50  0000 C CNN
F 3 "" H 5150 4600 50  0000 C CNN
	1    5150 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 4650 5150 4600
$Comp
L power:GND #PWR011
U 1 1 5BDB194E
P 5150 5400
F 0 "#PWR011" H 5150 5150 50  0001 C CNN
F 1 "GND" H 5150 5250 50  0000 C CNN
F 2 "" H 5150 5400 50  0000 C CNN
F 3 "" H 5150 5400 50  0000 C CNN
	1    5150 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 5400 5150 5350
$Comp
L power:GND #PWR023
U 1 1 5BDB4A71
P 9950 2900
F 0 "#PWR023" H 9950 2650 50  0001 C CNN
F 1 "GND" H 9950 2750 50  0000 C CNN
F 2 "" H 9950 2900 50  0000 C CNN
F 3 "" H 9950 2900 50  0000 C CNN
	1    9950 2900
	1    0    0    -1  
$EndComp
$Comp
L audioplayer:HW-323 U7
U 1 1 5BDD9E66
P 9950 2250
F 0 "U7" H 10150 3000 50  0000 C CNN
F 1 "HW-323" H 9950 2100 50  0000 C CNN
F 2 "audioplayer:hw-323" H 10100 3200 50  0001 C CNN
F 3 "" H 10100 3200 50  0001 C CNN
	1    9950 2250
	1    0    0    -1  
$EndComp
$Comp
L audioplayer:GY-521 U4
U 1 1 5BDD9F65
P 9000 4650
F 0 "U4" H 8850 4600 50  0000 L CNN
F 1 "GY-521" H 8650 3950 50  0000 L CNN
F 2 "audioplayer:gy-521" H 9000 4650 50  0001 C CNN
F 3 "" H 9000 4650 50  0001 C CNN
	1    9000 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 1750 7550 1750
Wire Wire Line
	6500 1850 7550 1850
Wire Wire Line
	7050 2050 7550 2050
$Comp
L power:+5V #PWR013
U 1 1 5BE25F75
P 8700 4650
F 0 "#PWR013" H 8700 4500 50  0001 C CNN
F 1 "+5V" H 8700 4790 50  0000 C CNN
F 2 "" H 8700 4650 50  0000 C CNN
F 3 "" H 8700 4650 50  0000 C CNN
	1    8700 4650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 5BE25FBC
P 8700 5850
F 0 "#PWR014" H 8700 5600 50  0001 C CNN
F 1 "GND" H 8700 5700 50  0000 C CNN
F 2 "" H 8700 5850 50  0000 C CNN
F 3 "" H 8700 5850 50  0000 C CNN
	1    8700 5850
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR020
U 1 1 5BE2603F
P 9000 950
F 0 "#PWR020" H 9000 800 50  0001 C CNN
F 1 "+5V" H 9000 1090 50  0000 C CNN
F 2 "" H 9000 950 50  0000 C CNN
F 3 "" H 9000 950 50  0000 C CNN
	1    9000 950 
	1    0    0    -1  
$EndComp
$Comp
L audioplayer:PB-500 U1
U 1 1 5BE27340
P 2500 4050
F 0 "U1" H 2425 4165 50  0000 C CNN
F 1 "PB-500" H 2425 4074 50  0000 C CNN
F 2 "audioplayer:adafruit-pb500-charge" H 2500 4050 50  0001 C CNN
F 3 "" H 2500 4050 50  0001 C CNN
	1    2500 4050
	1    0    0    -1  
$EndComp
Entry Wire Line
	7050 4900 7150 5000
Entry Wire Line
	7050 5000 7150 5100
Text Label 7150 5100 0    50   ~ 0
SDA
Text Label 7150 5000 0    50   ~ 0
SCL
$Comp
L power:+5V #PWR05
U 1 1 5BE3EE13
P 2850 4300
F 0 "#PWR05" H 2850 4150 50  0001 C CNN
F 1 "+5V" H 2850 4440 50  0000 C CNN
F 2 "" H 2850 4300 50  0000 C CNN
F 3 "" H 2850 4300 50  0000 C CNN
	1    2850 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 4400 2850 4300
$Comp
L power:GND #PWR04
U 1 1 5BE42FF5
P 2450 5250
F 0 "#PWR04" H 2450 5000 50  0001 C CNN
F 1 "GND" H 2450 5100 50  0000 C CNN
F 2 "" H 2450 5250 50  0000 C CNN
F 3 "" H 2450 5250 50  0000 C CNN
	1    2450 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 5200 2550 5150
Wire Wire Line
	2450 5150 2450 5200
Connection ~ 2450 5200
Wire Wire Line
	2450 5200 2550 5200
Wire Wire Line
	2450 5200 2450 5250
$Comp
L Device:L L2
U 1 1 5BDE9FBD
P 9150 1000
F 0 "L2" V 9340 1000 50  0000 C CNN
F 1 "L" V 9249 1000 50  0000 C CNN
F 2 "Inductor_THT:L_Axial_L6.6mm_D2.7mm_P10.16mm_Horizontal_Vishay_IM-2" H 9150 1000 50  0001 C CNN
F 3 "~" H 9150 1000 50  0001 C CNN
	1    9150 1000
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP C2
U 1 1 5BDEA165
P 9500 1200
F 0 "C2" H 9618 1246 50  0000 L CNN
F 1 "470u" H 9618 1155 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D10.0mm_P5.00mm" H 9538 1050 50  0001 C CNN
F 3 "~" H 9500 1200 50  0001 C CNN
	1    9500 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 950  9000 1000
Wire Wire Line
	9300 1000 9500 1000
Wire Wire Line
	9500 1050 9500 1000
$Comp
L power:GND #PWR022
U 1 1 5BDF8692
P 9500 1350
F 0 "#PWR022" H 9500 1100 50  0001 C CNN
F 1 "GND" H 9500 1200 50  0000 C CNN
F 2 "" H 9500 1350 50  0000 C CNN
F 3 "" H 9500 1350 50  0000 C CNN
	1    9500 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 1000 9950 1000
Connection ~ 9500 1000
Wire Wire Line
	8850 800  8850 1250
Wire Wire Line
	8850 1800 9550 1800
Wire Wire Line
	8700 2050 9450 2050
Wire Wire Line
	9450 2050 9550 1950
Wire Wire Line
	8700 2200 9550 2200
Wire Wire Line
	9950 1000 9950 1450
$Comp
L power:+5V #PWR015
U 1 1 5BE9C27D
P 7200 1050
F 0 "#PWR015" H 7200 900 50  0001 C CNN
F 1 "+5V" H 7200 1190 50  0000 C CNN
F 2 "" H 7200 1050 50  0000 C CNN
F 3 "" H 7200 1050 50  0000 C CNN
	1    7200 1050
	1    0    0    -1  
$EndComp
$Comp
L Device:L L1
U 1 1 5BE9C283
P 7350 1050
F 0 "L1" V 7540 1050 50  0000 C CNN
F 1 "L" V 7449 1050 50  0000 C CNN
F 2 "Inductor_THT:L_Axial_L6.6mm_D2.7mm_P10.16mm_Horizontal_Vishay_IM-2" H 7350 1050 50  0001 C CNN
F 3 "~" H 7350 1050 50  0001 C CNN
	1    7350 1050
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP C1
U 1 1 5BE9C289
P 7750 1200
F 0 "C1" H 7868 1246 50  0000 L CNN
F 1 "100u" H 7868 1155 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D10.0mm_P5.00mm" H 7788 1050 50  0001 C CNN
F 3 "~" H 7750 1200 50  0001 C CNN
	1    7750 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 1050 7750 1050
Wire Wire Line
	8400 1050 8400 1350
$Comp
L power:GND #PWR016
U 1 1 5BEA82EC
P 7750 1350
F 0 "#PWR016" H 7750 1100 50  0001 C CNN
F 1 "GND" H 7750 1200 50  0000 C CNN
F 2 "" H 7750 1350 50  0000 C CNN
F 3 "" H 7750 1350 50  0000 C CNN
	1    7750 1350
	1    0    0    -1  
$EndComp
Text Label 6700 800  2    50   ~ 0
AMP_ENABLE
$Comp
L Switch:SW_SPST SW1
U 1 1 5BEEA3BD
P 3100 4750
F 0 "SW1" H 3100 4650 50  0000 C CNN
F 1 "POWER" H 3100 4550 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 3100 4750 50  0001 C CNN
F 3 "" H 3100 4750 50  0001 C CNN
	1    3100 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 4750 2900 4750
Wire Wire Line
	3300 4750 3450 4750
Wire Wire Line
	3450 4750 3450 4850
$Comp
L power:GND #PWR06
U 1 1 5BEF6B43
P 3450 4850
F 0 "#PWR06" H 3450 4600 50  0001 C CNN
F 1 "GND" H 3450 4700 50  0000 C CNN
F 2 "" H 3450 4850 50  0000 C CNN
F 3 "" H 3450 4850 50  0000 C CNN
	1    3450 4850
	1    0    0    -1  
$EndComp
Text GLabel 5700 3250 2    50   Input ~ 0
LED1
Text GLabel 5700 3350 2    50   Input ~ 0
LED2
Text GLabel 4000 2600 0    50   Input ~ 0
LED3
Text GLabel 3300 3250 0    50   Input ~ 0
SW1
Text GLabel 3650 3350 0    50   Input ~ 0
SW2
Text GLabel 3950 3050 0    50   Input ~ 0
SW3
Wire Wire Line
	2850 4650 2950 4650
Wire Wire Line
	2000 4400 1950 4400
$Comp
L Device:R R2
U 1 1 5BF0B55E
P 1950 3100
F 0 "R2" H 2050 3150 50  0000 C CNN
F 1 "33k" H 2100 3050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 1880 3100 50  0001 C CNN
F 3 "~" H 1950 3100 50  0001 C CNN
	1    1950 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5BF0B649
P 1600 3100
F 0 "R1" H 1530 3054 50  0000 R CNN
F 1 "10k" H 1530 3145 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 1530 3100 50  0001 C CNN
F 3 "~" H 1600 3100 50  0001 C CNN
	1    1600 3100
	-1   0    0    1   
$EndComp
Wire Wire Line
	1600 2950 1600 2850
Wire Wire Line
	1600 3250 1600 3300
$Comp
L power:GND #PWR02
U 1 1 5BF23296
P 1600 3300
F 0 "#PWR02" H 1600 3050 50  0001 C CNN
F 1 "GND" H 1600 3150 50  0000 C CNN
F 2 "" H 1600 3300 50  0000 C CNN
F 3 "" H 1600 3300 50  0000 C CNN
	1    1600 3300
	1    0    0    -1  
$EndComp
Text Label 2750 2850 0    50   ~ 0
ADC_BATT
Text Label 2950 3300 1    50   ~ 0
LOW_BATT
$Comp
L Connector_Generic:Conn_01x02 J2
U 1 1 5BE39FB9
P 3650 4500
F 0 "J2" H 3729 4492 50  0000 L CNN
F 1 "Charge_USB" H 3729 4401 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 3650 4500 50  0001 C CNN
F 3 "~" H 3650 4500 50  0001 C CNN
	1    3650 4500
	1    0    0    -1  
$EndComp
Text Notes 3650 4850 0    50   ~ 0
Power supply\nfrom USB jack
$Comp
L Connector:Conn_01x04_Male J3
U 1 1 5BE4DA0C
P 10700 2000
F 0 "J3" H 10673 1973 50  0000 R CNN
F 1 "Speakers" H 10673 1882 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 10700 2000 50  0001 C CNN
F 3 "~" H 10700 2000 50  0001 C CNN
	1    10700 2000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10500 1900 10350 1900
Wire Wire Line
	10350 2000 10500 2000
Wire Wire Line
	10350 2150 10400 2150
Wire Wire Line
	10400 2150 10400 2100
Wire Wire Line
	10400 2100 10500 2100
Wire Wire Line
	10350 2250 10400 2250
Wire Wire Line
	10400 2250 10400 2200
Wire Wire Line
	10400 2200 10500 2200
Text GLabel 2600 6350 0    50   Input ~ 0
LED1
Text GLabel 2600 6450 0    50   Input ~ 0
LED2
Text GLabel 4900 6750 2    50   Input ~ 0
SW1
Text GLabel 2600 6550 0    50   Input ~ 0
LED3
Text GLabel 4900 6850 2    50   Input ~ 0
SW2
Text GLabel 4900 6950 2    50   Input ~ 0
SW3
$Comp
L power:GND #PWR03
U 1 1 5BE91661
P 5200 6650
F 0 "#PWR03" H 5200 6400 50  0001 C CNN
F 1 "GND" H 5205 6477 50  0000 C CNN
F 2 "" H 5200 6650 50  0001 C CNN
F 3 "" H 5200 6650 50  0001 C CNN
	1    5200 6650
	1    0    0    -1  
$EndComp
Text Notes 4350 6200 0    50   ~ 0
LED1: playing\nLED2: \nLED3: \n\nSW1: play/pause\nSW2: Volume -\nSW3: Volume +
Wire Wire Line
	3650 3350 3700 3350
Wire Wire Line
	3950 3050 4000 3050
Wire Wire Line
	1600 2850 1950 2850
Wire Wire Line
	2850 4500 3450 4500
Wire Wire Line
	1950 2950 1950 2850
Wire Wire Line
	1950 3250 1950 4400
Text Notes 1600 2850 0    50   ~ 0
4.3V battery -> 1.0V ADC
Wire Wire Line
	3300 3250 3400 3250
$Comp
L Device:R R3
U 1 1 5C16DEB1
P 3400 3600
F 0 "R3" H 3350 3650 50  0000 R CNN
F 1 "4.7k" H 3350 3550 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3330 3600 50  0001 C CNN
F 3 "~" H 3400 3600 50  0001 C CNN
	1    3400 3600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 5C16DF84
P 3700 3600
F 0 "R5" H 3850 3550 50  0000 R CNN
F 1 "4.7k" H 3900 3650 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3630 3600 50  0001 C CNN
F 3 "~" H 3700 3600 50  0001 C CNN
	1    3700 3600
	-1   0    0    1   
$EndComp
$Comp
L Device:R R7
U 1 1 5C16DFD0
P 4000 3600
F 0 "R7" H 4150 3550 50  0000 R CNN
F 1 "4.7k" H 4200 3650 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3930 3600 50  0001 C CNN
F 3 "~" H 4000 3600 50  0001 C CNN
	1    4000 3600
	-1   0    0    1   
$EndComp
Wire Wire Line
	2950 2950 2950 4650
Text Notes 8650 2150 0    50   ~ 0
Audio signal
$Comp
L Device:R R9
U 1 1 5C263D1E
P 9000 1450
F 0 "R9" H 8930 1404 50  0000 R CNN
F 1 "22k" H 8930 1495 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 8930 1450 50  0001 C CNN
F 3 "~" H 9000 1450 50  0001 C CNN
	1    9000 1450
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR021
U 1 1 5C264198
P 9000 1600
F 0 "#PWR021" H 9000 1350 50  0001 C CNN
F 1 "GND" H 9000 1450 50  0000 C CNN
F 2 "" H 9000 1600 50  0000 C CNN
F 3 "" H 9000 1600 50  0000 C CNN
	1    9000 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 1300 9000 1250
Wire Wire Line
	9000 1250 8850 1250
Connection ~ 8850 1250
Wire Wire Line
	8850 1250 8850 1800
Wire Wire Line
	4350 2400 4150 2400
Wire Wire Line
	4150 2400 4150 4000
Wire Wire Line
	4350 2500 4100 2500
Wire Wire Line
	4100 2500 4100 4150
Connection ~ 1950 2850
Connection ~ 4000 3050
Wire Wire Line
	4350 3250 3400 3250
Connection ~ 3400 3250
Wire Wire Line
	3700 3350 4350 3350
Connection ~ 3700 3350
Wire Wire Line
	1950 2850 2600 2850
Wire Wire Line
	2950 2950 4350 2950
Wire Wire Line
	4000 3050 4350 3050
Wire Wire Line
	4350 1400 4250 1400
Wire Wire Line
	4250 1400 4250 900 
Wire Wire Line
	4250 900  6950 900 
Wire Wire Line
	6950 900  6950 2250
Wire Wire Line
	4350 2700 4200 2700
Wire Wire Line
	7500 3900 4200 3900
Wire Wire Line
	4200 2700 4200 3900
Wire Wire Line
	7050 2050 7050 850 
Wire Wire Line
	7050 850  4200 850 
Wire Wire Line
	4200 850  4200 1700
Wire Wire Line
	4200 1700 4350 1700
Wire Wire Line
	7050 2450 7050 2350
Wire Wire Line
	7050 2350 5450 2350
Wire Wire Line
	7050 2450 7550 2450
Wire Wire Line
	5450 2450 7000 2450
Wire Wire Line
	7000 2450 7000 2550
Wire Wire Line
	7000 2550 7550 2550
Wire Wire Line
	5450 2550 6300 2550
Wire Wire Line
	6300 2650 5450 2650
Wire Wire Line
	7150 5100 8300 5100
Wire Wire Line
	7150 5000 8300 5000
Wire Wire Line
	5450 3000 6300 3000
Wire Wire Line
	5450 2800 6950 2800
Wire Wire Line
	5450 2900 6950 2900
Wire Bus Line
	5850 4500 7050 4500
Connection ~ 7050 4500
Wire Wire Line
	4850 1000 4850 750 
Wire Wire Line
	5050 750  5050 1000
Wire Wire Line
	7750 1050 8400 1050
Connection ~ 7750 1050
Wire Wire Line
	4000 2600 4350 2600
Wire Wire Line
	3150 3950 3150 4000
Wire Wire Line
	3150 4000 3400 4000
Wire Wire Line
	4000 4000 4000 3750
Wire Wire Line
	3700 3750 3700 4000
Connection ~ 3700 4000
Wire Wire Line
	3700 4000 4000 4000
Wire Wire Line
	3400 3750 3400 4000
Connection ~ 3400 4000
Wire Wire Line
	3400 4000 3700 4000
Wire Wire Line
	3400 3450 3400 3250
Wire Wire Line
	3700 3450 3700 3350
Wire Wire Line
	4000 3450 4000 3050
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J4
U 1 1 5BEA4391
P 8950 3800
F 0 "J4" H 9000 4217 50  0000 C CNN
F 1 "RFID" H 9000 4126 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x05_P2.54mm_Vertical" H 8950 3800 50  0001 C CNN
F 3 "~" H 8950 3800 50  0001 C CNN
	1    8950 3800
	-1   0    0    1   
$EndComp
Wire Wire Line
	8650 3600 8600 3600
Wire Wire Line
	8600 3600 8600 3500
Wire Wire Line
	7950 3450 7950 3700
Wire Wire Line
	6500 3450 7950 3450
Wire Wire Line
	7850 3800 7850 3550
Wire Wire Line
	6500 3550 7850 3550
Wire Wire Line
	7750 3650 7750 3900
Wire Wire Line
	6500 3650 7750 3650
Wire Wire Line
	9200 4150 9200 4000
Text Label 8550 4000 0    50   ~ 0
SS
Text Label 9000 4150 0    50   ~ 0
RST
Text Label 8000 3900 0    50   ~ 0
SCK
Text Label 8000 3800 0    50   ~ 0
MISO
Text Label 8000 3700 0    50   ~ 0
MOSI
Wire Wire Line
	4100 4150 9200 4150
Wire Wire Line
	4150 4000 8650 4000
Wire Wire Line
	7950 3700 8250 3700
Wire Wire Line
	8250 3700 8250 3800
Wire Wire Line
	8650 3800 8250 3800
Wire Wire Line
	7850 3800 8200 3800
Wire Wire Line
	8200 3800 8200 3850
Wire Wire Line
	8200 3850 8500 3850
Wire Wire Line
	8500 3850 8500 3700
Wire Wire Line
	8500 3700 8650 3700
Wire Wire Line
	7750 3900 8650 3900
Wire Wire Line
	9150 4000 9200 4000
Wire Wire Line
	9300 3900 9300 3950
Wire Wire Line
	9150 3900 9200 3900
Wire Wire Line
	9200 3900 9200 3800
Wire Wire Line
	9200 3600 9150 3600
Connection ~ 9200 3900
Wire Wire Line
	9200 3900 9300 3900
Wire Wire Line
	9150 3700 9200 3700
Connection ~ 9200 3700
Wire Wire Line
	9200 3700 9200 3600
Wire Wire Line
	9150 3800 9200 3800
Connection ~ 9200 3800
Wire Wire Line
	9200 3800 9200 3700
$Comp
L power:GND #PWR0101
U 1 1 5BEF0E36
P 9300 3950
F 0 "#PWR0101" H 9300 3700 50  0001 C CNN
F 1 "GND" H 9300 3800 50  0000 C CNN
F 2 "" H 9300 3950 50  0000 C CNN
F 3 "" H 9300 3950 50  0000 C CNN
	1    9300 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 4600 3400 4600
Wire Wire Line
	3400 4600 3400 4700
Wire Wire Line
	3400 4700 3600 4700
Wire Wire Line
	3600 4700 3600 5500
Wire Wire Line
	3600 5500 2350 5500
Wire Wire Line
	2350 5150 2350 5500
Text Label 7250 2050 0    50   ~ 0
SD_CS
Text Label 7500 3000 0    50   ~ 0
MP3_XCS
Text Label 6950 2150 0    50   ~ 0
MP3_XRESET
Text Label 6550 2350 0    50   ~ 0
MP3_XDCS
Text Label 6550 2450 0    50   ~ 0
MP3_DREQ
Wire Wire Line
	7500 2250 7500 3900
Wire Wire Line
	7550 2350 7100 2350
Wire Wire Line
	7100 2350 7100 2250
Wire Wire Line
	7100 2250 6950 2250
Wire Wire Line
	7500 2250 7550 2250
Wire Wire Line
	5450 3150 5550 3150
Wire Wire Line
	5550 3150 5550 800 
Wire Wire Line
	5550 800  8850 800 
Text Label 8000 1050 0    50   ~ 0
VCC_MP3
Text Label 9600 1000 0    50   ~ 0
VCC_AMP
Wire Wire Line
	9950 2800 9950 2900
$Comp
L Device:C C4
U 1 1 5BEB4DAC
P 7500 1200
F 0 "C4" H 7615 1246 50  0000 L CNN
F 1 "C" H 7615 1155 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.1mm_W3.2mm_P5.00mm" H 7538 1050 50  0001 C CNN
F 3 "~" H 7500 1200 50  0001 C CNN
	1    7500 1200
	1    0    0    -1  
$EndComp
Connection ~ 7500 1050
Wire Wire Line
	7500 1350 7750 1350
Connection ~ 7750 1350
$Comp
L Device:C C6
U 1 1 5BEE5F79
P 9250 1200
F 0 "C6" H 9365 1246 50  0000 L CNN
F 1 "C" H 9365 1155 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.1mm_W3.2mm_P5.00mm" H 9288 1050 50  0001 C CNN
F 3 "~" H 9250 1200 50  0001 C CNN
	1    9250 1200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 5BEE60EB
P 9200 5200
F 0 "C5" H 9315 5246 50  0000 L CNN
F 1 "C" H 9315 5155 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.1mm_W3.2mm_P5.00mm" H 9238 5050 50  0001 C CNN
F 3 "~" H 9200 5200 50  0001 C CNN
	1    9200 5200
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0102
U 1 1 5BEE61D8
P 9200 5050
F 0 "#PWR0102" H 9200 4900 50  0001 C CNN
F 1 "+5V" H 9200 5190 50  0000 C CNN
F 2 "" H 9200 5050 50  0000 C CNN
F 3 "" H 9200 5050 50  0000 C CNN
	1    9200 5050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 5BEE6219
P 9200 5350
F 0 "#PWR0103" H 9200 5100 50  0001 C CNN
F 1 "GND" H 9200 5200 50  0000 C CNN
F 2 "" H 9200 5350 50  0000 C CNN
F 3 "" H 9200 5350 50  0000 C CNN
	1    9200 5350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C7
U 1 1 5BEE6317
P 9550 3800
F 0 "C7" H 9665 3846 50  0000 L CNN
F 1 "C" H 9665 3755 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.1mm_W3.2mm_P5.00mm" H 9588 3650 50  0001 C CNN
F 3 "~" H 9550 3800 50  0001 C CNN
	1    9550 3800
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0104
U 1 1 5BEE6607
P 9550 3650
F 0 "#PWR0104" H 9550 3500 50  0001 C CNN
F 1 "+3.3V" H 9550 3790 50  0000 C CNN
F 2 "" H 9550 3650 50  0000 C CNN
F 3 "" H 9550 3650 50  0000 C CNN
	1    9550 3650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 5BEE664A
P 9550 3950
F 0 "#PWR0105" H 9550 3700 50  0001 C CNN
F 1 "GND" H 9550 3800 50  0000 C CNN
F 2 "" H 9550 3950 50  0000 C CNN
F 3 "" H 9550 3950 50  0000 C CNN
	1    9550 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5BF0EDA4
P 3950 900
F 0 "C3" H 4065 946 50  0000 L CNN
F 1 "C" H 4065 855 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.1mm_W3.2mm_P5.00mm" H 3988 750 50  0001 C CNN
F 3 "~" H 3950 900 50  0001 C CNN
	1    3950 900 
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0106
U 1 1 5BF0EDAA
P 3950 750
F 0 "#PWR0106" H 3950 600 50  0001 C CNN
F 1 "+5V" H 3950 890 50  0000 C CNN
F 2 "" H 3950 750 50  0000 C CNN
F 3 "" H 3950 750 50  0000 C CNN
	1    3950 750 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 5BF0EDB0
P 3950 1050
F 0 "#PWR0107" H 3950 800 50  0001 C CNN
F 1 "GND" H 3950 900 50  0000 C CNN
F 2 "" H 3950 1050 50  0000 C CNN
F 3 "" H 3950 1050 50  0000 C CNN
	1    3950 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 1350 9500 1350
Connection ~ 9500 1350
Wire Wire Line
	9250 1050 9300 1050
Wire Wire Line
	9300 1050 9300 1000
Connection ~ 9300 1000
$Comp
L Device:C C8
U 1 1 5BEBC35A
P 2600 3050
F 0 "C8" H 2715 3096 50  0000 L CNN
F 1 "1u" H 2715 3005 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.2mm_W5.5mm_P5.00mm_FKS2_FKP2_MKS2_MKP2" H 2638 2900 50  0001 C CNN
F 3 "~" H 2600 3050 50  0001 C CNN
	1    2600 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 2900 2600 2850
Connection ~ 2600 2850
Wire Wire Line
	2600 2850 4350 2850
$Comp
L power:GND #PWR0108
U 1 1 5BEC4ABC
P 2600 3250
F 0 "#PWR0108" H 2600 3000 50  0001 C CNN
F 1 "GND" H 2600 3100 50  0000 C CNN
F 2 "" H 2600 3250 50  0000 C CNN
F 3 "" H 2600 3250 50  0000 C CNN
	1    2600 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 3250 2600 3200
$Comp
L Transistor_Array:ULN2003 U6
U 1 1 5BEE63E1
P 3050 6550
F 0 "U6" H 3050 7217 50  0000 C CNN
F 1 "ULN2003" H 3050 7126 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm" H 3100 6000 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/uln2003a.pdf" H 3150 6350 50  0001 C CNN
	1    3050 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 6350 2650 6350
Wire Wire Line
	2600 6450 2650 6450
Wire Wire Line
	2600 6550 2650 6550
$Comp
L power:GND #PWR0110
U 1 1 5BF348CC
P 3050 7150
F 0 "#PWR0110" H 3050 6900 50  0001 C CNN
F 1 "GND" H 3055 6977 50  0000 C CNN
F 2 "" H 3050 7150 50  0001 C CNN
F 3 "" H 3050 7150 50  0001 C CNN
	1    3050 7150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 3350 5700 3350
Wire Wire Line
	5450 3250 5700 3250
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J1
U 1 1 5BE80741
P 4550 6750
F 0 "J1" H 4600 7167 50  0000 C CNN
F 1 "Panel" H 4600 7076 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x05_P2.54mm_Vertical" H 4550 6750 50  0001 C CNN
F 3 "~" H 4550 6750 50  0001 C CNN
	1    4550 6750
	1    0    0    1   
$EndComp
Text Label 3500 6350 0    50   ~ 0
LED1_OD
Text Label 3500 6450 0    50   ~ 0
LED2_OD
Text Label 3500 6550 0    50   ~ 0
LED3_OD
$Comp
L power:GND #PWR0112
U 1 1 5C0819A7
P 4100 6550
F 0 "#PWR0112" H 4100 6300 50  0001 C CNN
F 1 "GND" H 4105 6377 50  0000 C CNN
F 2 "" H 4100 6550 50  0001 C CNN
F 3 "" H 4100 6550 50  0001 C CNN
	1    4100 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 6550 3850 6550
Wire Wire Line
	3450 6450 3900 6450
Wire Wire Line
	3450 6350 3950 6350
$Comp
L power:+5V #PWR0109
U 1 1 5C1F71E9
P 4300 6500
F 0 "#PWR0109" H 4300 6350 50  0001 C CNN
F 1 "+5V" H 4315 6673 50  0000 C CNN
F 2 "" H 4300 6500 50  0001 C CNN
F 3 "" H 4300 6500 50  0001 C CNN
	1    4300 6500
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0111
U 1 1 5C1FFDF2
P 4900 6500
F 0 "#PWR0111" H 4900 6350 50  0001 C CNN
F 1 "+5V" H 4915 6673 50  0000 C CNN
F 2 "" H 4900 6500 50  0001 C CNN
F 3 "" H 4900 6500 50  0001 C CNN
	1    4900 6500
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR07
U 1 1 5C0A7548
P 3150 3950
F 0 "#PWR07" H 3150 3800 50  0001 C CNN
F 1 "+3.3V" H 3150 4090 50  0000 C CNN
F 2 "" H 3150 3950 50  0000 C CNN
F 3 "" H 3150 3950 50  0000 C CNN
	1    3150 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 6550 4900 6550
Wire Wire Line
	4900 6550 4900 6500
Wire Wire Line
	4300 6500 4300 6550
Wire Wire Line
	4300 6550 4350 6550
Wire Wire Line
	4850 6650 5200 6650
Wire Wire Line
	3950 6900 4250 6900
Wire Wire Line
	4250 6900 4250 6750
Wire Wire Line
	4250 6750 4350 6750
Wire Wire Line
	3950 6350 3950 6900
Wire Wire Line
	3900 6450 3900 6950
Wire Wire Line
	3900 6950 4300 6950
Wire Wire Line
	4300 6950 4300 6850
Wire Wire Line
	4300 6850 4350 6850
Wire Wire Line
	4350 6950 4350 7000
Wire Wire Line
	4350 7000 3850 7000
Wire Wire Line
	3850 6550 3850 7000
Wire Wire Line
	4100 6550 4200 6550
Wire Wire Line
	4200 6550 4200 6650
Wire Wire Line
	4200 6650 4350 6650
Wire Wire Line
	4850 6750 4900 6750
Wire Wire Line
	4900 6850 4850 6850
Wire Wire Line
	4850 6950 4900 6950
Wire Wire Line
	2650 6650 2600 6650
Wire Wire Line
	2600 6650 2600 6750
Wire Wire Line
	2650 6950 2600 6950
Connection ~ 2600 6950
Wire Wire Line
	2600 6950 2600 7150
Wire Wire Line
	2650 6850 2600 6850
Connection ~ 2600 6850
Wire Wire Line
	2600 6850 2600 6950
Wire Wire Line
	2650 6750 2600 6750
Wire Bus Line
	7050 2900 7050 4500
Wire Bus Line
	7050 4500 7050 5000
Wire Bus Line
	5850 4500 5850 5150
Wire Bus Line
	6400 1750 6400 3550
Connection ~ 2600 6750
Wire Wire Line
	2600 6750 2600 6850
$Comp
L power:GND #PWR?
U 1 1 5C5B1938
P 2600 7150
F 0 "#PWR?" H 2600 6900 50  0001 C CNN
F 1 "GND" H 2605 6977 50  0000 C CNN
F 2 "" H 2600 7150 50  0001 C CNN
F 3 "" H 2600 7150 50  0001 C CNN
	1    2600 7150
	1    0    0    -1  
$EndComp
$EndSCHEMATC
