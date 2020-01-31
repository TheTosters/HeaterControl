EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 8275 1800 0    50   ~ 0
SDA
Text Label 8275 1700 0    50   ~ 0
SCL
Text Label 650  5400 0    50   ~ 0
SCL
Text Label 650  5100 0    50   ~ 0
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
P 4400 775
F 0 "#PWR0105" H 4400 625 50  0001 C CNN
F 1 "+3.3V" H 4415 948 50  0000 C CNN
F 2 "" H 4400 775 50  0001 C CNN
F 3 "" H 4400 775 50  0001 C CNN
	1    4400 775 
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 925  4400 775 
Wire Wire Line
	2800 1125 2850 1125
$Comp
L Device:C C1
U 1 1 5C5F7FF2
P 4400 1075
F 0 "C1" H 4515 1121 50  0000 L CNN
F 1 "100nF" H 4515 1030 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 4438 925 50  0001 C CNN
F 3 "~" H 4400 1075 50  0001 C CNN
	1    4400 1075
	1    0    0    -1  
$EndComp
Connection ~ 4400 925 
Wire Wire Line
	4050 1525 4200 1525
Wire Wire Line
	4200 1525 4200 1625
Wire Wire Line
	4050 1625 4200 1625
Text Label 3950 6850 2    50   ~ 0
SWDIO_EXT
Text Label 3950 6750 2    50   ~ 0
SWDCLK_EXT
$Comp
L power:GND #PWR0110
U 1 1 5C609334
P 1600 4300
F 0 "#PWR0110" H 1600 4050 50  0001 C CNN
F 1 "GND" H 1605 4127 50  0000 C CNN
F 2 "" H 1600 4300 50  0001 C CNN
F 3 "" H 1600 4300 50  0001 C CNN
	1    1600 4300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0111
U 1 1 5C60C59C
P 1950 3900
F 0 "#PWR0111" H 1950 3750 50  0001 C CNN
F 1 "+3.3V" H 1965 4073 50  0000 C CNN
F 2 "" H 1950 3900 50  0001 C CNN
F 3 "" H 1950 3900 50  0001 C CNN
	1    1950 3900
	1    0    0    -1  
$EndComp
NoConn ~ 4050 1325
NoConn ~ 2850 1025
$Comp
L power:+3.3V #PWR0116
U 1 1 5C6B4B23
P 7275 925
F 0 "#PWR0116" H 7275 775 50  0001 C CNN
F 1 "+3.3V" H 7290 1098 50  0000 C CNN
F 2 "" H 7275 925 50  0001 C CNN
F 3 "" H 7275 925 50  0001 C CNN
	1    7275 925 
	-1   0    0    -1  
$EndComp
Text Label 6575 1425 0    50   ~ 0
DS18B20
Wire Wire Line
	4050 1725 4200 1725
Wire Wire Line
	2800 1125 2800 1825
Wire Wire Line
	4050 925  4400 925 
Wire Wire Line
	4400 1225 4400 1725
$Comp
L Device:R R4
U 1 1 5C6F1E14
P 6925 1125
F 0 "R4" H 6995 1171 50  0000 L CNN
F 1 "4,7k" H 6995 1080 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 6855 1125 50  0001 C CNN
F 3 "0402WGF4702TCE" H 6925 1125 50  0001 C CNN
	1    6925 1125
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6925 975  7275 975 
Connection ~ 7275 975 
Wire Wire Line
	7275 975  7275 925 
Wire Wire Line
	6925 1275 6925 1425
Wire Wire Line
	6925 1425 6575 1425
