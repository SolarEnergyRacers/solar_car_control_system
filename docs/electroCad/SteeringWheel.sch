EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
Title "SER4"
Date "2021-08-07"
Rev "0.1"
Comp "Solar Energy Car Racers"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "Steering Wheel Unit"
$EndDescr
$Comp
L Analog_ADC:ADS1115IDGS ADC?
U 1 1 611115DA
P 2200 6000
AR Path="/611115DA" Ref="ADC?"  Part="1" 
AR Path="/611353D7/611115DA" Ref="ADC301"  Part="1" 
F 0 "ADC301" H 2200 6681 50  0000 C CNN
F 1 "ADS1115IDGS" H 2200 6590 50  0000 C CNN
F 2 "Package_SO:TSSOP-10_3x3mm_P0.5mm" H 2200 5500 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ads1113.pdf" H 2150 5100 50  0001 C CNN
	1    2200 6000
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 611115E0
P 6500 5450
AR Path="/611115E0" Ref="R?"  Part="1" 
AR Path="/611353D7/611115E0" Ref="R302"  Part="1" 
F 0 "R302" V 6293 5450 50  0000 C CNN
F 1 "10k" V 6384 5450 50  0000 C CNN
F 2 "" V 6430 5450 50  0001 C CNN
F 3 "~" H 6500 5450 50  0001 C CNN
	1    6500 5450
	0    1    1    0   
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 611115E6
P 1550 5450
AR Path="/611115E6" Ref="#PWR?"  Part="1" 
AR Path="/611353D7/611115E6" Ref="#PWR0101"  Part="1" 
F 0 "#PWR0101" H 1550 5300 50  0001 C CNN
F 1 "VCC" H 1565 5623 50  0000 C CNN
F 2 "" H 1550 5450 50  0001 C CNN
F 3 "" H 1550 5450 50  0001 C CNN
	1    1550 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 5100 6800 5100
Wire Wire Line
	6650 5450 6800 5450
$Comp
L Connector:DB15_Female_HighDensity J?
U 1 1 611115EE
P 8000 5850
AR Path="/611115EE" Ref="J?"  Part="1" 
AR Path="/611353D7/611115EE" Ref="J301"  Part="1" 
F 0 "J301" H 8000 6717 50  0000 C CNN
F 1 "DB15_Female_HighDensity" H 8000 6626 50  0000 C CNN
F 2 "" H 7050 6250 50  0001 C CNN
F 3 " ~" H 7050 6250 50  0001 C CNN
	1    8000 5850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 611115F4
P 5050 6600
AR Path="/611115F4" Ref="#PWR?"  Part="1" 
AR Path="/611353D7/611115F4" Ref="#PWR0102"  Part="1" 
F 0 "#PWR0102" H 5050 6350 50  0001 C CNN
F 1 "GND" H 5055 6427 50  0000 C CNN
F 2 "" H 5050 6600 50  0001 C CNN
F 3 "" H 5050 6600 50  0001 C CNN
	1    5050 6600
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 611115FA
P 4400 5300
AR Path="/611115FA" Ref="#PWR?"  Part="1" 
AR Path="/611353D7/611115FA" Ref="#PWR0103"  Part="1" 
F 0 "#PWR0103" H 4400 5150 50  0001 C CNN
F 1 "VCC" H 4415 5473 50  0000 C CNN
F 2 "" H 4400 5300 50  0001 C CNN
F 3 "" H 4400 5300 50  0001 C CNN
	1    4400 5300
	1    0    0    -1  
$EndComp
NoConn ~ 5850 6100
NoConn ~ 4250 5900
NoConn ~ 4250 5800
NoConn ~ 4250 5700
NoConn ~ 4250 5600
NoConn ~ 4250 5500
Wire Wire Line
	5050 6500 5050 6600
$Comp
L Driver_Display:CR2013-MI2120 Display?
U 1 1 61111607
P 5050 5900
AR Path="/61111607" Ref="Display?"  Part="1" 
AR Path="/611353D7/61111607" Ref="Display301"  Part="1" 
F 0 "Display301" H 5050 6681 50  0000 C CNN
F 1 "CR2013-MI2120" H 5050 6590 50  0000 C CNN
F 2 "Display:CR2013-MI2120" H 5050 5200 50  0001 C CNN
F 3 "http://pan.baidu.com/s/11Y990" H 4400 6400 50  0001 C CNN
	1    5050 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 6111160D
