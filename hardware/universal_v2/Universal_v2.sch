EESchema Schematic File Version 4
LIBS:Universal_v2-cache
EELAYER 30 0
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
L Device:R R2
U 1 1 5C5F703A
P 3350 1500
F 0 "R2" H 3420 1546 50  0000 L CNN
F 1 "4,7k" H 3420 1455 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 3280 1500 50  0001 C CNN
F 3 "~" H 3350 1500 50  0001 C CNN
	1    3350 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5C5F70CD
P 3100 1600
F 0 "R1" H 3170 1646 50  0000 L CNN
F 1 "4,7k" H 3170 1555 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 3030 1600 50  0001 C CNN
F 3 "~" H 3100 1600 50  0001 C CNN
	1    3100 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 1850 3100 1850
Wire Wire Line
	3100 1750 3100 1850
Wire Wire Line
	9500 3750 10050 3750
Wire Wire Line
	3350 1950 3350 1650
Wire Wire Line
	3350 1950 3700 1950
$Comp
L power:+3.3V #PWR0101
U 1 1 5C5F740A
P 3100 1300
F 0 "#PWR0101" H 3100 1150 50  0001 C CNN
F 1 "+3.3V" H 3115 1473 50  0000 C CNN
F 2 "" H 3100 1300 50  0001 C CNN
F 3 "" H 3100 1300 50  0001 C CNN
	1    3100 1300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0102
U 1 1 5C5F7424
P 3350 1300
F 0 "#PWR0102" H 3350 1150 50  0001 C CNN
F 1 "+3.3V" H 3365 1473 50  0000 C CNN
F 2 "" H 3350 1300 50  0001 C CNN
F 3 "" H 3350 1300 50  0001 C CNN
	1    3350 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 1300 3350 1350
Wire Wire Line
	3100 1300 3100 1450
Text Label 9500 3750 0    50   ~ 0
SDA
Text Label 9500 3650 0    50   ~ 0
SCL
Text Label 1700 4700 0    50   ~ 0
SCL
Text Label 1700 4400 0    50   ~ 0
SDA
$Comp
L power:GND #PWR0103
U 1 1 5C5F790C
P 1500 2250
F 0 "#PWR0103" H 1500 2000 50  0001 C CNN
F 1 "GND" H 1505 2077 50  0000 C CNN
F 2 "" H 1500 2250 50  0001 C CNN
F 3 "" H 1500 2250 50  0001 C CNN
	1    1500 2250
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0104
U 1 1 5C5F79F2
P 1500 1550
F 0 "#PWR0104" H 1500 1400 50  0001 C CNN
F 1 "+3.3V" H 1515 1723 50  0000 C CNN
F 2 "" H 1500 1550 50  0001 C CNN
F 3 "" H 1500 1550 50  0001 C CNN
	1    1500 1550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0105
U 1 1 5C5F7AC2
P 5250 1300
F 0 "#PWR0105" H 5250 1150 50  0001 C CNN
F 1 "+3.3V" H 5265 1473 50  0000 C CNN
F 2 "" H 5250 1300 50  0001 C CNN
F 3 "" H 5250 1300 50  0001 C CNN
	1    5250 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 1450 5250 1300
Wire Wire Line
	3650 1650 3700 1650
$Comp
L Device:C C1
U 1 1 5C5F7FF2
P 5250 1600
F 0 "C1" H 5365 1646 50  0000 L CNN
F 1 "100nF" H 5365 1555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 5288 1450 50  0001 C CNN
F 3 "~" H 5250 1600 50  0001 C CNN
	1    5250 1600
	1    0    0    -1  
$EndComp
Connection ~ 5250 1450
Wire Wire Line
	4900 2050 5050 2050
Wire Wire Line
	5050 2050 5050 2150
Wire Wire Line
	4900 2150 5050 2150
Text Label 5000 6150 2    50   ~ 0
SWDIO_EXT
Text Label 5000 6050 2    50   ~ 0
SWDCLK_EXT
$Comp
L power:GND #PWR0110
U 1 1 5C609334
P 2650 3600
F 0 "#PWR0110" H 2650 3350 50  0001 C CNN
F 1 "GND" H 2655 3427 50  0000 C CNN
F 2 "" H 2650 3600 50  0001 C CNN
F 3 "" H 2650 3600 50  0001 C CNN
	1    2650 3600
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0111
U 1 1 5C60C59C
P 3000 3200
F 0 "#PWR0111" H 3000 3050 50  0001 C CNN
F 1 "+3.3V" H 3015 3373 50  0000 C CNN
F 2 "" H 3000 3200 50  0001 C CNN
F 3 "" H 3000 3200 50  0001 C CNN
	1    3000 3200
	1    0    0    -1  