$Comp
L SHT30-DIS-B:SHT30-DIS-B U3
U 1 1 5C5F6E3D
P 3450 1325
F 0 "U3" H 3450 1992 50  0000 C CNN
F 1 "SHT30-DIS-B" H 3450 1901 50  0000 C CNN
F 2 "SHT30-DIS-B:DFN250X250X100-9N" H 3450 1325 50  0001 L BNN
F 3 "Sensirion AG" H 3450 1325 50  0001 L BNN
F 4 "Unavailable" H 3450 1325 50  0001 L BNN "Field4"
F 5 "None" H 3450 1325 50  0001 L BNN "Field5"
F 6 "DFN-8 Sensirion" H 3450 1325 50  0001 L BNN "Field6"
F 7 "SHT30-DIS-B" H 3450 1325 50  0001 L BNN "Field7"
F 8 "SHT3x Series 5.5 V ±3 %RH 0 to 100% Temperature and Humidity Sensor - DFN-8" H 3450 1325 50  0001 L BNN "Field8"
	1    3450 1325
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 1625 4200 1725
Connection ~ 4200 1625
Connection ~ 4200 1725
Wire Wire Line
	4200 1725 4400 1725
$Comp
L power:GND #PWR0114
U 1 1 5CB2C1A7
P 4400 1825
F 0 "#PWR0114" H 4400 1575 50  0001 C CNN
F 1 "GND" H 4405 1652 50  0000 C CNN
F 2 "" H 4400 1825 50  0001 C CNN
F 3 "" H 4400 1825 50  0001 C CNN
	1    4400 1825
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 1725 4400 1825
Connection ~ 4400 1725
$Comp
L power:GND #PWR0115
U 1 1 5CB36B32
P 2800 1825
F 0 "#PWR0115" H 2800 1575 50  0001 C CNN
F 1 "GND" H 2805 1652 50  0000 C CNN
F 2 "" H 2800 1825 50  0001 C CNN
F 3 "" H 2800 1825 50  0001 C CNN
	1    2800 1825
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0118
U 1 1 5CB396B8
P 7100 1975
F 0 "#PWR0118" H 7100 1725 50  0001 C CNN
F 1 "GND" H 7105 1802 50  0000 C CNN
F 2 "" H 7100 1975 50  0001 C CNN
F 3 "" H 7100 1975 50  0001 C CNN
	1    7100 1975
	-1   0    0    -1  
$EndComp
$Comp
L Device:CP_Small C2
U 1 1 5CB235A5
P 1600 4100
F 0 "C2" H 1300 4150 50  0000 L CNN
F 1 "10uF" H 1300 4050 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-2012-12_Kemet-R" H 1600 4100 50  0001 C CNN
F 3 "~" H 1600 4100 50  0001 C CNN
	1    1600 4100
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J1
U 1 1 5D66D7F7
P 7575 1525
F 0 "J1" H 7495 1200 50  0000 C CNN
F 1 "DS18B20" H 7495 1291 50  0000 C CNN
F 2 "Pads:3Pads" H 7575 1525 50  0001 C CNN
F 3 "~" H 7575 1525 50  0001 C CNN
	1    7575 1525
	1    0    0    -1  
$EndComp
Text Label 8275 1600 0    50   ~ 0
ANALOG-1
Text Label 550  6850 0    50   ~ 0
ANALOG-1
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
Text Label 1950 1325 0    50   ~ 0
SDA
Text Label 1950 1425 0    50   ~ 0
SCL
Wire Wire Line
	7375 1625 7275 1625
Wire Wire Line
	7275 975  7275 1625
Text Label 8400 775  0    50   ~ 0
SWDCLK_EXT
Text Label 8400 675  0    50   ~ 0
SWDIO_EXT
$Comp
L power:+3.3V #PWR0120
U 1 1 5D9E308A
P 8200 900
F 0 "#PWR0120" H 8200 750 50  0001 C CNN
F 1 "+3.3V" H 8215 1073 50  0000 C CNN
F 2 "" H 8200 900 50  0001 C CNN
F 3 "" H 8200 900 50  0001 C CNN
	1    8200 900 
	1    0    0    -1  
$EndComp
Text Label 8400 875  0    50   ~ 0
nRESET
$Comp
L power:GND #PWR0121
U 1 1 5D9E3095
P 8200 1125
F 0 "#PWR0121" H 8200 875 50  0001 C CNN
F 1 "GND" H 8205 952 50  0000 C CNN
F 2 "" H 8200 1125 50  0001 C CNN
F 3 "" H 8200 1125 50  0001 C CNN
	1    8200 1125
	1    0    0    -1  