P 6500 5100
AR Path="/6111160D" Ref="R?"  Part="1" 
AR Path="/611353D7/6111160D" Ref="R301"  Part="1" 
F 0 "R301" V 6293 5100 50  0000 C CNN
F 1 "10k" V 6384 5100 50  0000 C CNN
F 2 "" V 6430 5100 50  0001 C CNN
F 3 "~" H 6500 5100 50  0001 C CNN
	1    6500 5100
	0    1    1    0   
$EndComp
Wire Wire Line
	6800 5100 6800 5450
Wire Wire Line
	5850 5700 6200 5700
Wire Wire Line
	5850 5600 6350 5600
Wire Wire Line
	6350 5450 6350 5600
Connection ~ 6350 5600
Wire Wire Line
	6350 5100 6200 5100
Wire Wire Line
	6200 5100 6200 5700
$Comp
L power:GND #PWR?
U 1 1 6111161A
P 2200 6500
AR Path="/6111161A" Ref="#PWR?"  Part="1" 
AR Path="/611353D7/6111161A" Ref="#PWR0104"  Part="1" 
F 0 "#PWR0104" H 2200 6250 50  0001 C CNN
F 1 "GND" H 2205 6327 50  0000 C CNN
F 2 "" H 2200 6500 50  0001 C CNN
F 3 "" H 2200 6500 50  0001 C CNN
	1    2200 6500
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1550 5450 1550 5500
Wire Wire Line
	1550 5500 2200 5500
Wire Wire Line
	4650 2500 5050 2500
Connection ~ 4650 2500
Wire Wire Line
	4650 2500 4000 2500
$Comp
L power:GND #PWR?
U 1 1 61111625
P 5050 2500
AR Path="/61111625" Ref="#PWR?"  Part="1" 
AR Path="/611353D7/61111625" Ref="#PWR0105"  Part="1" 
F 0 "#PWR0105" H 5050 2250 50  0001 C CNN
F 1 "GND" H 5055 2327 50  0000 C CNN
F 2 "" H 5050 2500 50  0001 C CNN
F 3 "" H 5050 2500 50  0001 C CNN
	1    5050 2500
	1    0    0    -1  
$EndComp
Connection ~ 4250 4450
Connection ~ 3600 4250
Connection ~ 4250 4000
Wire Wire Line
	3600 3900 3600 3800
Connection ~ 4250 3500
Connection ~ 4250 3000
Wire Wire Line
	4150 3000 4150 3200
Wire Wire Line
	4250 3000 4150 3000
Wire Wire Line
	3600 3350 3600 3400
Connection ~ 3600 3000
Wire Wire Line
	3600 2900 3600 3000
Connection ~ 4000 4350
Wire Wire Line
	4000 4550 4000 4350
Wire Wire Line
	4650 4550 4000 4550
Wire Wire Line
	4250 4450 4250 4350
Wire Wire Line
	4250 3900 4250 4000
Wire Wire Line
	4250 3500 4250 3400
Wire Wire Line
	4250 3000 4250 2900
Wire Wire Line
	3600 4250 3600 4350
$Comp
L Switch:SW_MEC_5E SW?
U 1 1 6111163E
P 4450 4450
AR Path="/6111163E" Ref="SW?"  Part="1" 
AR Path="/611353D7/6111163E" Ref="SW316"  Part="1" 
F 0 "SW316" H 4450 4835 50  0000 C CNN
F 1 "SW_MEC_5E" H 4450 4744 50  0000 C CNN
F 2 "" H 4450 4750 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4450 4750 50  0001 C CNN
	1    4450 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 4350 4650 4000
Wire Wire Line
	4650 4450 4650 4350
Connection ~ 4650 4350
Wire Wire Line
	4650 4450 4650 4550
