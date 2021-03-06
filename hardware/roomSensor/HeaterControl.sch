EESchema Schematic File Version 4
LIBS:HeaterControl-cache
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
L Laird:BL654 U2
U 1 1 5C5F6F2C
P 4100 4750
F 0 "U2" H 4600 4450 50  0000 L CNN
F 1 "BL654" H 4500 4650 150 0000 L CNN
F 2 "Laird:BL654" H 4200 4700 50  0001 C CNN
F 3 "" H 4200 4700 50  0001 C CNN
	1    4100 4750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5C5F703A
P 3350 1500
F 0 "R2" H 3420 1546 50  0000 L CNN
F 1 "4,7k" H 3420 1455 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3280 1500 50  0001 C CNN
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
F 2 "Resistor_SMD:R_1206_3216Metric" V 3030 1600 50  0001 C CNN
F 3 "~" H 3100 1600 50  0001 C CNN
	1    3100 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 1850 3100 1850
Wire Wire Line
	3100 1750 3100 1850
Connection ~ 3100 1850
Wire Wire Line
	3100 1850 2800 1850
Wire Wire Line
	2800 1950 3350 1950
Wire Wire Line
	3350 1950 3350 1650
Connection ~ 3350 1950
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
Text Label 2800 1850 0    50   ~ 0
SDA
Text Label 2800 1950 0    50   ~ 0
SCL
Wire Wire Line
	1950 4350 1400 4350
Wire Wire Line
	1950 4550 1400 4550
Text Label 1400 4350 0    50   ~ 0
SCL
Text Label 1400 4550 0    50   ~ 0
SDA
$Comp
L power:GND #PWR0103
U 1 1 5C5F790C
P 7150 4150
F 0 "#PWR0103" H 7150 3900 50  0001 C CNN
F 1 "GND" H 7155 3977 50  0000 C CNN
F 2 "" H 7150 4150 50  0001 C CNN
F 3 "" H 7150 4150 50  0001 C CNN
	1    7150 4150
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0104
U 1 1 5C5F79F2
P 7150 3150
F 0 "#PWR0104" H 7150 3000 50  0001 C CNN
F 1 "+3.3V" H 7165 3323 50  0000 C CNN
F 2 "" H 7150 3150 50  0001 C CNN
F 3 "" H 7150 3150 50  0001 C CNN
	1    7150 3150
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
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5288 1450 50  0001 C CNN
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
$Comp
L power:+3.3V #PWR0107
U 1 1 5C600127
P 8550 4650
F 0 "#PWR0107" H 8550 4500 50  0001 C CNN
F 1 "+3.3V" H 8565 4823 50  0000 C CNN
F 2 "" H 8550 4650 50  0001 C CNN
F 3 "" H 8550 4650 50  0001 C CNN
	1    8550 4650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 5C60040B
P 8700 5650
F 0 "#PWR0108" H 8700 5400 50  0001 C CNN
F 1 "GND" H 8705 5477 50  0000 C CNN
F 2 "" H 8700 5650 50  0001 C CNN
F 3 "" H 8700 5650 50  0001 C CNN
	1    8700 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 6150 3300 6600
Text Label 3300 6600 1    50   ~ 0
nRESET
Text Label 9150 4950 2    50   ~ 0
nRESET
Wire Wire Line
	8450 4950 8800 4950
Text Label 7100 4950 0    50   ~ 0
SWDCLK_EXT
Text Label 7100 5050 0    50   ~ 0
SWDIO_EXT
Text Label 7100 4850 0    50   ~ 0
SWO_EXT
Text Label 5100 6700 1    50   ~ 0
SWDIO_EXT
Wire Wire Line
	5100 6150 5100 6700
Text Label 4900 6700 1    50   ~ 0
SWDCLK_EXT
Wire Wire Line
	4900 6150 4900 6700
Wire Wire Line
	4500 6150 4500 6700
Text Label 4500 6700 1    50   ~ 0
SWO_EXT
$Comp
L power:GND #PWR0109
U 1 1 5C606176
P 2900 3400
F 0 "#PWR0109" H 2900 3150 50  0001 C CNN
F 1 "GND" H 2905 3227 50  0000 C CNN
F 2 "" H 2900 3400 50  0001 C CNN
F 3 "" H 2900 3400 50  0001 C CNN
	1    2900 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 3400 2900 3350
Wire Wire Line
	2900 3350 3050 3350
Wire Wire Line
	3050 3350 3050 3650
Wire Wire Line
	3450 3650 3450 3350
Wire Wire Line
	3450 3350 3050 3350
Connection ~ 3050 3350
Wire Wire Line
	3950 3650 3950 3350
Wire Wire Line
	3950 3350 3450 3350
