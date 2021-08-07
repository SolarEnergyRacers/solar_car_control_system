EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Test Steering Wheel"
Date "2021-05-02"
Rev "0.1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RF_Module:ESP32-WROOM-32 U3
U 1 1 608FA806
P 9400 2550
F 0 "U3" H 9400 4131 50  0000 C CNN
F 1 "ESP32-WROOM-32" H 9400 4040 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 9400 1050 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf" H 9100 2600 50  0001 C CNN
	1    9400 2550
	1    0    0    -1  
$EndComp
$Comp
L Analog_ADC:ADS1115IDGS ADC2
U 1 1 608FCD52
P 2900 6000
F 0 "ADC2" H 2900 6681 50  0000 C CNN
F 1 "ADS1115IDGS" H 2900 6590 50  0000 C CNN
F 2 "Package_SO:TSSOP-10_3x3mm_P0.5mm" H 2900 5500 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ads1113.pdf" H 2850 5100 50  0001 C CNN
	1    2900 6000
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 60900BCE
P 7200 5450
F 0 "R2" V 6993 5450 50  0000 C CNN
F 1 "10k" V 7084 5450 50  0000 C CNN
F 2 "" V 7130 5450 50  0001 C CNN
F 3 "~" H 7200 5450 50  0001 C CNN
	1    7200 5450
	0    1    1    0   
$EndComp
$Comp
L power:VCC #PWR0101
U 1 1 60902293
P 2250 5450
F 0 "#PWR0101" H 2250 5300 50  0001 C CNN
F 1 "VCC" H 2265 5623 50  0000 C CNN
F 2 "" H 2250 5450 50  0001 C CNN
F 3 "" H 2250 5450 50  0001 C CNN
	1    2250 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 5100 7500 5100
Wire Wire Line
	7350 5450 7500 5450
$Comp
L Connector:DB15_Male J1
U 1 1 6090E457
P 7350 2150
F 0 "J1" H 7505 2196 50  0000 L CNN
F 1 "DB15_Male" H 7505 2105 50  0000 L CNN
F 2 "" H 7350 2150 50  0001 C CNN
F 3 " ~" H 7350 2150 50  0001 C CNN
	1    7350 2150
	1    0    0    -1  
$EndComp
$Comp
L Connector:DB15_Female_HighDensity J2
U 1 1 6090C7A3
P 8700 5850
F 0 "J2" H 8700 6717 50  0000 C CNN
F 1 "DB15_Female_HighDensity" H 8700 6626 50  0000 C CNN
F 2 "" H 7750 6250 50  0001 C CNN
F 3 " ~" H 7750 6250 50  0001 C CNN
	1    8700 5850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 60926C75
P 5750 6600
F 0 "#PWR0103" H 5750 6350 50  0001 C CNN
F 1 "GND" H 5755 6427 50  0000 C CNN
F 2 "" H 5750 6600 50  0001 C CNN
F 3 "" H 5750 6600 50  0001 C CNN
	1    5750 6600
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0104
U 1 1 60927562
P 5100 5300
F 0 "#PWR0104" H 5100 5150 50  0001 C CNN
F 1 "VCC" H 5115 5473 50  0000 C CNN
F 2 "" H 5100 5300 50  0001 C CNN
F 3 "" H 5100 5300 50  0001 C CNN
	1    5100 5300
	1    0    0    -1  
$EndComp
NoConn ~ 6550 6100
NoConn ~ 4950 5900
NoConn ~ 4950 5800
NoConn ~ 4950 5700
NoConn ~ 4950 5600
NoConn ~ 4950 5500
Wire Wire Line
	5750 6500 5750 6600
$Comp
L Driver_Display:CR2013-MI2120 Display0
U 1 1 608FF9B9
P 5750 5900
F 0 "Display0" H 5750 6681 50  0000 C CNN
F 1 "CR2013-MI2120" H 5750 6590 50  0000 C CNN
F 2 "Display:CR2013-MI2120" H 5750 5200 50  0001 C CNN
F 3 "http://pan.baidu.com/s/11Y990" H 5100 6400 50  0001 C CNN
	1    5750 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 609011A2
P 7200 5100
F 0 "R1" V 6993 5100 50  0000 C CNN
F 1 "10k" V 7084 5100 50  0000 C CNN
F 2 "" V 7130 5100 50  0001 C CNN
F 3 "~" H 7200 5100 50  0001 C CNN
	1    7200 5100
	0    1    1    0   
$EndComp
Wire Wire Line
	7500 5100 7500 5450