Connection ~ 4650 4450
Connection ~ 4650 4000
Connection ~ 4000 4250
Connection ~ 4000 3900
Wire Wire Line
	4000 3900 4000 4250
Wire Wire Line
	4650 3900 4650 4000
Connection ~ 4650 3900
Wire Wire Line
	4650 3500 4650 3900
Connection ~ 4650 3500
Wire Wire Line
	4650 3400 4650 3500
Connection ~ 4650 3400
Wire Wire Line
	4650 3000 4650 3400
Connection ~ 4650 3000
Wire Wire Line
	4650 2900 4650 3000
Wire Wire Line
	4000 4250 4000 4350
Wire Wire Line
	4000 3800 4000 3900
Connection ~ 4000 3800
Wire Wire Line
	4000 3450 4000 3800
Connection ~ 4000 3450
Wire Wire Line
	4000 3350 4000 3450
Connection ~ 4000 3350
Wire Wire Line
	4000 3000 4000 3350
Connection ~ 4000 3000
Wire Wire Line
	4000 2900 4000 3000
$Comp
L Switch:SW_MEC_5E SW?
U 1 1 61111660
P 4450 4000
AR Path="/61111660" Ref="SW?"  Part="1" 
AR Path="/611353D7/61111660" Ref="SW314"  Part="1" 
F 0 "SW314" H 4450 4385 50  0000 C CNN
F 1 "SW_MEC_5E" H 4450 4294 50  0000 C CNN
F 2 "" H 4450 4300 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4450 4300 50  0001 C CNN
	1    4450 4000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW?
U 1 1 61111666
P 4450 3500
AR Path="/61111666" Ref="SW?"  Part="1" 
AR Path="/611353D7/61111666" Ref="SW312"  Part="1" 
F 0 "SW312" H 4450 3885 50  0000 C CNN
F 1 "SW_MEC_5E" H 4450 3794 50  0000 C CNN
F 2 "" H 4450 3800 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4450 3800 50  0001 C CNN
	1    4450 3500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW?
U 1 1 6111166C
P 4450 3000
AR Path="/6111166C" Ref="SW?"  Part="1" 
AR Path="/611353D7/6111166C" Ref="SW310"  Part="1" 
F 0 "SW310" H 4450 3385 50  0000 C CNN
F 1 "SW_MEC_5E" H 4450 3294 50  0000 C CNN
F 2 "" H 4450 3300 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4450 3300 50  0001 C CNN
	1    4450 3000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW?
U 1 1 61111672
P 3800 4350
AR Path="/61111672" Ref="SW?"  Part="1" 
AR Path="/611353D7/61111672" Ref="SW315"  Part="1" 
F 0 "SW315" H 3800 4735 50  0000 C CNN
F 1 "SW_MEC_5E" H 3800 4644 50  0000 C CNN
F 2 "" H 3800 4650 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 3800 4650 50  0001 C CNN
	1    3800 4350
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW?
U 1 1 61111678
P 3800 3900
AR Path="/61111678" Ref="SW?"  Part="1" 
AR Path="/611353D7/61111678" Ref="SW313"  Part="1" 
F 0 "SW313" H 3800 4285 50  0000 C CNN
F 1 "SW_MEC_5E" H 3800 4194 50  0000 C CNN
F 2 "" H 3800 4200 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 3800 4200 50  0001 C CNN
	1    3800 3900
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW?
U 1 1 6111167E
P 3800 3450
AR Path="/6111167E" Ref="SW?"  Part="1" 
AR Path="/611353D7/6111167E" Ref="SW311"  Part="1" 
F 0 "SW311" H 3800 3835 50  0000 C CNN
F 1 "SW_MEC_5E" H 3800 3744 50  0000 C CNN
F 2 "" H 3800 3750 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 3800 3750 50  0001 C CNN
	1    3800 3450
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW?
U 1 1 61111684
P 3800 3000
AR Path="/61111684" Ref="SW?"  Part="1" 
AR Path="/611353D7/61111684" Ref="SW309"  Part="1" 
F 0 "SW309" H 3800 3385 50  0000 C CNN
F 1 "SW_MEC_5E" H 3800 3294 50  0000 C CNN
F 2 "" H 3800 3300 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 3800 3300 50  0001 C CNN
	1    3800 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 3200 2700 3200