$EndComp
Text Label 1600 5950 0    50   ~ 0
IO-1
NoConn ~ 4900 1850
NoConn ~ 3700 1550
$Comp
L power:+3.3V #PWR0116
U 1 1 5C6B4B23
P 6650 3100
F 0 "#PWR0116" H 6650 2950 50  0001 C CNN
F 1 "+3.3V" H 6665 3273 50  0000 C CNN
F 2 "" H 6650 3100 50  0001 C CNN
F 3 "" H 6650 3100 50  0001 C CNN
	1    6650 3100
	-1   0    0    -1  
$EndComp
Text Label 5950 3600 0    50   ~ 0
DS18B20
Wire Wire Line
	4900 2250 5050 2250
Wire Wire Line
	3650 1650 3650 2350
Wire Wire Line
	4900 1450 5250 1450
Wire Wire Line
	5250 1750 5250 2250
$Comp
L Device:R R4
U 1 1 5C6F1E14
P 6300 3300
F 0 "R4" H 6370 3346 50  0000 L CNN
F 1 "4,7k" H 6370 3255 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 6230 3300 50  0001 C CNN
F 3 "~" H 6300 3300 50  0001 C CNN
	1    6300 3300
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6300 3150 6650 3150
Connection ~ 6650 3150
Wire Wire Line
	6650 3150 6650 3100
Wire Wire Line
	6750 3600 6300 3600
Wire Wire Line
	6300 3450 6300 3600
Connection ~ 6300 3600
Wire Wire Line
	6300 3600 5950 3600
$Comp
L SHT30-DIS-B:SHT30-DIS-B U3
U 1 1 5C5F6E3D
P 4300 1850
F 0 "U3" H 4300 2517 50  0000 C CNN
F 1 "SHT30-DIS-B" H 4300 2426 50  0000 C CNN
F 2 "SHT30-DIS-B:DFN250X250X100-9N" H 4300 1850 50  0001 L BNN
F 3 "Sensirion AG" H 4300 1850 50  0001 L BNN
F 4 "Unavailable" H 4300 1850 50  0001 L BNN "Field4"
F 5 "None" H 4300 1850 50  0001 L BNN "Field5"
F 6 "DFN-8 Sensirion" H 4300 1850 50  0001 L BNN "Field6"
F 7 "SHT30-DIS-B" H 4300 1850 50  0001 L BNN "Field7"
F 8 "SHT3x Series 5.5 V ±3 %RH 0 to 100% Temperature and Humidity Sensor - DFN-8" H 4300 1850 50  0001 L BNN "Field8"
	1    4300 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 2150 5050 2250
Connection ~ 5050 2150
Connection ~ 5050 2250
Wire Wire Line
	5050 2250 5250 2250
$Comp
L power:GND #PWR0114
U 1 1 5CB2C1A7
P 5250 2350
F 0 "#PWR0114" H 5250 2100 50  0001 C CNN
F 1 "GND" H 5255 2177 50  0000 C CNN
F 2 "" H 5250 2350 50  0001 C CNN
F 3 "" H 5250 2350 50  0001 C CNN
	1    5250 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 2250 5250 2350
Connection ~ 5250 2250
$Comp
L power:GND #PWR0115
U 1 1 5CB36B32
P 3650 2350
F 0 "#PWR0115" H 3650 2100 50  0001 C CNN
F 1 "GND" H 3655 2177 50  0000 C CNN
F 2 "" H 3650 2350 50  0001 C CNN
F 3 "" H 3650 2350 50  0001 C CNN
	1    3650 2350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0118
U 1 1 5CB396B8
P 6650 4150
F 0 "#PWR0118" H 6650 3900 50  0001 C CNN
F 1 "GND" H 6655 3977 50  0000 C CNN
F 2 "" H 6650 4150 50  0001 C CNN
F 3 "" H 6650 4150 50  0001 C CNN
	1    6650 4150
	-1   0    0    -1  