Wire Wire Line
	6550 5700 6900 5700
Wire Wire Line
	6550 5600 7050 5600
Wire Wire Line
	7050 5450 7050 5600
Connection ~ 7050 5600
Wire Wire Line
	7050 5100 6900 5100
Wire Wire Line
	6900 5100 6900 5700
$Comp
L power:GND #PWR0106
U 1 1 610D31F6
P 2900 6400
F 0 "#PWR0106" H 2900 6150 50  0001 C CNN
F 1 "GND" H 2905 6227 50  0000 C CNN
F 2 "" H 2900 6400 50  0001 C CNN
F 3 "" H 2900 6400 50  0001 C CNN
	1    2900 6400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2250 5450 2250 5500
Wire Wire Line
	2250 5500 2900 5500
Connection ~ 4950 2400
Wire Wire Line
	4300 1850 4300 1750
Connection ~ 4950 950 
Wire Wire Line
	4850 950  4850 1100
Wire Wire Line
	4950 950  4850 950 
Wire Wire Line
	4300 1300 4300 1400
Connection ~ 4300 950 
Wire Wire Line
	4300 850  4300 950 
Connection ~ 4700 2300
Wire Wire Line
	5350 2500 5750 2500
Connection ~ 5350 2500
Wire Wire Line
	4700 2500 4700 2300
Wire Wire Line
	5350 2500 4700 2500
Wire Wire Line
	4950 2400 4950 2300
Wire Wire Line
	4950 1850 4950 1950
Wire Wire Line
	4950 1450 4950 1350
Wire Wire Line
	4950 950  4950 850 
Wire Wire Line
	4300 2200 4300 2300
$Comp
L Switch:SW_MEC_5E SW12
U 1 1 6109EDA6
P 5150 2400
F 0 "SW12" H 5150 2785 50  0000 C CNN
F 1 "SW_MEC_5E" H 5150 2694 50  0000 C CNN
F 2 "" H 5150 2700 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 5150 2700 50  0001 C CNN
	1    5150 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 2300 5350 1950
Wire Wire Line
	5350 2400 5350 2300
Connection ~ 5350 2300
Wire Wire Line
	5350 2400 5350 2500
Connection ~ 5350 2400
Connection ~ 5350 1950
Connection ~ 4700 2200
Connection ~ 4700 1850
Wire Wire Line
	4700 1850 4700 2200
Wire Wire Line
	5350 1850 5350 1950
Connection ~ 5350 1850
Wire Wire Line
	5350 1450 5350 1850
Connection ~ 5350 1450
Wire Wire Line
	5350 1350 5350 1450
Connection ~ 5350 1350
Wire Wire Line
	5350 950  5350 1350
Connection ~ 5350 950 
Wire Wire Line
	5350 850  5350 950 
Wire Wire Line
	4700 2200 4700 2300
Wire Wire Line
	4700 1750 4700 1850
Connection ~ 4700 1750
Wire Wire Line
	4700 1400 4700 1750
Connection ~ 4700 1400
Wire Wire Line
	4700 1300 4700 1400
Connection ~ 4700 1300
Wire Wire Line
	4700 950  4700 1300
Connection ~ 4700 950 
Wire Wire Line
	4700 850  4700 950 
$Comp
L power:GND #PWR0107
U 1 1 610EDD24
P 5750 2500
F 0 "#PWR0107" H 5750 2250 50  0001 C CNN
F 1 "GND" H 5755 2327 50  0000 C CNN
F 2 "" H 5750 2500 50  0001 C CNN
F 3 "" H 5750 2500 50  0001 C CNN
	1    5750 2500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW11
