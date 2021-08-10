EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 4
Title "SER4"
Date "2021-08-07"
Rev "0.1"
Comp "Solar Energy Car Racers"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "Main Unit"
$EndDescr
$Comp
L ser4:Huzzah32feather ESP32
U 1 1 611BAF24
P 7750 2250
F 0 "ESP32" H 7750 2365 50  0000 C CNN
F 1 "Huzzah32feather" H 7750 2274 50  0000 C CNN
F 2 "" H 7750 2250 50  0001 C CNN
F 3 "" H 7750 2250 50  0001 C CNN
	1    7750 2250
	1    0    0    -1  
$EndComp
$Comp
L ser4:PCF8574Wave IOExt0
U 1 1 6110207B
P 2250 5800
F 0 "IOExt0" H 2308 5865 50  0000 C CNN
F 1 "PCF8574Wave" H 2308 5774 50  0000 C CNN
F 2 "" H 2250 5800 50  0001 C CNN
F 3 "" H 2250 5800 50  0001 C CNN
	1    2250 5800
	1    0    0    -1  
$EndComp
$Comp
L ser4:PCF8574Wave IOExt1
U 1 1 61102B5B
P 3700 5800
F 0 "IOExt1" H 3758 5865 50  0000 C CNN
F 1 "PCF8574Wave" H 3758 5774 50  0000 C CNN
F 2 "" H 3700 5800 50  0001 C CNN
F 3 "" H 3700 5800 50  0001 C CNN
	1    3700 5800
	1    0    0    -1  
$EndComp
$Comp
L ser4:ADS1115Grove ADC1
U 1 1 611014BF
P 3600 4400
F 0 "ADC1" H 3658 4515 50  0000 C CNN
F 1 "ADS1115Grove" H 3658 4424 50  0000 C CNN
F 2 "" H 3600 4400 50  0001 C CNN
F 3 "" H 3600 4400 50  0001 C CNN
	1    3600 4400
	1    0    0    -1  
$EndComp
$Comp
L ser4:ADS1115Grove ADC0
U 1 1 6110100E
P 2300 4450
F 0 "ADC0" H 2358 4565 50  0000 C CNN
F 1 "ADS1115Grove" H 2358 4474 50  0000 C CNN
F 2 "" H 2300 4450 50  0001 C CNN
F 3 "" H 2300 4450 50  0001 C CNN
	1    2300 4450
	1    0    0    -1  
$EndComp
Text Notes 800  1350 0    50   ~ 0
Neu angelegte Ser4 Elemente sind mit dem Tag SER4 versehen.\nNoch nicht in Library gefunden oder angelegt:\n- Micro SD Card SSD1306\n- RTC\n- Can Transeiver sn65hcd232\n- Digipot DS1803\n- Gyro&Acc BM088\nDiverse Stecker/Buchsen
$Comp
L Connector:DB15_Male_HighDensity J?
U 1 1 6111C69C
P 2000 3300
F 0 "J?" H 2000 4167 50  0000 C CNN
F 1 "DB15_Male_HighDensity" H 2000 4076 50  0000 C CNN
F 2 "" H 1050 3700 50  0001 C CNN
F 3 " ~" H 1050 3700 50  0001 C CNN
	1    2000 3300
	1    0    0    -1  
$EndComp
$Comp
L DS18030-100+:DS18030-100+ IC?
U 1 1 611299BB
P 5100 4550
F 0 "IC?" H 5650 4815 50  0000 C CNN
F 1 "DS18030-100+" H 5650 4724 50  0000 C CNN
F 2 "DIP762W56P254L1918H457Q16N" H 6050 4650 50  0001 L CNN
F 3 "http://docs-emea.rs-online.com/webdocs/0a29/0900766b80a29b9d.pdf" H 6050 4550 50  0001 L CNN
F 4 "DS18030-100+, Digital Potentiometer 100 k 256-Position Linear 2-channel Serial-2 Wire 16-Pin PDIP N" H 6050 4450 50  0001 L CNN "Description"
F 5 "4.572" H 6050 4350 50  0001 L CNN "Height"
F 6 "Maxim Integrated" H 6050 4250 50  0001 L CNN "Manufacturer_Name"
F 7 "DS18030-100+" H 6050 4150 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "700-DS18030-100" H 6050 4050 50  0001 L CNN "Mouser Part Number"
F 9 "https://www.mouser.co.uk/ProductDetail/Maxim-Integrated/DS18030-100%2b?qs=wTZ%2FFzl837ZzSoi8Pd5T4g%3D%3D" H 6050 3950 50  0001 L CNN "Mouser Price/Stock"
F 10 "" H 6050 3850 50  0001 L CNN "Arrow Part Number"
F 11 "" H 6050 3750 50  0001 L CNN "Arrow Price/Stock"
	1    5100 4550
	1    0    0    -1  
$EndComp
$EndSCHEMATC