$EndComp
$Comp
L Device:CP_Small C2
U 1 1 5CB235A5
P 2650 3400
F 0 "C2" H 2350 3450 50  0000 L CNN
F 1 "10uF" H 2350 3350 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-2012-12_Kemet-R" H 2650 3400 50  0001 C CNN
F 3 "~" H 2650 3400 50  0001 C CNN
	1    2650 3400
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x04_Odd_Even J4
U 1 1 5D66D1C0
P 10250 3650
F 0 "J4" H 10300 3967 50  0000 C CNN
F 1 "Ext" H 10300 3876 50  0000 C CNN
F 2 "Connector_PinHeader_1.00mm:PinHeader_2x04_P1.00mm_Vertical_SMD" H 10250 3650 50  0001 C CNN
F 3 "~" H 10250 3650 50  0001 C CNN
	1    10250 3650
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J1
U 1 1 5D66D7F7
P 6950 3700
F 0 "J1" H 6870 3375 50  0000 C CNN
F 1 "DS18B20" H 6870 3466 50  0000 C CNN
F 2 "Pads:3Pads" H 6950 3700 50  0001 C CNN
F 3 "~" H 6950 3700 50  0001 C CNN
	1    6950 3700
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0106
U 1 1 5D67EF60
P 9750 3400
F 0 "#PWR0106" H 9750 3250 50  0001 C CNN
F 1 "+3.3V" H 9765 3573 50  0000 C CNN
F 2 "" H 9750 3400 50  0001 C CNN
F 3 "" H 9750 3400 50  0001 C CNN
	1    9750 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 5D67EF89
P 9750 4000
F 0 "#PWR0108" H 9750 3750 50  0001 C CNN
F 1 "GND" H 9755 3827 50  0000 C CNN
F 2 "" H 9750 4000 50  0001 C CNN
F 3 "" H 9750 4000 50  0001 C CNN
	1    9750 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	9750 3400 9750 3550
Wire Wire Line
	9750 3550 10050 3550
Wire Wire Line
	9750 4000 9750 3850
Wire Wire Line
	9750 3850 10050 3850
Wire Wire Line
	9500 3650 10050 3650
Wire Wire Line
	10550 3750 11050 3750
Wire Wire Line
	10550 3850 11050 3850
Text Label 11050 3750 2    50   ~ 0
ANALOG-1
Text Label 11050 3850 2    50   ~ 0
ANALOG-2
Text Label 11050 3650 2    50   ~ 0
IO-1
Text Label 11050 3550 2    50   ~ 0
IO-2
Wire Wire Line
	10550 3650 11050 3650
Wire Wire Line
	10550 3550 11050 3550
Text Label 1600 6050 0    50   ~ 0
ANALOG-1
Text Label 1600 6150 0    50   ~ 0
ANALOG-2
Wire Wire Line
	1500 1550 1500 1650
Wire Wire Line
	1500 2250 1500 2150
$Comp
L CR2032_BatHolder:Battery B1
U 1 1 5D75D3B3
P 1500 1900
F 0 "B1" V 1454 1988 50  0000 L CNN
F 1 "Battery" V 1545 1988 50  0000 L CNN
F 2 "CR2032_BatHolder:Cr2032-Holder" H 1500 1900 50  0001 C CNN
F 3 "" H 1500 1900 50  0001 C CNN
	1    1500 1900
	0    1    1    0   
$EndComp
Wire Wire Line
	3100 1850 2800 1850
Connection ~ 3100 1850
Wire Wire Line
	3350 1950 2800 1950
Connection ~ 3350 1950
Text Label 2800 1850 0    50   ~ 0
SDA
Text Label 2800 1950 0    50   ~ 0
SCL
Text Label 1600 5850 0    50   ~ 0
IO-2
Wire Wire Line
	6750 3800 6650 3800
Wire Wire Line
	6650 3800 6650 4150
Wire Wire Line
	6650 3700 6750 3700
Wire Wire Line
	6650 3150 6650 3700
$Comp
L Connector_Generic:Conn_02x03_Counter_Clockwise J2
U 1 1 5D9D5D2A
P 8150 3750
F 0 "J2" H 8200 4067 50  0000 C CNN
F 1 "DEV_PORT" H 8200 3976 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical" H 8150 3750 50  0001 C CNN
F 3 "~" H 8150 3750 50  0001 C CNN
	1    8150 3750
	1    0    0    -1  
$EndComp
Text Label 7500 3750 0    50   ~ 0
SWDCLK_EXT
Text Label 7500 3650 0    50   ~ 0
SWDIO_EXT
Wire Wire Line
	7950 3750 7500 3750