$EndComp
$Comp
L HolyIot:HolyIot-18010 U1
U 1 1 5DBFF4AC
P 2500 6050
F 0 "U1" H 2525 7482 50  0000 C CNN
F 1 "HolyIot-18010" H 2525 7573 50  0000 C CNN
F 2 "HolyIot:HolyIot_18010" H 2500 6800 50  0001 C CNN
F 3 "" H 2500 6800 50  0001 C CNN
	1    2500 6050
	-1   0    0    1   
$EndComp
Wire Wire Line
	1950 4550 1950 4000
Wire Wire Line
	1600 4200 1600 4300
Wire Wire Line
	1600 4000 1950 4000
Connection ~ 1950 4000
Wire Wire Line
	1950 4000 1950 3900
$Comp
L power:GND #PWR0109
U 1 1 5DC12479
P 3975 6175
F 0 "#PWR0109" H 3975 5925 50  0001 C CNN
F 1 "GND" H 3980 6002 50  0000 C CNN
F 2 "" H 3975 6175 50  0001 C CNN
F 3 "" H 3975 6175 50  0001 C CNN
	1    3975 6175
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 6150 3975 6150
Wire Wire Line
	3975 6150 3975 6175
$Comp
L power:GND #PWR0112
U 1 1 5DC16732
P 3600 7450
F 0 "#PWR0112" H 3600 7200 50  0001 C CNN
F 1 "GND" H 3605 7277 50  0000 C CNN
F 2 "" H 3600 7450 50  0001 C CNN
F 3 "" H 3600 7450 50  0001 C CNN
	1    3600 7450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 7350 3600 7350
Wire Wire Line
	3600 7350 3600 7450
$Comp
L power:GND #PWR0117
U 1 1 5DC1AC5C
P 950 7500
F 0 "#PWR0117" H 950 7250 50  0001 C CNN
F 1 "GND" H 955 7327 50  0000 C CNN
F 2 "" H 950 7500 50  0001 C CNN
F 3 "" H 950 7500 50  0001 C CNN
	1    950  7500
	1    0    0    -1  
$EndComp
Wire Wire Line
	950  7500 950  7350
Wire Wire Line
	3400 6750 3950 6750
Wire Wire Line
	3400 6850 3950 6850
Text Label 2650 4150 3    50   ~ 0
nRESET
Wire Wire Line
	2650 4150 2650 4550
$Comp
L power:GND #PWR0119
U 1 1 5DCA5579
P 5250 825
F 0 "#PWR0119" H 5250 575 50  0001 C CNN
F 1 "GND" H 5255 652 50  0000 C CNN
F 2 "" H 5250 825 50  0001 C CNN
F 3 "" H 5250 825 50  0001 C CNN
	1    5250 825 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0122
U 1 1 5DCAA096
P 6450 1675
F 0 "#PWR0122" H 6450 1425 50  0001 C CNN
F 1 "GND" H 6455 1502 50  0000 C CNN
F 2 "" H 6450 1675 50  0001 C CNN
F 3 "" H 6450 1675 50  0001 C CNN
	1    6450 1675
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0123
U 1 1 5DCB39F6
P 4950 1925
F 0 "#PWR0123" H 4950 1675 50  0001 C CNN
F 1 "GND" H 4955 1752 50  0000 C CNN
F 2 "" H 4950 1925 50  0001 C CNN
F 3 "" H 4950 1925 50  0001 C CNN
	1    4950 1925
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0125
U 1 1 5DCBD6A3
P 4950 1025
F 0 "#PWR0125" H 4950 875 50  0001 C CNN
F 1 "+3.3V" H 4965 1198 50  0000 C CNN
F 2 "" H 4950 1025 50  0001 C CNN
F 3 "" H 4950 1025 50  0001 C CNN
	1    4950 1025
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5DCBD7BD
P 4950 1775
F 0 "C3" H 5065 1821 50  0000 L CNN
F 1 "100nF" H 5065 1730 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 4988 1625 50  0001 C CNN
F 3 "~" H 4950 1775 50  0001 C CNN
	1    4950 1775
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 1575 5150 1575
$Comp
L power:+3.3V #PWR0127
U 1 1 5DCEDEDA
P 6050 725
F 0 "#PWR0127" H 6050 575 50  0001 C CNN
F 1 "+3.3V" H 6065 898 50  0000 C CNN
F 2 "" H 6050 725 50  0001 C CNN
F 3 "" H 6050 725 50  0001 C CNN
	1    6050 725 
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 1375 4950 1375
Wire Wire Line
	6450 1675 6450 1575