Connection ~ 3450 3350
Wire Wire Line
	4250 3650 4250 3350
Wire Wire Line
	4250 3350 3950 3350
Connection ~ 3950 3350
Wire Wire Line
	4550 3650 4550 3350
Wire Wire Line
	4550 3350 4250 3350
Connection ~ 4250 3350
Wire Wire Line
	5250 3650 5250 3350
Wire Wire Line
	5250 3350 4550 3350
Connection ~ 4550 3350
$Comp
L power:GND #PWR0110
U 1 1 5C609334
P 1800 6000
F 0 "#PWR0110" H 1800 5750 50  0001 C CNN
F 1 "GND" H 1805 5827 50  0000 C CNN
F 2 "" H 1800 6000 50  0001 C CNN
F 3 "" H 1800 6000 50  0001 C CNN
	1    1800 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 4150 1800 4150
Wire Wire Line
	1800 4150 1800 4850
Wire Wire Line
	1950 5850 1800 5850
Connection ~ 1800 5850
Wire Wire Line
	1800 5850 1800 6000
Wire Wire Line
	1950 5550 1800 5550
Connection ~ 1800 5550
Wire Wire Line
	1800 5550 1800 5850
Wire Wire Line
	1950 4850 1800 4850
Connection ~ 1800 4850
Wire Wire Line
	1800 4850 1800 5550
$Comp
L power:+3.3V #PWR0111
U 1 1 5C60C59C
P 1550 5000
F 0 "#PWR0111" H 1550 4850 50  0001 C CNN
F 1 "+3.3V" H 1565 5173 50  0000 C CNN
F 2 "" H 1550 5000 50  0001 C CNN
F 3 "" H 1550 5000 50  0001 C CNN
	1    1550 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 5000 1550 5050
Wire Wire Line
	1550 5050 1950 5050
$Comp
L power:GND #PWR0112
U 1 1 5C60D612
P 6150 6300
F 0 "#PWR0112" H 6150 6050 50  0001 C CNN
F 1 "GND" H 6155 6127 50  0000 C CNN
F 2 "" H 6150 6300 50  0001 C CNN
F 3 "" H 6150 6300 50  0001 C CNN
	1    6150 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 5650 6150 5650
Wire Wire Line
	6150 5650 6150 5750
Wire Wire Line
	5200 6150 5200 6200
Wire Wire Line
	5200 6200 5400 6200
Connection ~ 6150 6200
Wire Wire Line
	6150 6300 6150 6200
Wire Wire Line
	5400 6150 5400 6200
Connection ~ 5400 6200
Wire Wire Line
	5400 6200 5550 6200
Wire Wire Line
	5550 6150 5550 6200
Connection ~ 5550 6200
Wire Wire Line
	5550 6200 5700 6200
Wire Wire Line
	5700 6150 5700 6200
Connection ~ 5700 6200
Wire Wire Line
	5700 6200 6150 6200
Wire Wire Line
	6100 5850 6150 5850
Connection ~ 6150 5850
Wire Wire Line
	6150 5850 6150 6200
Wire Wire Line
	6100 5750 6150 5750
Connection ~ 6150 5750
Wire Wire Line
	6150 5750 6150 5850
Text Label 8700 3850 0    50   ~ 0
UART_TX
Text Label 1400 4650 0    50   ~ 0
UART_TX
Wire Wire Line
	1400 4650 1950 4650
$Comp
L Switch:SW_DIP_x01 SW1
U 1 1 5C623472
P 8800 5250
F 0 "SW1" V 8754 5380 50  0000 L CNN
F 1 "RESET" V 8845 5380 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" H 8800 5250 50  0001 C CNN
F 3 "" H 8800 5250 50  0001 C CNN
	1    8800 5250
	0    1    1    0   
$EndComp
NoConn ~ 5000 6150
NoConn ~ 4800 6150
NoConn ~ 4700 6150
NoConn ~ 4600 6150
NoConn ~ 4400 6150
NoConn ~ 4200 6150
NoConn ~ 4100 6150
NoConn ~ 4000 6150
NoConn ~ 3900 6150
NoConn ~ 3800 6150
NoConn ~ 3700 6150
NoConn ~ 3600 6150
NoConn ~ 3500 6150
NoConn ~ 3400 6150
NoConn ~ 3200 6150
NoConn ~ 1950 5750
NoConn ~ 1950 5250
NoConn ~ 1950 5150
NoConn ~ 1950 4950
NoConn ~ 1950 4750
NoConn ~ 1950 4450
NoConn ~ 1950 4250
NoConn ~ 3150 3650
NoConn ~ 3250 3650
NoConn ~ 3350 3650
NoConn ~ 3550 3650
NoConn ~ 3650 3650
NoConn ~ 3750 3650
NoConn ~ 4150 3650
NoConn ~ 4350 3650
NoConn ~ 4450 3650
NoConn ~ 4650 3650
NoConn ~ 4750 3650
NoConn ~ 4950 3650
NoConn ~ 5050 3650
NoConn ~ 5150 3650
NoConn ~ 4900 1850
NoConn ~ 3700 1550
$Comp
L Sensor_Temperature:DS18B20 U1
U 1 1 5C6AC06A
P 1800 1850
F 0 "U1" H 1570 1896 50  0000 R CNN
F 1 "DS18B20" H 1570 1805 50  0000 R CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 800 1600 50  0001 C CNN
F 3 "http://datasheets.maximintegrated.com/en/ds/DS18B20.pdf" H 1650 2100 50  0001 C CNN
	1    1800 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 1550 1800 1400