Wire Wire Line
	7950 3650 7500 3650
$Comp
L power:+3.3V #PWR0120
U 1 1 5D9E308A
P 8550 3450
F 0 "#PWR0120" H 8550 3300 50  0001 C CNN
F 1 "+3.3V" H 8565 3623 50  0000 C CNN
F 2 "" H 8550 3450 50  0001 C CNN
F 3 "" H 8550 3450 50  0001 C CNN
	1    8550 3450
	1    0    0    -1  
$EndComp
Text Label 9150 3750 2    50   ~ 0
nRESET
Wire Wire Line
	8550 3850 8450 3850
Wire Wire Line
	8550 3450 8550 3850
Wire Wire Line
	8600 3650 8450 3650
Wire Wire Line
	8450 3750 9150 3750
$Comp
L power:GND #PWR0121
U 1 1 5D9E3095
P 8600 3950
F 0 "#PWR0121" H 8600 3700 50  0001 C CNN
F 1 "GND" H 8605 3777 50  0000 C CNN
F 2 "" H 8600 3950 50  0001 C CNN
F 3 "" H 8600 3950 50  0001 C CNN
	1    8600 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 3950 8600 3650
$Comp
L HolyIot:HolyIot-18010 U1
U 1 1 5DBFF4AC
P 3550 5350
F 0 "U1" H 3575 6782 50  0000 C CNN
F 1 "HolyIot-18010" H 3575 6873 50  0000 C CNN
F 2 "HolyIot:HolyIot_18010" H 3550 6100 50  0001 C CNN
F 3 "" H 3550 6100 50  0001 C CNN
	1    3550 5350
	-1   0    0    1   
$EndComp
Wire Wire Line
	3000 3850 3000 3300
Wire Wire Line
	2650 3500 2650 3600
Wire Wire Line
	2650 3300 3000 3300
Connection ~ 3000 3300
Wire Wire Line
	3000 3300 3000 3200
$Comp
L power:GND #PWR0109
U 1 1 5DC12479
P 4700 5500
F 0 "#PWR0109" H 4700 5250 50  0001 C CNN
F 1 "GND" H 4705 5327 50  0000 C CNN
F 2 "" H 4700 5500 50  0001 C CNN
F 3 "" H 4700 5500 50  0001 C CNN
	1    4700 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 5450 4700 5450
Wire Wire Line
	4700 5450 4700 5500
$Comp
L power:GND #PWR0112
U 1 1 5DC16732
P 4650 6750
F 0 "#PWR0112" H 4650 6500 50  0001 C CNN
F 1 "GND" H 4655 6577 50  0000 C CNN
F 2 "" H 4650 6750 50  0001 C CNN
F 3 "" H 4650 6750 50  0001 C CNN
	1    4650 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 6650 4650 6650
Wire Wire Line
	4650 6650 4650 6750
$Comp
L power:GND #PWR0117
U 1 1 5DC1AC5C
P 2000 6800
F 0 "#PWR0117" H 2000 6550 50  0001 C CNN
F 1 "GND" H 2005 6627 50  0000 C CNN
F 2 "" H 2000 6800 50  0001 C CNN
F 3 "" H 2000 6800 50  0001 C CNN
	1    2000 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 6800 2000 6650
Wire Wire Line
	4450 6050 5000 6050
Wire Wire Line
	4450 6150 5000 6150
Text Label 3700 3450 3    50   ~ 0
nRESET
Wire Wire Line
	3700 3450 3700 3850
$Comp
L Sensor:BME680 U2
U 1 1 5DC4A160
P 7300 1850
F 0 "U2" H 6870 1896 50  0000 R CNN
F 1 "BME680" H 6870 1805 50  0000 R CNN
F 2 "Package_LGA:Bosch_LGA-8_3x3mm_P0.8mm_ClockwisePinNumbering" H 7300 1650 50  0001 C CNN
F 3 "https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME680-DS001-00.pdf" H 7300 1650 50  0001 C CNN
	1    7300 1850
	0    -1   -1   0   
$EndComp
NoConn ~ 7950 3850
$Comp
L power:GND #PWR0119
U 1 1 5DCA5579
P 6800 1200
F 0 "#PWR0119" H 6800 950 50  0001 C CNN
F 1 "GND" H 6805 1027 50  0000 C CNN
F 2 "" H 6800 1200 50  0001 C CNN
F 3 "" H 6800 1200 50  0001 C CNN
	1    6800 1200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0122