Wire Wire Line
	2700 3200 2700 3000
Wire Wire Line
	2650 3300 2750 3300
Wire Wire Line
	2750 3300 2750 3200
Connection ~ 3600 3400
Wire Wire Line
	3600 3400 3600 3450
Wire Wire Line
	2650 3900 2700 3900
Wire Wire Line
	2700 3900 2700 4450
Wire Wire Line
	2650 3800 2750 3800
Wire Wire Line
	2750 3800 2750 4250
Wire Wire Line
	2650 3700 2800 3700
Wire Wire Line
	2650 3600 2850 3600
Wire Wire Line
	2850 3600 2850 3800
Connection ~ 3600 3800
Wire Wire Line
	1100 3300 1650 3300
Wire Wire Line
	1050 3200 1650 3200
Wire Wire Line
	1150 4000 1650 4000
$Comp
L power:GND #PWR?
U 1 1 6111169B
P 2150 4400
AR Path="/6111169B" Ref="#PWR?"  Part="1" 
AR Path="/611353D7/6111169B" Ref="#PWR0106"  Part="1" 
F 0 "#PWR0106" H 2150 4150 50  0001 C CNN
F 1 "GND" H 2155 4227 50  0000 C CNN
F 2 "" H 2150 4400 50  0001 C CNN
F 3 "" H 2150 4400 50  0001 C CNN
	1    2150 4400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 611116A1
P 1550 3800
AR Path="/611116A1" Ref="#PWR?"  Part="1" 
AR Path="/611353D7/611116A1" Ref="#PWR0107"  Part="1" 
F 0 "#PWR0107" H 1550 3550 50  0001 C CNN
F 1 "GND" H 1555 3627 50  0000 C CNN
F 2 "" H 1550 3800 50  0001 C CNN
F 3 "" H 1550 3800 50  0001 C CNN
	1    1550 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 3600 1550 3600
Wire Wire Line
	1550 3600 1550 3700
Wire Wire Line
	1650 3700 1550 3700
Connection ~ 1550 3700
Wire Wire Line
	1550 3700 1550 3800
$Comp
L power:VCC #PWR?
U 1 1 611116AC
P 1550 2700
AR Path="/611116AC" Ref="#PWR?"  Part="1" 
AR Path="/611353D7/611116AC" Ref="#PWR0108"  Part="1" 
F 0 "#PWR0108" H 1550 2550 50  0001 C CNN
F 1 "VCC" H 1565 2873 50  0000 C CNN
F 2 "" H 1550 2700 50  0001 C CNN
F 3 "" H 1550 2700 50  0001 C CNN
	1    1550 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 2900 1550 2900
Wire Wire Line
	1550 2900 1550 2700
Wire Wire Line
	1650 3500 1550 3500
Wire Wire Line
	1550 3500 1550 2900
Connection ~ 1550 2900
Wire Wire Line
	1100 6100 1800 6100
Wire Wire Line
	1050 6000 1800 6000
Wire Wire Line
	2700 3000 3600 3000
Wire Wire Line
	2700 4450 4250 4450
Wire Wire Line
	2750 3200 4150 3200
Wire Wire Line
	2750 4250 3600 4250
Wire Wire Line
	2800 4000 4250 4000
Wire Wire Line
	2850 3800 3600 3800
Wire Wire Line
	2650 3500 4250 3500
Wire Wire Line
	2650 3400 3600 3400
$Comp
L Interface_Expansion:PCF8574 IOExt?
U 1 1 611116C1
P 2150 3600
AR Path="/611116C1" Ref="IOExt?"  Part="1" 
AR Path="/611353D7/611116C1" Ref="IOExt302"  Part="1" 
F 0 "IOExt302" H 2150 4481 50  0000 C CNN
F 1 "PCF8574" H 2150 4390 50  0000 C CNN
F 2 "" H 2150 3600 50  0001 C CNN
F 3 "http://www.nxp.com/documents/data_sheet/PCF8574_PCF8574A.pdf" H 2150 3600 50  0001 C CNN
	1    2150 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 3700 2800 4000