U 1 1 6109ED9C
P 5150 1950
F 0 "SW11" H 5150 2335 50  0000 C CNN
F 1 "SW_MEC_5E" H 5150 2244 50  0000 C CNN
F 2 "" H 5150 2250 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 5150 2250 50  0001 C CNN
	1    5150 1950
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW10
U 1 1 6109ED92
P 5150 1450
F 0 "SW10" H 5150 1835 50  0000 C CNN
F 1 "SW_MEC_5E" H 5150 1744 50  0000 C CNN
F 2 "" H 5150 1750 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 5150 1750 50  0001 C CNN
	1    5150 1450
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW9
U 1 1 6109EAC6
P 5150 950
F 0 "SW9" H 5150 1335 50  0000 C CNN
F 1 "SW_MEC_5E" H 5150 1244 50  0000 C CNN
F 2 "" H 5150 1250 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 5150 1250 50  0001 C CNN
	1    5150 950 
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW4
U 1 1 61115A87
P 4500 2300
F 0 "SW4" H 4500 2685 50  0000 C CNN
F 1 "SW_MEC_5E" H 4500 2594 50  0000 C CNN
F 2 "" H 4500 2600 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4500 2600 50  0001 C CNN
	1    4500 2300
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW3
U 1 1 61100AD7
P 4500 1850
F 0 "SW3" H 4500 2235 50  0000 C CNN
F 1 "SW_MEC_5E" H 4500 2144 50  0000 C CNN
F 2 "" H 4500 2150 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4500 2150 50  0001 C CNN
	1    4500 1850
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW2
U 1 1 610FFF22
P 4500 1400
F 0 "SW2" H 4500 1785 50  0000 C CNN
F 1 "SW_MEC_5E" H 4500 1694 50  0000 C CNN
F 2 "" H 4500 1700 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4500 1700 50  0001 C CNN
	1    4500 1400
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW1
U 1 1 610FF600
P 4500 950
F 0 "SW1" H 4500 1335 50  0000 C CNN
F 1 "SW_MEC_5E" H 4500 1244 50  0000 C CNN
F 2 "" H 4500 1250 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4500 1250 50  0001 C CNN
	1    4500 950 
	1    0    0    -1  
$EndComp
Connection ~ 4950 4450
Connection ~ 4300 4250
Connection ~ 4950 4000
Wire Wire Line
	4300 3900 4300 3800
Connection ~ 4950 3500
Connection ~ 4950 3000
Wire Wire Line
	4850 3000 4850 3200
Wire Wire Line
	4950 3000 4850 3000
Wire Wire Line
	4300 3350 4300 3400
Connection ~ 4300 3000
Wire Wire Line
	4300 2900 4300 3000
Connection ~ 4700 4350
Wire Wire Line
	4700 4550 4700 4350
Wire Wire Line
	5350 4550 4700 4550
Wire Wire Line
	4950 4450 4950 4350
Wire Wire Line
	4950 3900 4950 4000
Wire Wire Line
	4950 3500 4950 3400
Wire Wire Line
	4950 3000 4950 2900
Wire Wire Line
	4300 4250 4300 4350
$Comp
L Switch:SW_MEC_5E SW16
U 1 1 611D0F2C
P 5150 4450
F 0 "SW16" H 5150 4835 50  0000 C CNN
F 1 "SW_MEC_5E" H 5150 4744 50  0000 C CNN
F 2 "" H 5150 4750 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 5150 4750 50  0001 C CNN
	1    5150 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 4350 5350 4000
Wire Wire Line
	5350 4450 5350 4350
Connection ~ 5350 4350
Wire Wire Line
	5350 4450 5350 4550
Connection ~ 5350 4450
Connection ~ 5350 4000
Connection ~ 4700 4250
Connection ~ 4700 3900
Wire Wire Line
	4700 3900 4700 4250
Wire Wire Line
	5350 3900 5350 4000
Connection ~ 5350 3900
Wire Wire Line
	5350 3500 5350 3900
Connection ~ 5350 3500
Wire Wire Line
	5350 3400 5350 3500
Connection ~ 5350 3400
Wire Wire Line
	5350 3000 5350 3400
Connection ~ 5350 3000
Wire Wire Line
	5350 2900 5350 3000
Wire Wire Line
	4700 4250 4700 4350
Wire Wire Line
	4700 3800 4700 3900
Connection ~ 4700 3800
Wire Wire Line
	4700 3450 4700 3800
Connection ~ 4700 3450
Wire Wire Line
	4700 3350 4700 3450
Connection ~ 4700 3350
Wire Wire Line
	4700 3000 4700 3350
Connection ~ 4700 3000
Wire Wire Line
	4700 2900 4700 3000
$Comp
L Switch:SW_MEC_5E SW15
U 1 1 611D0F5C
P 5150 4000
F 0 "SW15" H 5150 4385 50  0000 C CNN
F 1 "SW_MEC_5E" H 5150 4294 50  0000 C CNN
F 2 "" H 5150 4300 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 5150 4300 50  0001 C CNN
	1    5150 4000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW14
U 1 1 611D0F66
P 5150 3500
F 0 "SW14" H 5150 3885 50  0000 C CNN
F 1 "SW_MEC_5E" H 5150 3794 50  0000 C CNN
F 2 "" H 5150 3800 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 5150 3800 50  0001 C CNN
	1    5150 3500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW13