U 1 1 5DCAA096
P 8000 2050
F 0 "#PWR0122" H 8000 1800 50  0001 C CNN
F 1 "GND" H 8005 1877 50  0000 C CNN
F 2 "" H 8000 2050 50  0001 C CNN
F 3 "" H 8000 2050 50  0001 C CNN
	1    8000 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0123
U 1 1 5DCB39F6
P 6100 2250
F 0 "#PWR0123" H 6100 2000 50  0001 C CNN
F 1 "GND" H 6105 2077 50  0000 C CNN
F 2 "" H 6100 2250 50  0001 C CNN
F 3 "" H 6100 2250 50  0001 C CNN
	1    6100 2250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0124
U 1 1 5DCB3A1D
P 6500 2250
F 0 "#PWR0124" H 6500 2000 50  0001 C CNN
F 1 "GND" H 6505 2077 50  0000 C CNN
F 2 "" H 6500 2250 50  0001 C CNN
F 3 "" H 6500 2250 50  0001 C CNN
	1    6500 2250
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0125
U 1 1 5DCBD6A3
P 6100 1500
F 0 "#PWR0125" H 6100 1350 50  0001 C CNN
F 1 "+3.3V" H 6115 1673 50  0000 C CNN
F 2 "" H 6100 1500 50  0001 C CNN
F 3 "" H 6100 1500 50  0001 C CNN
	1    6100 1500
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0126
U 1 1 5DCBD6CA
P 6500 1500
F 0 "#PWR0126" H 6500 1350 50  0001 C CNN
F 1 "+3.3V" H 6515 1673 50  0000 C CNN
F 2 "" H 6500 1500 50  0001 C CNN
F 3 "" H 6500 1500 50  0001 C CNN
	1    6500 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 5DCBD70D
P 6500 2100
F 0 "C4" H 6650 2150 50  0000 L CNN
F 1 "100nF" H 6600 2000 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 6538 1950 50  0001 C CNN
F 3 "~" H 6500 2100 50  0001 C CNN
	1    6500 2100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5DCBD7BD
P 6100 2100
F 0 "C3" H 6215 2146 50  0000 L CNN
F 1 "100nF" H 6215 2055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 6138 1950 50  0001 C CNN
F 3 "~" H 6100 2100 50  0001 C CNN
	1    6100 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 1950 6100 1750
Wire Wire Line
	6500 1950 6700 1950
$Comp
L power:+3.3V #PWR0127
U 1 1 5DCEDEDA
P 7600 1100
F 0 "#PWR0127" H 7600 950 50  0001 C CNN
F 1 "+3.3V" H 7615 1273 50  0000 C CNN
F 2 "" H 7600 1100 50  0001 C CNN
F 3 "" H 7600 1100 50  0001 C CNN
	1    7600 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 1750 6100 1750
Connection ~ 6100 1750
Wire Wire Line
	6100 1750 6100 1500
Wire Wire Line
	6500 1500 6500 1950
Connection ~ 6500 1950
Wire Wire Line
	8000 2050 8000 1950
Wire Wire Line
	8000 1750 7900 1750
Wire Wire Line
	7900 1950 8000 1950
Connection ~ 8000 1950
Wire Wire Line
	8000 1950 8000 1750
Wire Wire Line
	7600 1100 7600 1250
Wire Wire Line
	6800 1200 6800 1150
Wire Wire Line
	6800 1150 7000 1150
Wire Wire Line
	7000 1150 7000 1250
Text Label 7200 1000 3    50   ~ 0
SCL
Wire Wire Line
	7200 1000 7200 1250
Text Label 7400 1000 3    50   ~ 0
SDA
Wire Wire Line
	7400 1000 7400 1250
Wire Wire Line
	2100 4400 1700 4400
Wire Wire Line
	2100 4700 1700 4700
Wire Wire Line
	2000 6650 2100 6650
Wire Wire Line
	2100 6050 1600 6050
Wire Wire Line
	2100 6150 1600 6150
Wire Wire Line
	1600 5950 2100 5950
Wire Wire Line
	1600 5850 2100 5850
Text Label 1700 5450 0    50   ~ 0
DS18B20
Wire Wire Line
	1700 5450 2100 5450
$EndSCHEMATC