$Comp
L Device:R_Variable_US R-left?
U 1 1 611116C8
P 3100 5550
AR Path="/611116C8" Ref="R-left?"  Part="1" 
AR Path="/611353D7/611116C8" Ref="R-left301"  Part="1" 
F 0 "R-left301" H 3228 5596 50  0000 L CNN
F 1 "R-force" H 3228 5505 50  0000 L CNN
F 2 "" V 3030 5550 50  0001 C CNN
F 3 "~" H 3100 5550 50  0001 C CNN
	1    3100 5550
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Variable_US R-right?
U 1 1 611116CE
P 3100 6300
AR Path="/611116CE" Ref="R-right?"  Part="1" 
AR Path="/611353D7/611116CE" Ref="R-right301"  Part="1" 
F 0 "R-right301" H 3228 6346 50  0000 L CNN
F 1 "R-force" H 3228 6255 50  0000 L CNN
F 2 "" V 3030 6300 50  0001 C CNN
F 3 "~" H 3100 6300 50  0001 C CNN
	1    3100 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 5400 2700 5400
Wire Wire Line
	2700 5400 2700 5900
Wire Wire Line
	2700 5900 2600 5900
Wire Wire Line
	2600 6000 2750 6000
Wire Wire Line
	2750 6000 2750 5700
Wire Wire Line
	2750 5700 3100 5700
Wire Wire Line
	3100 6150 3100 6100
Wire Wire Line
	3100 6100 2600 6100
Wire Wire Line
	2600 6200 2700 6200
Wire Wire Line
	2700 6200 2700 6450
Wire Wire Line
	2700 6450 3100 6450
$Comp
L power:GND #PWR?
U 1 1 611116DF
P 8900 5450
AR Path="/611116DF" Ref="#PWR?"  Part="1" 
AR Path="/611353D7/611116DF" Ref="#PWR0109"  Part="1" 
F 0 "#PWR0109" H 8900 5200 50  0001 C CNN
F 1 "GND" H 8905 5277 50  0000 C CNN
F 2 "" H 8900 5450 50  0001 C CNN
F 3 "" H 8900 5450 50  0001 C CNN
	1    8900 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	8300 5450 8600 5450
Wire Wire Line
	6800 5100 8600 5100
Wire Wire Line
	8600 5100 8600 5450
Connection ~ 6800 5100
Connection ~ 8600 5450
Wire Wire Line
	8600 5450 8900 5450
Wire Wire Line
	7450 6000 7450 5850
Wire Wire Line
	7450 5850 7700 5850
Wire Wire Line
	7100 5800 7100 6050
Wire Wire Line
	7100 6050 7700 6050
Wire Wire Line
	7000 5900 7000 6250
Wire Wire Line
	7000 6250 7700 6250
Wire Wire Line
	7150 5600 7150 5950
Wire Wire Line
	7150 5950 7700 5950
Wire Wire Line
	6350 5600 7150 5600
Wire Wire Line
	6200 5700 7050 5700
Wire Wire Line
	7050 5700 7050 6150
Wire Wire Line
	7050 6150 7700 6150
Connection ~ 6200 5700
Wire Wire Line
	4400 5300 5050 5300
Wire Wire Line
	4650 2900 4650 2500
Connection ~ 4650 2900
Wire Wire Line
	1150 4000 1150 4850
Wire Wire Line
	1150 4850 7400 4850
Wire Wire Line
	7400 4850 7400 5650
Wire Wire Line
	7400 5650 7700 5650
Wire Wire Line
	1100 4800 7450 4800
Wire Wire Line
	7450 4800 7450 5550
Wire Wire Line
	7450 5550 7700 5550
Connection ~ 1100 4800
Wire Wire Line
	1100 4800 1100 6100
Wire Wire Line
	1050 3200 1050 4750
Wire Wire Line
	1100 3300 1100 4800
Wire Wire Line
	7500 4750 1050 4750
Wire Wire Line
	7500 4750 7500 5350
Connection ~ 1050 4750
Wire Wire Line
	1050 4750 1050 6000
