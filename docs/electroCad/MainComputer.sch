EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
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
L RF_Module:ESP32-WROOM-32 U?
U 1 1 6118D375
P 7750 3100
AR Path="/6118D375" Ref="U?"  Part="1" 
AR Path="/6112CB36/6118D375" Ref="U?"  Part="1" 
F 0 "U?" H 7750 4681 50  0000 C CNN
F 1 "ESP32-WROOM-32" H 7750 4590 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 7750 1600 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf" H 7450 3150 50  0001 C CNN
	1    7750 3100
	1    0    0    -1  
$EndComp
$Comp
L Connector:DB15_Male J?
U 1 1 611932EE
P 3200 3050
F 0 "J?" H 3355 3096 50  0000 L CNN
F 1 "DB15_Male" H 3355 3005 50  0000 L CNN
F 2 "" H 3200 3050 50  0001 C CNN
F 3 " ~" H 3200 3050 50  0001 C CNN
	1    3200 3050
	1    0    0    -1  
$EndComp
$EndSCHEMATC