Wire Wire Line
	6450 1375 6350 1375
Wire Wire Line
	6350 1575 6450 1575
Connection ~ 6450 1575
Wire Wire Line
	6450 1575 6450 1375
Wire Wire Line
	6050 725  6050 875 
Wire Wire Line
	5250 825  5250 775 
Wire Wire Line
	5250 775  5450 775 
Wire Wire Line
	5450 775  5450 875 
Text Label 5650 625  3    50   ~ 0
SCL
Wire Wire Line
	5650 625  5650 875 
Text Label 5850 625  3    50   ~ 0
SDA
Wire Wire Line
	5850 625  5850 875 
Wire Wire Line
	1050 5100 650  5100
Wire Wire Line
	1050 5400 650  5400
Wire Wire Line
	950  7350 1050 7350
Wire Wire Line
	1050 6850 550  6850
Text Label 2950 4150 3    50   ~ 0
DS18B20
$Comp
L Device:LED_BARG D1
U 1 1 5DF32085
P 10600 1400
F 0 "D1" H 10600 1897 50  0000 C CNN
F 1 "LED_BARG" H 10600 1806 50  0000 C CNN
F 2 "Everlight:Everlight_19-237_R6GHBHC-M07_2T" H 10600 1350 50  0001 C CNN
F 3 "~" H 10600 1350 50  0001 C CNN
	1    10600 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 1425 2850 1425
Wire Wire Line
	1950 1325 2850 1325
$Comp
L Device:R R1
U 1 1 5DF554B3
P 10100 1200
F 0 "R1" H 10170 1246 50  0000 L CNN
F 1 "10" H 10170 1155 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 10030 1200 50  0001 C CNN
F 3 "0402WGF100JTCE" H 10100 1200 50  0001 C CNN
	1    10100 1200
	0    1    -1   0   
$EndComp
$Comp
L Device:R R2
U 1 1 5DF56DBB
P 10100 1400
F 0 "R2" H 10170 1446 50  0000 L CNN
F 1 "10" H 10170 1355 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 10030 1400 50  0001 C CNN
F 3 "0402WGF100JTCE" H 10100 1400 50  0001 C CNN
	1    10100 1400
	0    1    -1   0   
$EndComp
$Comp
L Device:R R3
U 1 1 5DF57166
P 10100 1600
F 0 "R3" H 10170 1646 50  0000 L CNN
F 1 "10" H 10170 1555 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 10030 1600 50  0001 C CNN
F 3 "0402WGF100JTCE" H 10100 1600 50  0001 C CNN
	1    10100 1600
	0    1    -1   0   
$EndComp
Wire Wire Line
	10250 1200 10400 1200
Wire Wire Line
	10250 1400 10400 1400
Wire Wire Line
	10250 1600 10400 1600
Wire Wire Line
	10800 1400 11025 1400
$Comp
L power:+3.3V #PWR0101
U 1 1 5DF6622A
P 11025 1275
F 0 "#PWR0101" H 11025 1125 50  0001 C CNN
F 1 "+3.3V" H 11040 1448 50  0000 C CNN
F 2 "" H 11025 1275 50  0001 C CNN
F 3 "" H 11025 1275 50  0001 C CNN
	1    11025 1275
	1    0    0    -1  
$EndComp
Wire Wire Line
	11025 1275 11025 1400
Wire Wire Line
	9950 1200 9550 1200
Wire Wire Line
	9950 1400 9550 1400
Wire Wire Line
	9950 1600 9550 1600
Text Label 9550 1200 0    50   ~ 0
LED_R
Text Label 9550 1400 0    50   ~ 0
LED_G
Text Label 9550 1600 0    50   ~ 0
LED_B
Text Label 2750 4150 3    50   ~ 0
LED_R
Text Label 2150 4150 3    50   ~ 0
LED_G
Text Label 2850 4150 3    50   ~ 0
LED_B
Wire Wire Line
	2750 4550 2750 4150