Wire Wire Line
	7500 5350 7700 5350
$Comp
L power:VCC #PWR?
U 1 1 6111170B
P 7100 5400
AR Path="/6111170B" Ref="#PWR?"  Part="1" 
AR Path="/611353D7/6111170B" Ref="#PWR0110"  Part="1" 
F 0 "#PWR0110" H 7100 5250 50  0001 C CNN
F 1 "VCC" H 7115 5573 50  0000 C CNN
F 2 "" H 7100 5400 50  0001 C CNN
F 3 "" H 7100 5400 50  0001 C CNN
	1    7100 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 5450 7100 5400
Wire Wire Line
	7100 5450 7700 5450
Wire Wire Line
	5850 5800 7100 5800
Wire Wire Line
	6200 5750 7700 5750
Wire Wire Line
	6200 5750 6200 6200
Wire Wire Line
	6200 6200 5850 6200
Wire Wire Line
	5850 6000 7450 6000
Wire Wire Line
	5850 5900 7000 5900
Connection ~ 1150 4000
Connection ~ 1050 3200
Connection ~ 1100 3300
Wire Wire Line
	1550 800  1550 700 
Wire Wire Line
	2150 800  1550 800 
$Comp
L power:VCC #PWR?
U 1 1 6111171E
P 1550 700
AR Path="/6111171E" Ref="#PWR?"  Part="1" 
AR Path="/611353D7/6111171E" Ref="#PWR0111"  Part="1" 
F 0 "#PWR0111" H 1550 550 50  0001 C CNN
F 1 "VCC" H 1565 873 50  0000 C CNN
F 2 "" H 1550 700 50  0001 C CNN
F 3 "" H 1550 700 50  0001 C CNN
	1    1550 700 
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 1600 1550 1600
Wire Wire Line
	1550 1500 1550 1600
Connection ~ 1550 1500
Wire Wire Line
	1650 1500 1550 1500
Connection ~ 1550 1600
Wire Wire Line
	1550 1400 1550 1500
Wire Wire Line
	1650 1400 1550 1400
$Comp
L power:GND #PWR?
U 1 1 6111172B
P 1550 1600
AR Path="/6111172B" Ref="#PWR?"  Part="1" 
AR Path="/611353D7/6111172B" Ref="#PWR0112"  Part="1" 
F 0 "#PWR0112" H 1550 1350 50  0001 C CNN
F 1 "GND" H 1555 1427 50  0000 C CNN
F 2 "" H 1550 1600 50  0001 C CNN
F 3 "" H 1550 1600 50  0001 C CNN
	1    1550 1600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61111731
P 1550 1600
AR Path="/61111731" Ref="#PWR?"  Part="1" 
AR Path="/611353D7/61111731" Ref="#PWR0113"  Part="1" 
F 0 "#PWR0113" H 1550 1350 50  0001 C CNN
F 1 "GND" H 1555 1427 50  0000 C CNN
F 2 "" H 1550 1600 50  0001 C CNN
F 3 "" H 1550 1600 50  0001 C CNN
	1    1550 1600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61111737
P 2150 2300
AR Path="/61111737" Ref="#PWR?"  Part="1" 
AR Path="/611353D7/61111737" Ref="#PWR0114"  Part="1" 
F 0 "#PWR0114" H 2150 2050 50  0001 C CNN
F 1 "GND" H 2155 2127 50  0000 C CNN
F 2 "" H 2150 2300 50  0001 C CNN
F 3 "" H 2150 2300 50  0001 C CNN
	1    2150 2300
	1    0    0    -1  
$EndComp
Connection ~ 3600 1300
Wire Wire Line
	2650 1300 3600 1300
Wire Wire Line
	2750 1100 4150 1100
Wire Wire Line
	2750 1200 2750 1100
Wire Wire Line
	2650 1200 2750 1200
Connection ~ 4250 1450
Wire Wire Line
	2900 1400 2650 1400
Wire Wire Line
	2900 1450 2900 1400
Wire Wire Line
	4250 1450 2900 1450
Connection ~ 3600 1750
Wire Wire Line
	2850 1500 2650 1500
Wire Wire Line
	2850 1750 2850 1500