U 1 1 611D0F70
P 5150 3000
F 0 "SW13" H 5150 3385 50  0000 C CNN
F 1 "SW_MEC_5E" H 5150 3294 50  0000 C CNN
F 2 "" H 5150 3300 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 5150 3300 50  0001 C CNN
	1    5150 3000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW8
U 1 1 611D0F7A
P 4500 4350
F 0 "SW8" H 4500 4735 50  0000 C CNN
F 1 "SW_MEC_5E" H 4500 4644 50  0000 C CNN
F 2 "" H 4500 4650 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4500 4650 50  0001 C CNN
	1    4500 4350
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW7
U 1 1 611D0F84
P 4500 3900
F 0 "SW7" H 4500 4285 50  0000 C CNN
F 1 "SW_MEC_5E" H 4500 4194 50  0000 C CNN
F 2 "" H 4500 4200 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4500 4200 50  0001 C CNN
	1    4500 3900
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW6
U 1 1 611D0F8E
P 4500 3450
F 0 "SW6" H 4500 3835 50  0000 C CNN
F 1 "SW_MEC_5E" H 4500 3744 50  0000 C CNN
F 2 "" H 4500 3750 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4500 3750 50  0001 C CNN
	1    4500 3450
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW5
U 1 1 611D0F98
P 4500 3000
F 0 "SW5" H 4500 3385 50  0000 C CNN
F 1 "SW_MEC_5E" H 4500 3294 50  0000 C CNN
F 2 "" H 4500 3300 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4500 3300 50  0001 C CNN
	1    4500 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 3200 3400 3200
Wire Wire Line
	3400 3200 3400 3000
Wire Wire Line
	3350 3300 3450 3300
Wire Wire Line
	3450 3300 3450 3200
Connection ~ 4300 3400
Wire Wire Line
	4300 3400 4300 3450
Wire Wire Line
	3350 3900 3400 3900
Wire Wire Line
	3400 3900 3400 4450
Wire Wire Line
	3350 3800 3450 3800
Wire Wire Line
	3450 3800 3450 4250
Wire Wire Line
	3350 3700 3500 3700
Wire Wire Line
	3350 3600 3550 3600
Wire Wire Line
	3550 3600 3550 3800
Connection ~ 4300 3800
Wire Wire Line
	2350 1200 1800 1200
Wire Wire Line
	1800 1200 1800 3300
Wire Wire Line
	1800 3300 2350 3300
Wire Wire Line
	2350 1100 1750 1100
Wire Wire Line
	1750 1100 1750 3200
Wire Wire Line
	1750 3200 2350 3200
Wire Wire Line
	2350 1900 1850 1900
Wire Wire Line
	1850 1900 1850 4000
Wire Wire Line
	1850 4000 2350 4000
$Comp
L Interface_Expansion:PCF8574 IOExt2
U 1 1 610F878D
P 2850 1500
F 0 "IOExt2" H 2850 2381 50  0000 C CNN
F 1 "PCF8574" H 2850 2290 50  0000 C CNN
F 2 "" H 2850 1500 50  0001 C CNN
F 3 "http://www.nxp.com/documents/data_sheet/PCF8574_PCF8574A.pdf" H 2850 1500 50  0001 C CNN
	1    2850 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 1100 3350 1100
Wire Wire Line
	3400 1100 3400 950 
Wire Wire Line
	3400 950  4300 950 
Wire Wire Line
	3350 1800 3400 1800
Wire Wire Line
	3400 1800 3400 2400
Wire Wire Line
	3400 2400 4950 2400
Wire Wire Line
	3350 1700 3450 1700
Wire Wire Line
	3450 1700 3450 2200
Wire Wire Line
	3450 2200 4300 2200
Connection ~ 4300 2200
Wire Wire Line
	4950 1950 3500 1950
Wire Wire Line
	3500 1950 3500 1600
Wire Wire Line
	3500 1600 3350 1600
Connection ~ 4950 1950
Wire Wire Line
	4300 1750 3550 1750
Wire Wire Line
	3550 1750 3550 1500
Wire Wire Line
	3550 1500 3350 1500
Connection ~ 4300 1750
Wire Wire Line
	4950 1450 3600 1450
Wire Wire Line
	3600 1450 3600 1400
Wire Wire Line
	3600 1400 3350 1400
Connection ~ 4950 1450
Wire Wire Line
	3350 1200 3450 1200
Wire Wire Line
	3450 1200 3450 1100
Wire Wire Line
	3450 1100 4850 1100