Wire Wire Line
	2850 4150 2850 4550
Wire Wire Line
	2150 4150 2150 4550
$Comp
L Connector:Conn_01x05_Female J3
U 1 1 5E022212
P 8900 1700
F 0 "J3" H 8928 1726 50  0000 L CNN
F 1 "EXT_PORT" H 8928 1635 50  0000 L CNN
F 2 "Pads:5xPad" H 8900 1700 50  0001 C CNN
F 3 "~" H 8900 1700 50  0001 C CNN
	1    8900 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8275 1600 8700 1600
Wire Wire Line
	8275 1700 8700 1700
$Comp
L power:+3.3V #PWR0102
U 1 1 5E033FB9
P 8125 1625
F 0 "#PWR0102" H 8125 1475 50  0001 C CNN
F 1 "+3.3V" H 8140 1798 50  0000 C CNN
F 2 "" H 8125 1625 50  0001 C CNN
F 3 "" H 8125 1625 50  0001 C CNN
	1    8125 1625
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 5E03443E
P 8200 2025
F 0 "#PWR0106" H 8200 1775 50  0001 C CNN
F 1 "GND" H 8205 1852 50  0000 C CNN
F 2 "" H 8200 2025 50  0001 C CNN
F 3 "" H 8200 2025 50  0001 C CNN
	1    8200 2025
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 1900 8125 1900
$Comp
L Connector:Conn_01x05_Female J2
U 1 1 5E03DCED
P 9150 875
F 0 "J2" H 9178 901 50  0000 L CNN
F 1 "PROG_PORT" H 9178 810 50  0000 L CNN
F 2 "Pads:5xPad" H 9150 875 50  0001 C CNN
F 3 "~" H 9150 875 50  0001 C CNN
	1    9150 875 
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 675  8400 675 
Wire Wire Line
	8400 775  8950 775 
Wire Wire Line
	8400 875  8950 875 
Wire Wire Line
	8200 900  8200 975 
Wire Wire Line
	8200 975  8950 975 
Wire Wire Line
	8200 1125 8200 1075
Wire Wire Line
	8200 1075 8950 1075
Wire Wire Line
	2950 4550 2950 4150
Wire Wire Line
	7375 1425 7100 1425
Wire Wire Line
	7100 1425 7100 1975
Wire Wire Line
	6925 1425 6925 1525
Wire Wire Line
	6925 1525 7375 1525
Connection ~ 6925 1425
Wire Wire Line
	8125 1625 8125 1900
Text Label 650  6550 0    50   ~ 0
SCL
Text Label 650  6450 0    50   ~ 0
SDA
Wire Wire Line
	1050 6550 650  6550
Wire Wire Line
	1050 6450 650  6450
Wire Wire Line
	8200 2025 8200 1500
Wire Wire Line
	8200 1500 8700 1500
Wire Wire Line
	8275 1800 8700 1800
Text Label 3700 6450 2    50   ~ 0
nRESET
Wire Wire Line
	3700 6450 3400 6450
$Comp
L Sensor:BME680 U2
U 1 1 5DC4A160
P 5750 1475
F 0 "U2" H 5320 1521 50  0000 R CNN
F 1 "BME680" H 5320 1430 50  0000 R CNN
F 2 "Package_LGA:Bosch_LGA-8_3x3mm_P0.8mm_ClockwisePinNumbering" H 5750 1275 50  0001 C CNN
F 3 "https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME680-DS001-00.pdf" H 5750 1275 50  0001 C CNN
	1    5750 1475
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4950 1575 4950 1375
Connection ~ 4950 1375
Wire Wire Line
	4950 1625 4950 1575
Connection ~ 4950 1575
Wire Wire Line
	4950 1025 4950 1375
$Sheet
S 5300 3600 2875 1950
U 5DF39E1A
F0 "Laird" 50
F1 "Laird_schematic.sch" 50
$EndSheet
$EndSCHEMATC