Wire Wire Line
	3600 1750 2850 1750
Connection ~ 4250 1950
Wire Wire Line
	2800 1600 2650 1600
Wire Wire Line
	2800 1950 2800 1600
Wire Wire Line
	4250 1950 2800 1950
Connection ~ 3600 2200
Wire Wire Line
	2750 2200 3600 2200
Wire Wire Line
	2650 1700 2750 1700
Wire Wire Line
	2700 2400 4250 2400
Wire Wire Line
	2700 1800 2700 2400
Wire Wire Line
	2650 1800 2700 1800
Wire Wire Line
	2700 950  3600 950 
Wire Wire Line
	2700 1100 2700 950 
Wire Wire Line
	2700 1100 2650 1100
$Comp
L Interface_Expansion:PCF8574 IOExt?
U 1 1 61111758
P 2150 1500
AR Path="/61111758" Ref="IOExt?"  Part="1" 
AR Path="/611353D7/61111758" Ref="IOExt301"  Part="1" 
F 0 "IOExt301" H 2150 2381 50  0000 C CNN
F 1 "PCF8574" H 2150 2290 50  0000 C CNN
F 2 "" H 2150 1500 50  0001 C CNN
F 3 "http://www.nxp.com/documents/data_sheet/PCF8574_PCF8574A.pdf" H 2150 1500 50  0001 C CNN
	1    2150 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 1900 1150 4000
Wire Wire Line
	1650 1900 1150 1900
Wire Wire Line
	1050 1100 1050 3200
Wire Wire Line
	1650 1100 1050 1100
Wire Wire Line
	1100 1200 1100 3300
Wire Wire Line
	1650 1200 1100 1200
$Comp
L Switch:SW_MEC_5E SW?
U 1 1 61111764
P 3800 950
AR Path="/61111764" Ref="SW?"  Part="1" 
AR Path="/611353D7/61111764" Ref="SW301"  Part="1" 
F 0 "SW301" H 3800 1335 50  0000 C CNN
F 1 "SW_MEC_5E" H 3800 1244 50  0000 C CNN
F 2 "Button_Switch_THT:SW_MEC_5GTH9" H 3800 1250 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 3800 1250 50  0001 C CNN
	1    3800 950 
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW?
U 1 1 6111176A
P 3800 1400
AR Path="/6111176A" Ref="SW?"  Part="1" 
AR Path="/611353D7/6111176A" Ref="SW303"  Part="1" 
F 0 "SW303" H 3800 1785 50  0000 C CNN
F 1 "SW_MEC_5E" H 3800 1694 50  0000 C CNN
F 2 "Button_Switch_THT:SW_MEC_5GTH9" H 3800 1700 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 3800 1700 50  0001 C CNN
	1    3800 1400
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW?
U 1 1 61111770
P 3800 1850
AR Path="/61111770" Ref="SW?"  Part="1" 
AR Path="/611353D7/61111770" Ref="SW305"  Part="1" 
F 0 "SW305" H 3800 2235 50  0000 C CNN
F 1 "SW_MEC_5E" H 3800 2144 50  0000 C CNN
F 2 "Button_Switch_THT:SW_MEC_5GTH9" H 3800 2150 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 3800 2150 50  0001 C CNN
	1    3800 1850
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW?
U 1 1 61111776
P 3800 2300
AR Path="/61111776" Ref="SW?"  Part="1" 
AR Path="/611353D7/61111776" Ref="SW307"  Part="1" 
F 0 "SW307" H 3800 2685 50  0000 C CNN
F 1 "SW_MEC_5E" H 3800 2594 50  0000 C CNN
F 2 "Button_Switch_THT:SW_MEC_5GTH9" H 3800 2600 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 3800 2600 50  0001 C CNN
	1    3800 2300
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW?
U 1 1 6111177C
P 4450 950
AR Path="/6111177C" Ref="SW?"  Part="1" 
AR Path="/611353D7/6111177C" Ref="SW302"  Part="1" 
F 0 "SW302" H 4450 1335 50  0000 C CNN
F 1 "SW_MEC_5E" H 4450 1244 50  0000 C CNN
F 2 "Button_Switch_THT:SW_MEC_5GTH9" H 4450 1250 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4450 1250 50  0001 C CNN
	1    4450 950 
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW?
U 1 1 61111782
P 4450 1450
AR Path="/61111782" Ref="SW?"  Part="1" 
AR Path="/611353D7/61111782" Ref="SW304"  Part="1" 
F 0 "SW304" H 4450 1835 50  0000 C CNN
F 1 "SW_MEC_5E" H 4450 1744 50  0000 C CNN
F 2 "Button_Switch_THT:SW_MEC_5GTH9" H 4450 1750 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4450 1750 50  0001 C CNN
	1    4450 1450
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW?
U 1 1 61111788
P 4450 1950
AR Path="/61111788" Ref="SW?"  Part="1" 
AR Path="/611353D7/61111788" Ref="SW306"  Part="1" 
F 0 "SW306" H 4450 2335 50  0000 C CNN
F 1 "SW_MEC_5E" H 4450 2244 50  0000 C CNN
F 2 "Button_Switch_THT:SW_MEC_5GTH9" H 4450 2250 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4450 2250 50  0001 C CNN
	1    4450 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 850  4000 950 