$Comp
L power:+3.3V #PWR0116
U 1 1 5C6B4B23
P 1800 1350
F 0 "#PWR0116" H 1800 1200 50  0001 C CNN
F 1 "+3.3V" H 1815 1523 50  0000 C CNN
F 2 "" H 1800 1350 50  0001 C CNN
F 3 "" H 1800 1350 50  0001 C CNN
	1    1800 1350
	1    0    0    -1  
$EndComp
Text Label 1400 4050 0    50   ~ 0
DS18B20
Text Label 2500 1850 2    50   ~ 0
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
L BS-3:BS-3 U4
U 1 1 5C6E1F18
P 7150 4050
F 0 "U4" V 7596 3922 50  0000 R CNN
F 1 "Coin Battery" V 7505 3922 50  0000 R CNN
F 2 "BS-3:BS3" H 7800 4150 50  0001 L CNN
F 3 "http://www.memoryprotectiondevices.com/datasheets/BS-3-datasheet.pdf" H 7800 4050 50  0001 L CNN
F 4 "BATTERY HOLDER COIN 20MM PC PIN" H 7800 3950 50  0001 L CNN "Description"
F 5 "6.75" H 7800 3850 50  0001 L CNN "Height"
F 6 "MPD (Memory Protection Devices)" H 7800 3750 50  0001 L CNN "Manufacturer_Name"
F 7 "BS-3" H 7800 3650 50  0001 L CNN "Manufacturer_Part_Number"
	1    7150 4050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7150 4050 7150 4150
Wire Wire Line
	7150 3150 7150 3250
$Comp
L Device:R R4
U 1 1 5C6F1E14
P 2150 1550
F 0 "R4" H 2220 1596 50  0000 L CNN
F 1 "4,7k" H 2220 1505 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 2080 1550 50  0001 C CNN
F 3 "~" H 2150 1550 50  0001 C CNN
	1    2150 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 1400 1800 1400
Connection ~ 1800 1400
Wire Wire Line
	1800 1400 1800 1350
Wire Wire Line
	2100 1850 2150 1850
Wire Wire Line
	2150 1700 2150 1850
Connection ~ 2150 1850
Wire Wire Line
	2150 1850 2500 1850
NoConn ~ 3000 6150
NoConn ~ 3100 6150
NoConn ~ 1950 5450
Wire Wire Line
	1950 4050 1400 4050
NoConn ~ 1950 5350
$Comp
L Tag-Connect:TC2030-CTX P1
U 1 1 5C982CF8
P 8000 4950
F 0 "P1" H 8000 4658 40  0000 C CNN
F 1 "TC2030-CTX" H 8000 4734 40  0000 C CNN
F 2 "Tag-Connect:TC2030-NL_SMALL" H 8000 4950 50  0001 C CNN
F 3 "" H 8000 4950 50  0001 C CNN
	1    8000 4950
	-1   0    0    1   
$EndComp
Wire Wire Line
	8550 5050 8450 5050
Wire Wire Line
	8550 4650 8550 5050
Wire Wire Line
	8600 4850 8450 4850
Wire Wire Line
	8800 4950 9150 4950
Connection ~ 8800 4950
Wire Wire Line
	8600 5600 8700 5600
Wire Wire Line
	8800 5600 8800 5550
Wire Wire Line
	8600 4850 8600 5600
Wire Wire Line
	8700 5600 8700 5650
Connection ~ 8700 5600
Wire Wire Line
	8700 5600 8800 5600
Wire Wire Line
	7550 5050 7100 5050
Wire Wire Line
	7550 4950 7100 4950
Wire Wire Line
	7550 4850 7100 4850
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
L power:+3.3V #PWR0106
U 1 1 5C9E7EE0
P 10700 3750
F 0 "#PWR0106" H 10700 3600 50  0001 C CNN
F 1 "+3.3V" H 10715 3923 50  0000 C CNN
F 2 "" H 10700 3750 50  0001 C CNN
F 3 "" H 10700 3750 50  0001 C CNN
	1    10700 3750
	1    0    0    -1  