Wire Wire Line
	3350 1300 4300 1300
Connection ~ 4300 1300
$Comp
L power:GND #PWR0109
U 1 1 613A0FF9
P 2850 4300
F 0 "#PWR0109" H 2850 4050 50  0001 C CNN
F 1 "GND" H 2855 4127 50  0000 C CNN
F 2 "" H 2850 4300 50  0001 C CNN
F 3 "" H 2850 4300 50  0001 C CNN
	1    2850 4300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 613A19D9
P 2850 2200
F 0 "#PWR0110" H 2850 1950 50  0001 C CNN
F 1 "GND" H 2855 2027 50  0000 C CNN
F 2 "" H 2850 2200 50  0001 C CNN
F 3 "" H 2850 2200 50  0001 C CNN
	1    2850 2200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0111
U 1 1 613A3A97
P 2250 1600
F 0 "#PWR0111" H 2250 1350 50  0001 C CNN
F 1 "GND" H 2255 1427 50  0000 C CNN
F 2 "" H 2250 1600 50  0001 C CNN
F 3 "" H 2250 1600 50  0001 C CNN
	1    2250 1600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0112
U 1 1 613AF289
P 2250 1600
F 0 "#PWR0112" H 2250 1350 50  0001 C CNN
F 1 "GND" H 2255 1427 50  0000 C CNN
F 2 "" H 2250 1600 50  0001 C CNN
F 3 "" H 2250 1600 50  0001 C CNN
	1    2250 1600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0113
U 1 1 613C4997
P 2250 3800
F 0 "#PWR0113" H 2250 3550 50  0001 C CNN
F 1 "GND" H 2255 3627 50  0000 C CNN
F 2 "" H 2250 3800 50  0001 C CNN
F 3 "" H 2250 3800 50  0001 C CNN
	1    2250 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 1400 2250 1400
Wire Wire Line
	2250 1400 2250 1500
Connection ~ 2250 1600
Wire Wire Line
	2350 1500 2250 1500
Connection ~ 2250 1500
Wire Wire Line
	2250 1500 2250 1600
Wire Wire Line
	2350 1600 2250 1600
Wire Wire Line
	2350 3600 2250 3600
Wire Wire Line
	2250 3600 2250 3700
Wire Wire Line
	2350 3700 2250 3700
Connection ~ 2250 3700
Wire Wire Line
	2250 3700 2250 3800
$Comp
L power:VCC #PWR0114
U 1 1 613EA1E4
P 2250 2700
F 0 "#PWR0114" H 2250 2550 50  0001 C CNN
F 1 "VCC" H 2265 2873 50  0000 C CNN
F 2 "" H 2250 2700 50  0001 C CNN
F 3 "" H 2250 2700 50  0001 C CNN
	1    2250 2700
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0115
U 1 1 613EAFA9
P 2250 700
F 0 "#PWR0115" H 2250 550 50  0001 C CNN
F 1 "VCC" H 2265 873 50  0000 C CNN
F 2 "" H 2250 700 50  0001 C CNN
F 3 "" H 2250 700 50  0001 C CNN
	1    2250 700 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 800  2250 800 
Wire Wire Line
	2250 800  2250 700 
Wire Wire Line
	2850 2900 2250 2900
Wire Wire Line
	2250 2900 2250 2700
Wire Wire Line
	2350 3500 2250 3500
Wire Wire Line
	2250 3500 2250 2900
Connection ~ 2250 2900
Wire Wire Line
	1800 6100 2500 6100
Connection ~ 1800 3300
Wire Wire Line
	1750 6000 2500 6000
Connection ~ 1750 3200
Wire Wire Line
	3400 3000 4300 3000
Wire Wire Line
	3400 4450 4950 4450
Wire Wire Line
	3450 3200 4850 3200
Wire Wire Line
	3450 4250 4300 4250
Wire Wire Line
	3500 4000 4950 4000
Wire Wire Line
	3550 3800 4300 3800
Wire Wire Line
	3350 3500 4950 3500
Wire Wire Line
	3350 3400 4300 3400
$Comp
L Interface_Expansion:PCF8574 IOExt3
U 1 1 610F930B
P 2850 3600
F 0 "IOExt3" H 2850 4481 50  0000 C CNN
F 1 "PCF8574" H 2850 4390 50  0000 C CNN
F 2 "" H 2850 3600 50  0001 C CNN
F 3 "http://www.nxp.com/documents/data_sheet/PCF8574_PCF8574A.pdf" H 2850 3600 50  0001 C CNN
	1    2850 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 3700 3500 4000