Connection ~ 4000 950 
Wire Wire Line
	4000 950  4000 1300
Connection ~ 4000 1300
Wire Wire Line
	4000 1300 4000 1400
Connection ~ 4000 1400
Wire Wire Line
	4000 1400 4000 1750
Connection ~ 4000 1750
Wire Wire Line
	4000 1750 4000 1850
Wire Wire Line
	4000 2200 4000 2300
Wire Wire Line
	4650 850  4650 950 
Connection ~ 4650 950 
Wire Wire Line
	4650 950  4650 1350
Connection ~ 4650 1350
Wire Wire Line
	4650 1350 4650 1450
Connection ~ 4650 1450
Wire Wire Line
	4650 1450 4650 1850
Connection ~ 4650 1850
Wire Wire Line
	4650 1850 4650 1950
Wire Wire Line
	4000 1850 4000 2200
Connection ~ 4000 1850
Connection ~ 4000 2200
Connection ~ 4650 1950
Connection ~ 4650 2400
Wire Wire Line
	4650 2400 4650 2500
Connection ~ 4650 2300
Wire Wire Line
	4650 2400 4650 2300
Wire Wire Line
	4650 2300 4650 1950
$Comp
L Switch:SW_MEC_5E SW?
U 1 1 611117AA
P 4450 2400
AR Path="/611117AA" Ref="SW?"  Part="1" 
AR Path="/611353D7/611117AA" Ref="SW308"  Part="1" 
F 0 "SW308" H 4450 2785 50  0000 C CNN
F 1 "SW_MEC_5E" H 4450 2694 50  0000 C CNN
F 2 "Button_Switch_THT:SW_MEC_5GTH9" H 4450 2700 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4450 2700 50  0001 C CNN
	1    4450 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 2200 3600 2300
Wire Wire Line
	4250 950  4250 850 
Wire Wire Line
	4250 1450 4250 1350
Wire Wire Line
	4250 1850 4250 1950
Wire Wire Line
	4250 2400 4250 2300
Wire Wire Line
	4000 2500 4000 2300
Connection ~ 4000 2300
Wire Wire Line
	3600 850  3600 950 
Connection ~ 3600 950 
Wire Wire Line
	3600 1300 3600 1400
Wire Wire Line
	4250 950  4150 950 
Wire Wire Line
	4150 950  4150 1100
Connection ~ 4250 950 
Wire Wire Line
	3600 1850 3600 1750
Connection ~ 4250 2400
NoConn ~ 1800 5800
NoConn ~ 1800 6200
NoConn ~ 8300 5650
NoConn ~ 8300 6050
NoConn ~ 8300 6250
NoConn ~ 8300 5850
Wire Wire Line
	2150 2200 2150 2300
Wire Wire Line
	2150 4300 2150 4400
Wire Wire Line
	2200 6400 2200 6500
Wire Wire Line
	2750 1700 2750 2200
$EndSCHEMATC