$EndComp
Text Label 10900 4050 2    50   ~ 0
SDA
Text Label 9700 4050 0    50   ~ 0
SCL
Text Label 8800 3300 0    50   ~ 0
BTN-1
Text Label 10000 3300 2    50   ~ 0
BTN-2
Text Label 10000 3200 2    50   ~ 0
BTN-3
Wire Wire Line
	4850 3650 4850 3050
Wire Wire Line
	4050 3650 4050 3050
Wire Wire Line
	3850 3650 3850 3050
Text Label 3850 3050 3    50   ~ 0
BTN-1
Text Label 4050 3050 3    50   ~ 0
BTN-2
Text Label 4850 3050 3    50   ~ 0
BTN-3
$Comp
L Connector_Generic:Conn_02x02_Odd_Even J2
U 1 1 5CA6610E
P 9400 3200
F 0 "J2" H 9450 3417 50  0000 C CNN
F 1 "BTN" H 9450 3326 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x02_P2.54mm_Vertical" H 9400 3200 50  0001 C CNN
F 3 "~" H 9400 3200 50  0001 C CNN
	1    9400 3200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0113
U 1 1 5CA66342
P 9100 3400
F 0 "#PWR0113" H 9100 3150 50  0001 C CNN
F 1 "GND" H 9105 3227 50  0000 C CNN
F 2 "" H 9100 3400 50  0001 C CNN
F 3 "" H 9100 3400 50  0001 C CNN
	1    9100 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 3400 9100 3200
Wire Wire Line
	9100 3200 9200 3200
Wire Wire Line
	8800 3300 9200 3300
Wire Wire Line
	9700 3200 10000 3200
Wire Wire Line
	9700 3300 10000 3300
$Comp
L Connector:Conn_01x02_Male J1
U 1 1 5CA762C8
P 9250 3950
F 0 "J1" H 9222 3830 50  0000 R CNN
F 1 "UART" H 9222 3921 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9250 3950 50  0001 C CNN
F 3 "~" H 9250 3950 50  0001 C CNN
	1    9250 3950
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0117
U 1 1 5CA7E417
P 8950 4050
F 0 "#PWR0117" H 8950 3800 50  0001 C CNN
F 1 "GND" H 8955 3877 50  0000 C CNN
F 2 "" H 8950 4050 50  0001 C CNN
F 3 "" H 8950 4050 50  0001 C CNN
	1    8950 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 4050 8950 3950
Wire Wire Line
	8950 3950 9050 3950
Wire Wire Line
	9050 3850 8700 3850
$Comp
L Connector_Generic:Conn_02x02_Odd_Even J3
U 1 1 5CA86945
P 10350 3950
F 0 "J3" H 10400 4167 50  0000 C CNN
F 1 "OLED" H 10400 4076 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x02_P2.54mm_Vertical" H 10350 3950 50  0001 C CNN
F 3 "~" H 10350 3950 50  0001 C CNN
	1    10350 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	10900 4050 10650 4050
Wire Wire Line
	10650 3950 10700 3950
Wire Wire Line
	10700 3950 10700 3750
Wire Wire Line
	9700 4050 10150 4050
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
P 1800 2400
F 0 "#PWR0118" H 1800 2150 50  0001 C CNN
F 1 "GND" H 1805 2227 50  0000 C CNN
F 2 "" H 1800 2400 50  0001 C CNN
F 3 "" H 1800 2400 50  0001 C CNN
	1    1800 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 2400 1800 2150
$Comp
L power:GND #PWR0119
U 1 1 5CB51F1E
P 10100 4150
F 0 "#PWR0119" H 10100 3900 50  0001 C CNN
F 1 "GND" H 10105 3977 50  0000 C CNN
F 2 "" H 10100 4150 50  0001 C CNN
F 3 "" H 10100 4150 50  0001 C CNN
	1    10100 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	10100 4150 10100 3950
Wire Wire Line
	10100 3950 10150 3950
NoConn ~ 1950 3950
NoConn ~ 4300 6150
Wire Wire Line
	1550 5850 1800 5850
$Comp
L Device:CP_Small C2
U 1 1 5CB235A5
P 1550 5350
F 0 "C2" H 1250 5400 50  0000 L CNN
F 1 "10uF" H 1250 5300 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-2012-12_Kemet-R" H 1550 5350 50  0001 C CNN
F 3 "~" H 1550 5350 50  0001 C CNN
	1    1550 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 5250 1550 5050
Wire Wire Line
	1550 5450 1550 5850
Connection ~ 1550 5050
NoConn ~ 1950 5650
$EndSCHEMATC