$Comp
L Device:R_Variable_US R-left1
U 1 1 61539769
P 3800 5550
F 0 "R-left1" H 3928 5596 50  0000 L CNN
F 1 "R-force" H 3928 5505 50  0000 L CNN
F 2 "" V 3730 5550 50  0001 C CNN
F 3 "~" H 3800 5550 50  0001 C CNN
	1    3800 5550
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Variable_US R-right1
U 1 1 6154835D
P 3800 6300
F 0 "R-right1" H 3928 6346 50  0000 L CNN
F 1 "R-force" H 3928 6255 50  0000 L CNN
F 2 "" V 3730 6300 50  0001 C CNN
F 3 "~" H 3800 6300 50  0001 C CNN
	1    3800 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 5400 3400 5400
Wire Wire Line
	3400 5400 3400 5900
Wire Wire Line
	3400 5900 3300 5900
Wire Wire Line
	3300 6000 3450 6000
Wire Wire Line
	3450 6000 3450 5700
Wire Wire Line
	3450 5700 3800 5700
Wire Wire Line
	3800 6150 3800 6100
Wire Wire Line
	3800 6100 3300 6100
Wire Wire Line
	3300 6200 3400 6200
Wire Wire Line
	3400 6200 3400 6450
Wire Wire Line
	3400 6450 3800 6450
$Comp
L power:GND #PWR?
U 1 1 610C32A6
P 9600 5450
F 0 "#PWR?" H 9600 5200 50  0001 C CNN
F 1 "GND" H 9605 5277 50  0000 C CNN
F 2 "" H 9600 5450 50  0001 C CNN
F 3 "" H 9600 5450 50  0001 C CNN
	1    9600 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 5450 9300 5450
Wire Wire Line
	7500 5100 9300 5100
Wire Wire Line
	9300 5100 9300 5450
Connection ~ 7500 5100
Connection ~ 9300 5450
Wire Wire Line
	9300 5450 9600 5450
Wire Wire Line
	8150 6000 8150 5850
Wire Wire Line
	8150 5850 8400 5850
Wire Wire Line
	7800 5800 7800 6050
Wire Wire Line
	7800 6050 8400 6050
Wire Wire Line
	7700 5900 7700 6250
Wire Wire Line
	7700 6250 8400 6250
Wire Wire Line
	7850 5600 7850 5950
Wire Wire Line
	7850 5950 8400 5950
Wire Wire Line
	7050 5600 7850 5600
Wire Wire Line
	6900 5700 7750 5700
Wire Wire Line
	7750 5700 7750 6150
Wire Wire Line
	7750 6150 8400 6150
Connection ~ 6900 5700
Wire Wire Line
	5100 5300 5750 5300
Wire Wire Line
	5350 2900 5350 2500
Connection ~ 5350 2900
Wire Wire Line
	1850 4000 1850 4850
Wire Wire Line
	1850 4850 8100 4850
Wire Wire Line
	8100 4850 8100 5650
Wire Wire Line
	8100 5650 8400 5650
Connection ~ 1850 4000
Wire Wire Line
	1800 4800 8150 4800
Wire Wire Line
	8150 4800 8150 5550
Wire Wire Line
	8150 5550 8400 5550
Connection ~ 1800 4800
Wire Wire Line
	1800 4800 1800 6100
Wire Wire Line
	1750 3200 1750 4750
Wire Wire Line
	1800 3300 1800 4800
Wire Wire Line
	8200 4750 1750 4750
Wire Wire Line
	8200 4750 8200 5350
Connection ~ 1750 4750
Wire Wire Line
	1750 4750 1750 6000
Wire Wire Line
	8200 5350 8400 5350
$Comp
L power:VCC #PWR?
U 1 1 61298E5F
P 7800 5400
F 0 "#PWR?" H 7800 5250 50  0001 C CNN
F 1 "VCC" H 7815 5573 50  0000 C CNN
F 2 "" H 7800 5400 50  0001 C CNN
F 3 "" H 7800 5400 50  0001 C CNN
	1    7800 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 5450 7800 5400
Wire Wire Line
	7800 5450 8400 5450
Wire Wire Line
	6550 5800 7800 5800
Wire Wire Line
	6900 5750 8400 5750
Wire Wire Line
	6900 5750 6900 6200
Wire Wire Line
	6900 6200 6550 6200
Wire Wire Line
	6550 6000 8150 6000
Wire Wire Line
	6550 5900 7700 5900
$EndSCHEMATC
