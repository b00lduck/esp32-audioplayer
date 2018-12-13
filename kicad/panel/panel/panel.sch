EESchema Schematic File Version 4
LIBS:panel-cache
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
L Connector:Conn_01x08_Male J1
U 1 1 5C0AE15C
P 1000 2050
F 0 "J1" H 1106 2528 50  0000 C CNN
F 1 "RFID" H 1106 2437 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 1000 2050 50  0001 C CNN
F 3 "~" H 1000 2050 50  0001 C CNN
	1    1000 2050
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J2
U 1 1 5C0AE341
P 2400 2000
F 0 "J2" V 2404 2280 50  0000 L CNN
F 1 "RFID" V 2495 2280 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x05_P2.54mm_Vertical" H 2400 2000 50  0001 C CNN
F 3 "~" H 2400 2000 50  0001 C CNN
	1    2400 2000
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J4
U 1 1 5C0AE5A2
P 3400 2000
F 0 "J4" V 3404 2280 50  0000 L CNN
F 1 "Panel" V 3495 2280 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x05_P2.54mm_Vertical" H 3400 2000 50  0001 C CNN
F 3 "~" H 3400 2000 50  0001 C CNN
	1    3400 2000
	0    1    1    0   
$EndComp
Wire Wire Line
	2600 1800 2600 1700
Wire Wire Line
	2600 1700 2500 1700
Wire Wire Line
	1900 1700 1900 2250
Wire Wire Line
	1900 2250 1200 2250
Wire Wire Line
	2400 1800 2400 1700
Connection ~ 2400 1700
Wire Wire Line
	2400 1700 1900 1700
Wire Wire Line
	2500 1800 2500 1700
Connection ~ 2500 1700
Wire Wire Line
	2500 1700 2400 1700
Wire Wire Line
	2300 1800 2300 1750
Wire Wire Line
	2300 1750 1950 1750
Wire Wire Line
	1950 1750 1950 2350
Wire Wire Line
	1950 2350 1200 2350
Wire Wire Line
	2200 2300 2200 2450
Wire Wire Line
	2200 2450 1200 2450
Wire Wire Line
	2300 2300 2300 2400
Wire Wire Line
	2300 2400 1850 2400
Wire Wire Line
	1850 2400 1850 1750
Wire Wire Line
	1850 1750 1200 1750
Wire Wire Line
	1200 1850 2100 1850
Wire Wire Line
	2100 1850 2100 2500
Wire Wire Line
	2100 2500 2600 2500
Wire Wire Line
	2600 2500 2600 2300
Wire Wire Line
	1200 1950 2050 1950
Wire Wire Line
	2050 1950 2050 2350
Wire Wire Line
	2050 2350 2500 2350
Wire Wire Line
	2500 2350 2500 2300
Wire Wire Line
	1200 2050 2000 2050
Wire Wire Line
	2000 2050 2000 2550
Wire Wire Line
	2000 2550 2400 2550
Wire Wire Line
	2400 2550 2400 2300
$Comp
L Connector:Conn_01x04_Male J3
U 1 1 5C0BB9EF
P 3000 3000
F 0 "J3" V 3153 2713 50  0000 R CNN
F 1 "btn1" V 3062 2713 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 3000 3000 50  0001 C CNN
F 3 "~" H 3000 3000 50  0001 C CNN
	1    3000 3000
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x04_Male J5
U 1 1 5C0BBAD4
P 3600 3000
F 0 "J5" V 3753 2713 50  0000 R CNN
F 1 "btn2" V 3662 2713 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 3600 3000 50  0001 C CNN
F 3 "~" H 3600 3000 50  0001 C CNN
	1    3600 3000
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x04_Male J6
U 1 1 5C0BBB14
P 4150 3000
F 0 "J6" V 4303 2713 50  0000 R CNN
F 1 "btn3" V 4212 2713 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 4150 3000 50  0001 C CNN
F 3 "~" H 4150 3000 50  0001 C CNN
	1    4150 3000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2900 2800 2900 2600
Wire Wire Line
	2900 2600 3500 2600
Wire Wire Line
	3500 2600 3500 2800
Wire Wire Line
	3500 2600 3700 2600
Wire Wire Line
	4050 2600 4050 2800
Connection ~ 3500 2600
Wire Wire Line
	3000 2800 3000 2550
Wire Wire Line
	3000 2550 3600 2550
Wire Wire Line
	4150 2550 4150 2800
Wire Wire Line
	3600 2800 3600 2550
Connection ~ 3600 2550
Wire Wire Line
	3600 2550 4150 2550
Wire Wire Line
	3600 2550 3600 2300
Wire Wire Line
	3600 1800 3600 1750
Wire Wire Line
	3600 1750 3700 1750
Wire Wire Line
	3700 1750 3700 2600
Connection ~ 3700 2600
Wire Wire Line
	3700 2600 4050 2600
Wire Wire Line
	3350 2700 3350 2450
Wire Wire Line
	3350 2450 3050 2450
Wire Wire Line
	3050 2450 3050 1650
Wire Wire Line
	3800 2700 3800 2800
Wire Wire Line
	3350 2700 3800 2700
Wire Wire Line
	4350 2800 4350 1700
Wire Wire Line
	3200 2350 3200 2300
Wire Wire Line
	3100 2800 3100 2350
Wire Wire Line
	3100 2350 3200 2350
Wire Wire Line
	3700 2800 3700 2750
Wire Wire Line
	3700 2750 3300 2750
Wire Wire Line
	4250 2800 4250 2400
Wire Wire Line
	4250 2400 3400 2400
Wire Wire Line
	3400 2400 3400 2300
Wire Wire Line
	3300 1700 3300 1800
Wire Wire Line
	3300 1700 4350 1700
Wire Wire Line
	3050 1650 3400 1650
Wire Wire Line
	3400 1650 3400 1800
Wire Wire Line
	3500 1800 3500 1600
Wire Wire Line
	3500 1600 3000 1600
Wire Wire Line
	3000 1600 3000 2500
Wire Wire Line
	3000 2500 3200 2500
Wire Wire Line
	3200 2500 3200 2800
Wire Wire Line
	3300 2300 3300 2750
$EndSCHEMATC
