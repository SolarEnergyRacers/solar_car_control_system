# Boot Screens

## Boot Display Log - 2021.08.03

```
rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:2
load:0x3fff0018,len:7072
load:0x40078000,len:14052
ho 0 tail 12 room 4
load:0x40080400,len:4304
entry 0x400806e8
I (66) boot: Chip Revision: 3
I (67) boot_comm: chip revision: 3, min. bootloader chip revision: 0
I (40) boot: ESP-IDF 3.40001.200521 2nd stage bootloader
I (40) boot: compile time 20:33:39
I (40) boot: Enabling RNG early entropy source...
I (45) boot: SPI Speed      : 40MHz
I (49) boot: SPI Mode       : DIO
I (53) boot: SPI Flash Size : 4MB
I (57) boot: Partition Table:
I (61) boot: ## Label            Usage          Type ST Offset   Length
I (68) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (76) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (83) boot:  2 factory          factory app      00 00 00010000 00100000
I (91) boot: End of partition table
I (95) boot_comm: chip revision: 3, min. application chip revision: 0
I (102) esp_image: segment 0: paddr=0x00010020 vaddr=0x3f400020 size=0x1ddc4 (122308) map
I (155) esp_image: segment 1: paddr=0x0002ddec vaddr=0x3ffb0000 size=0x02170 (  8560) load
I (158) esp_image: segment 2: paddr=0x0002ff64 vaddr=0x40080000 size=0x000ac (   172) load
I (161) esp_image: segment 3: paddr=0x00030018 vaddr=0x400d0018 size=0x24690 (149136) map
I (224) esp_image: segment 4: paddr=0x000546b0 vaddr=0x400800ac size=0x0bd08 ( 48392) load
I (252) boot: Loaded app from partition at offset 0x10000
I (252) boot: Disabling RNG early entropy source...
I (252) cpu_start: Pro cpu up.
I (256) cpu_start: Application information:
I (261) cpu_start: Project name:     solar_car_control_system
I (267) cpu_start: App version:      7fd4144
I (272) cpu_start: Compile time:     Aug  3 2021 20:48:17
I (278) cpu_start: ELF file SHA256:  cb0798c030c444d7...
I (284) cpu_start: ESP-IDF:          3.40001.200521
I (290) cpu_start: Starting app cpu, entry point is 0x400821e8
I (0) cpu_start: App cpu up.
I (300) heap_init: Initializing. RAM available for dynamic allocation:
I (307) heap_init: At 3FFAE6E0 len 00001920 (6 KiB): DRAM
I (313) heap_init: At 3FFB3A90 len 0002C570 (177 KiB): DRAM
I (319) heap_init: At 3FFE0440 len 00003AE0 (14 KiB): D/IRAM
I (326) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM
I (332) heap_init: At 4008BDB4 len 0001424C (80 KiB): IRAM
I (338) cpu_start: Pro cpu start user code
I (358) spi_flash: detected chip: generic
I (359) spi_flash: flash io: dio
I (359) cpu_start: Starting scheduler on PRO CPU.
I (0) cpu_start: Starting scheduler on APP CPU.
�
--------------------
esp32dev + free RTOS

--------------------
This is ESP32 chip with 2 CPU cores, WiFi/BT/BLE, silicon revision 3, 4MB external flash
[v] OneWire inited: ONEWIRE_PIN=2.
[v] I2C inited: I2C_SDA=23, I2C_SCL=22, I2C_FREQ=100000.
    Scanning I2C addresses:
    00 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    20 21 22 -- -- -- -- -- 28 -- -- -- -- -- -- -- 
    -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    -- -- -- -- -- -- -- -- -- 49 -- -- -- -- -- -- 
    -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    Scan completed: 6 I2C devices found.
[v] SPI inited: SPI_CLK=5, SPI_MOSI=18, SPI_MISO=19.
    Init 'Display0 (driver display)'... done.
      Display0 (driver display) initializing...
      Display Power Mode: 0x94
      MADCTL Mode:        0x48
      Pixel Format:       0x5
      Image Format:       0x80
      Self Diagnostic:    0xc0
[v] Display0 (driver display) inited: screen ILI9341 with 320 x 240.
[v] Indicator handler inited
[v] Command handler inited
    Init 'ADC[2]' with address 0x49 ...
    Max voltage=6.144000 with multiplier=0.000188
      [ADS1x15] AIN0 --> 11996: 2.249319mV
      [ADS1x15] AIN1 --> 3821: 0.716459mV
      [ADS1x15] AIN2 --> 3203: 0.600581mV
      [ADS1x15] AIN3 --> 3202: 0.600393mV
[v] ADC[2] initialized.
[v] IOExt2 inited.

-----------------------------------------------------------------
Startup sequence(s) successful. System creating FreeRTOS tasks...
-----------------------------------------------------------------
 - create task 'Display0 (driver display)'... done.
 - create task 'Indicator'... done.
 - create task 'IOExt'... done.
 - create task 'CmdHandler'... done.
-----------------------------------------------------------------
Creating FreeRTOS tasks successful. System running.
-----------------------------------------------------------------

  Draw demo screen:
   - driver info
   - hazzard warn
   - spped
   - acceleration
   - increase arrow
   - decrease arrow
   - light1 on
   - light1 on
   - constant mode speed
   - drive direction forwards
   - battery
   - photovoltaic
   - motor
   - constant mode power
   - drive direction backwards
   - life sign
  End of demo screen.
lib/ADC/ADC.cpp                 : 89 adjust_min_acc_dec                  : Set MIN values set dec: 3860-->3850, acc: 12121-->12111
lib/ADC/ADC.cpp                 :186 read_adc_acc_dec                    : Dec (v0):  12196 --> 101 | Acc (v1): 12196 -->   0 | ACCEL-DISPLAY: -101 ==> set POT0 =1044 (dec( 3850-12000)), POT1 =  0 (acc(12111-15000))
lib/DAC/DAC.cpp                 : 60 set_pot                             : Write motor potentiometer [0x28] 0xa9 to 0 -- reread: 0
lib/DAC/DAC.cpp                 : 60 set_pot                             : Write motor potentiometer [0x28] 0xaa to 20 -- reread: 20
lib/ADC/ADC.cpp                 :186 read_adc_acc_dec                    : Dec (v0):   3884 -->   0 | Acc (v1): 12195 -->   0 | ACCEL-DISPLAY:   0 ==> set POT0 =  0 (dec( 3850-12000)), POT1 =  0 (acc(12111-15000))
lib/DAC/DAC.cpp                 : 60 set_pot                             : Write motor potentiometer [0x28] 0xa9 to 0 -- reread: 0
lib/DAC/DAC.cpp                 : 60 set_pot                             : Write motor potentiometer [0x28] 0xaa to 0 -- reread: 0
lib/ADC/ADC.cpp                 :186 read_adc_acc_dec                    : Dec (v0):  12194 --> 101 | Acc (v1): 12194 -->   0 | ACCEL-DISPLAY: -101 ==> set POT0 =1044 (dec( 3850-12000)), POT1 =  0 (acc(12111-15000))
lib/DAC/DAC.cpp                 : 60 set_pot                             : Write motor potentiometer [0x28] 0xa9 to 0 -- reread: 0
lib/DAC/DAC.cpp                 : 60 set_pot                             : Write motor potentiometer [0x28] 0xaa to 20 -- reread: 20
lib/ADC/ADC.cpp                 :186 read_adc_acc_dec                    : Dec (v0):   3884 -->   0 | Acc (v1): 12194 -->   0 | ACCEL-DISPLAY:   0 ==> set POT0 =  0 (dec( 3850-12000)), POT1 =  0 (acc(12111-15000))
lib/DAC/DAC.cpp                 : 60 set_pot                             : Write motor potentiometer [0x28] 0xa9 to 0 -- reread: 0
lib/DAC/DAC.cpp                 : 60 set_pot                             : Write motor potentiometer [0x28] 0xaa to 0 -- reread: 0
lib/ADC/ADC.cpp                 :186 read_adc_acc_dec                    : Dec (v0):  12188 --> 101 | Acc (v1): 12188 -->   0 | ACCEL-DISPLAY: -101 ==> set POT0 =1044 (dec( 3850-12000)), POT1 =  0 (acc(12111-15000))
lib/DAC/DAC.cpp                 : 60 set_pot                             : Write motor potentiometer [0x28] 0xa9 to 0 -- reread: 0
lib/DAC/DAC.cpp                 : 60 set_pot                             : Write motor potentiometer [0x28] 0xaa to 20 -- reread: 20
lib/ADC/ADC.cpp                 :186 read_adc_acc_dec                    : Dec (v0):   3880 -->   0 | Acc (v1): 12189 -->   0 | ACCEL-DISPLAY:   0 ==> set POT0 =  0 (dec( 3850-12000)), POT1 =  0 (acc(12111-15000))
lib/DAC/DAC.cpp                 : 60 set_pot                             : Write motor potentiometer [0x28] 0xa9 to 0 -- reread: 0
lib/DAC/DAC.cpp                 : 60 set_pot                             : Write motor potentiometer [0x28] 0xaa to 0 -- reread: 0
lib/ADC/ADC.cpp                 :186 read_adc_acc_dec                    : Dec (v0):   3881 -->   0 | Acc (v1):  3881 --> -282 | ACCEL-DISPLAY: -282 ==> set POT0 =2916 (dec( 3850-12000)), POT1 =  0 (acc(12111-15000))
lib/DAC/DAC.cpp                 : 60 set_pot                             : Write motor potentiometer [0x28] 0xa9 to 0 -- reread: 0
lib/DAC/DAC.cpp                 : 60 set_pot                             : Write motor potentiometer [0x28] 0xaa to 100 -- reread: 100
lib/ADC/ADC.cpp                 :186 read_adc_acc_dec                    : Dec (v0):   3881 -->   0 | Acc (v1): 12189 -->   0 | ACCEL-DISPLAY:   0 ==> set POT0 =  0 (dec( 3850-12000)), POT1 =  0 (acc(12111-15000))
lib/DAC/DAC.cpp                 : 60 set_pot                             : Write motor potentiometer [0x28] 0xa9 to 0 -- reread: 0
lib/DAC/DAC.cpp                 : 60 set_pot                             : Write motor potentiometer [0x28] 0xaa to 0 -- reread: 0
lib/ADC/ADC.cpp                 :186 read_adc_acc_dec                    : Dec (v0):  12189 --> 101 | Acc (v1): 12189 -->   0 | ACCEL-DISPLAY: -101 ==> set POT0 =1044 (dec( 3850-12000)), POT1 =  0 (acc(12111-15000))

```

### Working parts

- [x] Demo Screen
- [x] Driver Display 
  - [x] Driver Info
  - [x] Driver Warning	
  - [x] Acceleration/recuperation
  - [x] Speed
  - [x] Bat voltage
  - [x] PV ampere
  - [x] Motor ampere
  - [x] forward/backward
  - [x] Position light
  - [x] Beam light
  - [x] Life sign
- [x] indicator buttons
- [x] Light button
- [x] speed/power button
- [x] set speed/power button
- [x] horn
- [x] witch screen button
- [x] left paddle 0 ... -74
- [x] right paddle 0 .. 52
- [x] Gyroscope

## Boot Display Log - 2022.07.08

```log
Hard resetting via RTS pin...
=========================================================================================================================== [SUCCESS] Took 112.89 seconds ===========================================================================================================================
--- Available filters and text transformations: colorize, debug, default, direct, esp32_exception_decoder, hexlify, log2file, nocontrol, printable, send_on_enter, time
--- More details at https://bit.ly/pio-monitor-filters
--- Miniterm on /dev/ttyUSB0  115200,8,N,1 ---
--- Quit: Ctrl+C | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
[  ] Init 'Uart (SerialWire-SerialRadio)'...
     ---Serial------------
     Serial TX0 is on pin: 1
     Serial RX0 is on pin: 3
     Serial Baud Rate:     115200
     ---Serial2 HC-12------
     Serial2 TX2 is on pin: 17
     Serial2 RX2 is on pin: 16
     Serial2 Baud Rate:     9600
     done.
[ok] Serial and Serial2 initialized.

--------------------
esp32dev + free RTOS
Solar Energy Car Racers SER4 Controller: $GIT_BRANCH_$GIT_COMMIT_HASH
--------------------
-chip info -------------------
This is ESP32 chip with 2 CPU cores, WiFi/BT/BLE, silicon revision 3, 4MB external flash
-gpio pin settings ----------
[  ] Init 'GPIputOutput' ...done.
[ok] SPI_CS for TFT and SD card set, GPIO initialized.
-init bus systems ------------
[  ] Init 'SPI bus' with: SPI_CLK=5, SPI_MOSI=18, SPI_MISO=19...
[E][esp32-hal-cpu.c:125] removeApbChangeCallback(): not found func=401078DC arg=3FFB01A4
[ok] SPIBus initialized.
[  ] Init 'OneWireBus'...
[ok] OneWireBus initialized.  ONEWIRE_PIN=12
[  ] Init 'I2C Bus'...
     I2C inited: I2C_SDA=23, I2C_SCL=22, I2C_FREQ=50000.
     Scanning I2C addresses:
     00 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
     -- -- -- -- -- -- -- -- -- 19 -- -- -- -- -- -- 
     20 21 -- -- -- -- -- -- 28 -- -- -- -- -- -- -- 
     -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
     -- -- -- -- -- -- -- -- 48 49 4a -- -- -- -- -- 
     -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
     -- -- -- -- -- -- -- -- 68 69 -- -- -- -- -- -- 
     -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
     Scan completed: 10 I2C devices found.
     Expected addresses:
     Address | Device                               | Location      
     ------- | ------------------------------------ | --------------
      0x00   | ESP32 I2C master                     | main noard    
      0x19   | BMI088, 6-axis inertial sensor, acc  | main board    
      0x20   | MCP23017, Extended digital IOs       | main board    
      0x21   | MCP23017, Extended digital IOs       | steering wheel
      0x28   | DS1803, digital analog coder         | main board    
      0x48   | ADS1115, analog digital coder        | main board    
      0x49   | ADS1115, analog digital coder        | main board    
      0x4a   | ADS1115, analog digital coder        | steering wheel
      0x68   | DS1307, real time clock              | main board    
      0x69   | BMI088, 6-axis inertial sensor, gyro | main board    
[ok] I2CBus initialized.
[  ] Init 'EngineerDisplay'...
     Setup 'ILI9341' for 'EngineerDisplay' with: SPI_CLK=5, SPI_MOSI=18, SPI_MISO=19, SPI_CS_TFT=32
     ILI9341_RDMADCTL:   0xa
     ILI9341_RDPIXFMT:   0x1
     ILI9341_RDIMGFMT:   0xe0
     ILI9341_RDSELFDIAG: 0xf0
     ILI9341_RDMODE:     0xe5
[ok] Display initialized.  Screen 'ILI9341' 240x320.     Status: ENGINEER_CONSOLE
[  ] Init IOExt devices...
     DeviceNr [0]:
     ok IOExt[0]
     DeviceNr [1]:
     ok IOExt[1]
[ok] IOExt initialized.
[  ] Init 'SDCard'...
     No SD card detected!
[--] SDCard initialized.
Start reading CONFIG.INI:SER4CONF.INI
     No SD card detected!
WARN: No readable configfile: 'SER4CONF.INI' found. Using coded settings.
     No SD card detected!
[  ] Init 'CmdHandler'...
[ok] CmdHandler initialized.
[  ] Init 'RTC'...
     DS1307_ADDRESS 68
     [INFO] rtc compile date/time: 07/08/2022 15:21:07
     [INFO] rtc time newer than compile time. Updating esp32time DateTime.
[ok] RTC initialized.
[  ] Init Indicator...done.
[ok] Indicator initialized.
[  ] Init 'DAC'...
     DAC initialized with I2C_ADDRESS_DS1803=28.
[ok] DAC initialized.
     Init 'ADC[0]' with address 0x48 ...
        Max voltage=6.144000 with multiplier=0.000188
          [ADS1x15] AIN0 --> 4: 0.000750mV
          [ADS1x15] AIN1 --> 2437: 0.456951mV
          [ADS1x15] AIN2 --> 2446: 0.458639mV
          [ADS1x15] AIN3 --> 2438: 0.457139mV
     ok ADC[0] at 0x48 inited.
     Init 'ADC[1]' with address 0x49 ...
        Max voltage=6.144000 with multiplier=0.000188
          [ADS1x15] AIN0 --> 2361: 0.442701mV
          [ADS1x15] AIN1 --> 2380: 0.446264mV
          [ADS1x15] AIN2 --> 2383: 0.446826mV
          [ADS1x15] AIN3 --> 2376: 0.445514mV
     ok ADC[1] at 0x49 inited.
     Init 'ADC[2]' with address 0x4a ...
        Max voltage=6.144000 with multiplier=0.000188
          [ADS1x15] AIN0 --> -2: -0.000375mV
          [ADS1x15] AIN1 --> 15906: 2.982466mV
          [ADS1x15] AIN2 --> 2687: 0.503828mV
          [ADS1x15] AIN3 --> 2672: 0.501015mV
     ok ADC[2] at 0x4a inited.
[ok] ADC initialized.
[  ] Init GyroAcc...
     BMI088 is connected
     BMI088 is initialized
[ok] GyroAcc initialized.
[  ] Init CANBus...
     CANBus with rx=19m tx=1a inited.
[ok] CANBus initialized.

-----------------------------------------------------------------
Startup sequence(s) successful. System creating FreeRTOS tasks...
-----------------------------------------------------------------

[  ] Create IOExt as prio-4-task (sleep_polling=100ms, stack=4000) ... done.
[ok] IOExt started (100ms).
[  ] Create Indicator as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] Indicator started (330ms).
[  ] Create ADC as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] ADC started (330ms).
[  ] Create GyroAcc as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] GyroAcc started (330ms).
[  ] Create RTC as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] RTC started (330ms). RTC Time: 07/08/2022 15:21:34
[  ] Create CmdHandler as prio-20-task (sleep_polling=350ms, stack=6000) ... done.
[ok] CmdHandler started (350ms).
[  ] Create CANBus as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] CANBus started (330ms).
[  ] Init 'CarControl'... done.
[ok] CarControl initialized.
[  ] Create CarControl as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] CarControl started (330ms).
[  ] Init 'CarSpeed'...
[ok] CarSpeed initialized.
[  ] Create CarSpeed as prio-10-task (sleep_polling=250ms, stack=2048) ... done.
[ok] CarSpeed started (250ms).
ets Jul 29 2019 12:21:46

rst:0x1 (POWERON_RESET),boot:0x17 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:2
load:0x3fff0018,len:7084
ho 0 tail 12 room 4
load:0x40078000,len:14052
load:0x40080400,len:4352
entry 0x400806ec
I (70) boot: Chip Revision: 3
I (70) boot_comm: chip revision: 3, min. bootloader chip revision: 0
I (40) boot: ESP-IDF 4.0.1 2nd stage bootloader
I (40) boot: compile time 23:58:28
I (40) boot: Enabling RNG early entropy source...
I (44) boot: SPI Speed      : 40MHz
I (48) boot: SPI Mode       : DIO
I (52) boot: SPI Flash Size : 4MB
I (56) boot: Partition Table:
I (60) boot: ## Label            Usage          Type ST Offset   Length
I (67) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (75) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (82) boot:  2 factory          factory app      00 00 00010000 00100000
I (90) boot: End of partition table
I (94) boot_comm: chip revision: 3, min. application chip revision: 0
I (101) esp_image: segment 0: paddr=0x00010020 vaddr=0x3f400020 size=0x377a4 (227236) map
I (191) esp_image: segment 1: paddr=0x000477cc vaddr=0x3ffb0000 size=0x0222c (  8748) load
I (195) esp_image: segment 2: paddr=0x00049a00 vaddr=0x40080000 size=0x06610 ( 26128) load
I (209) esp_image: segment 3: paddr=0x00050018 vaddr=0x400d0018 size=0x82a28 (535080) map
I (401) esp_image: segment 4: paddr=0x000d2a48 vaddr=0x40086610 size=0x0561c ( 22044) load
I (418) boot: Loaded app from partition at offset 0x10000
I (418) boot: Disabling RNG early entropy source...
I (418) cpu_start: Pro cpu up.
I (422) cpu_start: Application information:
I (427) cpu_start: Project name:     solar_car_control_system
I (433) cpu_start: App version:      4d09524
I (438) cpu_start: Compile time:     Jul  7 2022 23:49:44
I (444) cpu_start: ELF file SHA256:  634b730938ae0688...
I (450) cpu_start: ESP-IDF:          4.0.1
I (455) cpu_start: Starting app cpu, entry point is 0x4008219c
I (0) cpu_start: App cpu up.
I (465) heap_init: Initializing. RAM available for dynamic allocation:
I (472) heap_init: At 3FFAE6E0 len 00001920 (6 KiB): DRAM
I (478) heap_init: At 3FFB72C8 len 00028D38 (163 KiB): DRAM
I (484) heap_init: At 3FFE0440 len 00003AE0 (14 KiB): D/IRAM
I (491) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM
I (497) heap_init: At 4008BC2C len 000143D4 (80 KiB): IRAM
I (503) cpu_start: Pro cpu start user code
I (530) spi_flash: detected chip: generic
I (531) spi_flash: flash io: dio
I (531) cpu_start: Starting scheduler on PRO CPU.
I (0) cpu_start: Starting scheduler on APP CPU.
�[  ] Init 'Uart (SerialWire-SerialRadio)'...
     ---Serial------------
     Serial TX0 is on pin: 1
     Serial RX0 is on pin: 3
     Serial Baud Rate:     115200
     ---Serial2 HC-12------
     Serial2 TX2 is on pin: 17
     Serial2 RX2 is on pin: 16
     Serial2 Baud Rate:     9600
     done.
[ok] Serial and Serial2 initialized.

--------------------
esp32dev + free RTOS
Solar Energy Car Racers SER4 Controller: $GIT_BRANCH_$GIT_COMMIT_HASH
--------------------
-chip info -------------------
This is ESP32 chip with 2 CPU cores, WiFi/BT/BLE, silicon revision 3, 4MB external flash
-gpio pin settings ----------
[  ] Init 'GPIputOutput' ...done.
[ok] SPI_CS for TFT and SD card set, GPIO initialized.
-init bus systems ------------
[  ] Init 'SPI bus' with: SPI_CLK=5, SPI_MOSI=18, SPI_MISO=19...
[E][esp32-hal-cpu.c:125] removeApbChangeCallback(): not found func=401078DC arg=3FFB01A4
[ok] SPIBus initialized.
[  ] Init 'OneWireBus'...
[ok] OneWireBus initialized.  ONEWIRE_PIN=12
[  ] Init 'I2C Bus'...
     I2C inited: I2C_SDA=23, I2C_SCL=22, I2C_FREQ=50000.
     Scanning I2C addresses:
     00 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
     -- -- -- -- -- -- -- -- -- 19 -- -- -- -- -- -- 
     20 21 -- -- -- -- -- -- 28 -- -- -- -- -- -- -- 
     -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
     -- -- -- -- -- -- -- -- 48 49 4a -- -- -- -- -- 
     -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
     -- -- -- -- -- -- -- -- 68 69 -- -- -- -- -- -- 
     -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
     Scan completed: 10 I2C devices found.
     Expected addresses:
     Address | Device                               | Location      
     ------- | ------------------------------------ | --------------
      0x00   | ESP32 I2C master                     | main noard    
      0x19   | BMI088, 6-axis inertial sensor, acc  | main board    
      0x20   | MCP23017, Extended digital IOs       | main board    
      0x21   | MCP23017, Extended digital IOs       | steering wheel
      0x28   | DS1803, digital analog coder         | main board    
      0x48   | ADS1115, analog digital coder        | main board    
      0x49   | ADS1115, analog digital coder        | main board    
      0x4a   | ADS1115, analog digital coder        | steering wheel
      0x68   | DS1307, real time clock              | main board    
      0x69   | BMI088, 6-axis inertial sensor, gyro | main board    
[ok] I2CBus initialized.
[  ] Init 'EngineerDisplay'...
     Setup 'ILI9341' for 'EngineerDisplay' with: SPI_CLK=5, SPI_MOSI=18, SPI_MISO=19, SPI_CS_TFT=32
     ILI9341_RDMADCTL:   0xa
     ILI9341_RDPIXFMT:   0x1
     ILI9341_RDIMGFMT:   0xe0
     ILI9341_RDSELFDIAG: 0xf0
     ILI9341_RDMODE:     0xe5
[ok] Display initialized.  Screen 'ILI9341' 240x320.     Status: ENGINEER_CONSOLE
[  ] Init IOExt devices...
     DeviceNr [0]:
     ok IOExt[0]
     DeviceNr [1]:
     ok IOExt[1]
[ok] IOExt initialized.
[  ] Init 'SDCard'...
     Mounting SD card ...
     SD card mounted.
[ok] SDCard initialized.
Start reading CONFIG.INI:SER4CONF.INI
     Mounting SD card ...
     SD card mounted.
     Log file opend for append.
[  ] Init 'CmdHandler'...
[ok] CmdHandler initialized.
[  ] Init 'RTC'...
     DS1307_ADDRESS 68
     [INFO] rtc compile date/time: 07/08/2022 15:21:07
     [INFO] rtc time newer than compile time. Updating esp32time DateTime.
[ok] RTC initialized.
[  ] Init Indicator...done.
[ok] Indicator initialized.
[  ] Init 'DAC'...
     DAC initialized with I2C_ADDRESS_DS1803=28.
[ok] DAC initialized.
     Init 'ADC[0]' with address 0x48 ...
        Max voltage=6.144000 with multiplier=0.000188
          [ADS1x15] AIN0 --> 4: 0.000750mV
          [ADS1x15] AIN1 --> 2437: 0.456951mV
          [ADS1x15] AIN2 --> 2440: 0.457514mV
          [ADS1x15] AIN3 --> 2441: 0.457701mV
     ok ADC[0] at 0x48 inited.
     Init 'ADC[1]' with address 0x49 ...
        Max voltage=6.144000 with multiplier=0.000188
          [ADS1x15] AIN0 --> 2366: 0.443639mV
          [ADS1x15] AIN1 --> 2380: 0.446264mV
          [ADS1x15] AIN2 --> 2370: 0.444389mV
          [ADS1x15] AIN3 --> 2372: 0.444764mV
     ok ADC[1] at 0x49 inited.
     Init 'ADC[2]' with address 0x4a ...
        Max voltage=6.144000 with multiplier=0.000188
          [ADS1x15] AIN0 --> -6: -0.001125mV
          [ADS1x15] AIN1 --> 15927: 2.986404mV
          [ADS1x15] AIN2 --> 2704: 0.507015mV
          [ADS1x15] AIN3 --> 2682: 0.502890mV
     ok ADC[2] at 0x4a inited.
[ok] ADC initialized.
[  ] Init GyroAcc...
     BMI088 is connected
     BMI088 is initialized
[ok] GyroAcc initialized.
[  ] Init CANBus...
     CANBus with rx=19m tx=1a inited.
[ok] CANBus initialized.

-----------------------------------------------------------------
Startup sequence(s) successful. System creating FreeRTOS tasks...
-----------------------------------------------------------------

[  ] Create IOExt as prio-4-task (sleep_polling=100ms, stack=4000) ... done.
[ok] IOExt started (100ms).
[  ] Create Indicator as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] Indicator started (330ms).
[  ] Create ADC as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] ADC started (330ms).
[  ] Create GyroAcc as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] GyroAcc started (330ms).
[  ] Create RTC as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] RTC started (330ms). RTC Time: 07/08/2022 15:21:57
[  ] Create CmdHandler as prio-20-task (sleep_polling=350ms, stack=6000) ... done.
[ok] CmdHandler started (350ms).
[  ] Create CANBus as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] CANBus started (330ms).
[  ] Init 'CarControl'... done.
[ok] CarControl initialized.
[  ] Create CarControl as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] CarControl started (330ms).
[  ] Init 'CarSpeed'...
[ok] CarSpeed initialized.
[  ] Create CarSpeed as prio-10-task (sleep_polling=250ms, stack=2048) ... done.
[ok] CarSpeed started (250ms).
[  ] Create EngineerDisplay as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] EngineerDisplay started (1500ms).
[  ] Init 'DriverDisplay'...
     Setup 'ILI9341' for 'DriverDisplay' with: SPI_CLK=5, SPI_MOSI=18, SPI_MISO=19, SPI_CS_TFT=32
     ILI9341_RDMADCTL:   0xa
     ILI9341_RDPIXFMT:   0x1
     ILI9341_RDIMGFMT:   0xe0
     ILI9341_RDSELFDIAG: 0xf0
     ILI9341_RDMODE:     0xe5
[  ] Create DriverDisplay as prio-16-task (sleep_polling=330ms, stack=4096) ... done.
     Setup 'ILI9341' for 'DriverDisplay[ok] DriverDisplay started (330ms).' with: SPI_CLK=DRIVER_SETUP
5, SPI_MOSI=18, SPI_MISO=19, SPI_CS_TFT=32
     ILI9341_RDMADCTL:   0xa
     ILI9341_RDPIXFMT:   0x1
     ILI9341_RDIMGFMT:   0xe0
     ILI9341_RDSELFDIAG: 0xf0
     ILI9341_RDMODE:     0xe5
[v] 'DriverDisplay' inited: screen E ILI9341 with 240 x 320
-----------------------------------------------------------------
FreeRTOS tasks successfully created. System running.
-----------------------------------------------------------------
Battery On
PV On
MC On
EcoMowerMode Eco
Direction Forward
Break pedal pressed released
Horn Off
Increase constant mode target.
Decrease constant mode target.
SD card detected, try to start logging...
[  ] Init 'SDCard'...
[ok] SDCard initialized.
     Log file opend for append.
Request Paddle Adjust
 paddle adjust:
 24 paddle adjust:
 23 paddle adjust:
 22 paddle adjust:
 21 paddle adjust:
 20 paddle adjust:
 19 paddle adjust:
 18 paddle adjust:
 17 paddle adjust:
 16 paddle adjust:
 15 paddle adjust:
 14 paddle adjust:
 13 paddle adjust:
 12 paddle adjust:
 11 paddle adjust:
 10 paddle adjust:
  9 paddle adjust:
  8 paddle adjust:
  7 paddle adjust:
  6 paddle adjust:
  5 paddle adjust:
  4 paddle adjust:
  3 paddle adjust:
  2 paddle adjust:
  1 paddle adjust:
  0
    ==>dec 18131-20450 == acc  6860-18726
```

## Boot Display LOG - 2022.08.07

```log
Building in release mode
Retrieving maximum program size .pio/build/esp32dev/firmware.elf
Checking size .pio/build/esp32dev/firmware.elf
Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
RAM:   [=         ]   9.3% (used 30472 bytes from 327680 bytes)
Flash: [========= ]  87.0% (used 911985 bytes from 1048576 bytes)
Configuring upload protocol...
AVAILABLE: cmsis-dap, esp-bridge, esp-prog, espota, esptool, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa
CURRENT: upload_protocol = esptool
Looking for upload port...
Using manually specified: /dev/ttyUSB0
Uploading .pio/build/esp32dev/firmware.bin
esptool.py v3.3
Serial port /dev/ttyUSB0
Connecting.......
Chip is ESP32-D0WDQ6 (revision 1)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
Crystal is 40MHz
MAC: 0c:b8:15:e2:d1:1c
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 460800
Changed.
Configuring flash size...
Flash will be erased from 0x00001000 to 0x00007fff...
Flash will be erased from 0x00008000 to 0x00008fff...
Flash will be erased from 0x00010000 to 0x000eefff...
Compressed 25280 bytes to 15794...
Writing at 0x00001000... (100 %)
Wrote 25280 bytes (15794 compressed) at 0x00001000 in 0.9 seconds (effective 233.0 kbit/s)...
Hash of data verified.
Compressed 3072 bytes to 103...
Writing at 0x00008000... (100 %)
Wrote 3072 bytes (103 compressed) at 0x00008000 in 0.1 seconds (effective 337.3 kbit/s)...
Hash of data verified.
Compressed 912368 bytes to 436508...
Writing at 0x00010000... (3 %)
Writing at 0x00019a79... (7 %)
Writing at 0x00024776... (11 %)
Writing at 0x0003147d... (14 %)
Writing at 0x0004e5d2... (18 %)
Writing at 0x00059d8e... (22 %)
Writing at 0x0005f9b0... (25 %)
Writing at 0x00067405... (29 %)
Writing at 0x0006d7d4... (33 %)
Writing at 0x00073e4b... (37 %)
Writing at 0x0007b149... (40 %)
Writing at 0x00083cf8... (44 %)
Writing at 0x0008b1df... (48 %)
Writing at 0x00091f06... (51 %)
Writing at 0x00097eb2... (55 %)
Writing at 0x0009e89f... (59 %)
Writing at 0x000a47a0... (62 %)
Writing at 0x000aa463... (66 %)
Writing at 0x000b07ae... (70 %)
Writing at 0x000b87f1... (74 %)
Writing at 0x000c00ce... (77 %)
Writing at 0x000c77b2... (81 %)
Writing at 0x000cd2d5... (85 %)
Writing at 0x000d3a09... (88 %)
Writing at 0x000dc837... (92 %)
Writing at 0x000e246d... (96 %)
Writing at 0x000eadf5... (100 %)
Wrote 912368 bytes (436508 compressed) at 0x00010000 in 11.7 seconds (effective 626.3 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
======================================================================= [SUCCESS] Took 119.09 seconds =======================================================================
--- Terminal on /dev/ttyUSB0 | 115200 8-N-1
--- Available filters and text transformations: colorize, debug, default, direct, esp32_exception_decoder, hexlify, log2file, nocontrol, printable, send_on_enter, time
--- More details at https://bit.ly/pio-monitor-filters
--- Quit: Ctrl+C | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H
[  ] Init 'UartSerial'...
     ---Serial------------
     Serial TX0 is on pin: 1
     Serial RX0 is on pin: 3
     Serial Baud Rate:     115200
I (1577) uart: queue free spaces: 20
     ---Serial2 HC-12------
     Serial2 TX2 is on pin: 17
     Serial2 RX2 is on pin: 16
     Serial2 Baud Rate:     9600
     done.
[ok] Serial and Serial2 initialized.

--------------------
esp32dev + free RTOS
Solar Energy Car Racers SER4 Controller
--------------------
-chip info -------------------
This is ESP32 chip with 2 CPU cores, WiFi/BT/BLE, silicon revision 1, 4MB external flash
-gpio pin settings ----------
[  ] Init 'GPInputOutput' ...
I (2997) gpio: GPIO[14]| InputEn: 1| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
I (2997) gpio: GPIO[32]| InputEn: 1| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
done.
[ok] SPI_CS for TFT (32) and SD card (14) set, GPIO initialized.
-init bus systems ------------
[  ] Init 'SPI bus' with: SPI_CLK=5, SPI_MOSI=18, SPI_MISO=19...
I (3377) gpio: GPIO[5]| InputEn: 1| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
I (3377) gpio: GPIO[19]| InputEn: 1| OutputEn: 0| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
I (3387) gpio: GPIO[18]| InputEn: 1| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
[ok] SPIBus initialized.
[  ] Init 'OneWireBus'...
I (3457) gpio: GPIO[12]| InputEn: 1| OutputEn: 0| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
[ok] OneWireBus initialized.  ONEWIRE_PIN=12
[  ] Init 'I2CBus'...
     I2C inited: I2C_SDA=23, I2C_SCL=22, I2C_FREQ=200000.
     Scanning I2C addresses:
     00 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
     -- -- -- -- -- -- -- -- -- 19 -- -- -- -- -- -- 
     20 21 -- -- -- -- -- -- 28 -- -- -- -- -- -- -- 
     -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
     -- -- -- -- -- -- -- -- 48 49 4a -- -- -- -- -- 
     -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
     -- -- -- -- -- -- -- -- 68 69 -- -- -- -- -- -- 
     -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
     Scan completed: 10 I2C devices found.
     Expected addresses:
     Address | Device                               | Location      
     ------- | ------------------------------------ | --------------
      0x00   | ESP32 I2C master                     | main noard    
      0x19   | BMI088, 6-axis inertial sensor, acc  | main board    
      0x20   | MCP23017, Extended digital IOs       | main board    
      0x21   | MCP23017, Extended digital IOs       | steering wheel
      0x28   | DS1803, digital analog coder         | main board    
      0x48   | ADS1115, analog digital coder        | main board    
      0x49   | ADS1115, analog digital coder        | main board    
      0x4a   | ADS1115, analog digital coder        | steering wheel
      0x68   | DS1307, real time clock              | main board    
      0x69   | BMI088, 6-axis inertial sensor, gyro | main board    
[ok] I2CBus initialized.
[  ] Init 'Display'...
     Setup 'ILI9341' for 'EngineerDisplay' with: SPI_CLK=5, SPI_MOSI=18, SPI_MISO=19, SPI_CS_TFT=32
I (5257) gpio: GPIO[32]| InputEn: 1| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
I (5257) gpio: GPIO[4]| InputEn: 1| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
I (5267) gpio: GPIO[21]| InputEn: 1| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
     ILI9341_RDMADCTL:   0xa
     ILI9341_RDPIXFMT:   0x1
     ILI9341_RDIMGFMT:   0xe0
     ILI9341_RDSELFDIAG: 0xf0
     ILI9341_RDMODE:     0xe5
[ok] Display initialized.  Screen 'ILI9341' 240x320.     Status: ENGINEER_CONSOLE
[  ] Init IOExt devices...
     DeviceNr [0]:
     ok IOExt[0]
     DeviceNr [1]:
     ok IOExt[1]
I (6287) gpio: GPIO[33]| InputEn: 1| OutputEn: 0| OpenDrain: 0| Pullup: 1| Pulldown: 0| Intr:0 
[ok] IOExt initialized.
SD card removed.
[  ] Init 'SDCard'...
     No SD card detected!
[--] SDCard initialized.
---01------------------------------------------
Start reading CONFIG.INI:SER4CONF.INI
     No SD card detected!
WARN: No readable configfile: 'SER4CONF.INI' found. Using coded settings.
====SER4 Car Status====$GIT_BRANCH_$GIT_COMMIT_HASH==70.0.1_0:0:6====uptime:00:00:06s====Thu Jan  1 00:00:06 1970
==State after reading SER4CONFIG.INI
Display Status ........ ENGINEER_CONSOLE
Speed ................. 0
Acceleration locked ... true
Acceleration .......... 0
Deceleration .......... 0
Acceleration Display... 0
Break pedal pressed ... false
Battery On............. 0
Battery Voltage ....... 0
Battery Current........ 0
Battery Errors .........[]
Battery Precharge State ERROR
Photo Voltaic On ...... 0
MPPT1 Current ......... 0
MPPT2 Current ......... 0
MPPT3 Current ......... 0
Photo Voltaic Current . 0
Photo Reference Cell .. 0
Acceleration Display .. 0
Break pedal pressed ... false
Photo Voltaic On ...... 0
Motor On .............. 0
Motor Current ......... 0
Drive Direction ....... fwd
Green Light ........... 0

Fan ................... 0
------------------------
Indicator ............. OFF
Constant Mode On ...... false
Constant Mode ......... SPEED
Target Speed .......... 0
Target Power .......... 0
SD Card detected....... false(0)
Info Last ............. [STATUS] Acceleration.still locked!
Speed Arrow ........... OFF
Light ................. OFF
IO .................... 0: 1101-1000 | 0000-0111 | 1: 1111-1111 | 1111-1011
Log file name ......... /SER4DATA.CSV
Log file persiod ...... 1000
Log file interval ..... 1
Sleep time EIOExt ..... 400
Kp .................... 15
Ki .................... 5
Kd .................... 0.05
Paddle damping ........ 10
Paddle offset ......... 999
Paddle adjustment ..... 18
Const speed increase .. 1
Const power invrease .. 0.5
Car data log period ... 1000
Serial 1 baud rate .... 115200
Serial 2 baud rate .... 9600
Telemetry send intervall1000
Telemetry cache records 100
===========================================================================================

     No SD card detected!
[  ] Init 'CmdHandler'...
[ok] CmdHandler initialized.
[  ] Init 'RTC'...
     DS1307_ADDRESS 68
     [INFO] rtc compile date/time: 08/07/2022 09:05:04
     [INFO] rtc time newer than compile time. Updating esp32time DateTime.
[ok] RTC initialized.
[  ] Init Indicator...done.
[ok] Indicator initialized.
[  ] Init 'DAC'...
     DAC initialized with I2C_ADDRESS_DS1803=28.
[ok] DAC initialized.
     Init 'ADC[0]' with address 0x48 ...
        Max voltage=6.144000 with multiplier=0.000188
          [ADS1x15] AIN0 --> 3: 0.000563mV
          [ADS1x15] AIN1 --> 2013: 0.377449mV
          [ADS1x15] AIN2 --> 2016: 0.378012mV
          [ADS1x15] AIN3 --> 2054: 0.385137mV
     ok ADC[0] at 0x48 inited.
     Init 'ADC[1]' with address 0x49 ...
        Max voltage=6.144000 with multiplier=0.000188
          [ADS1x15] AIN0 --> 2074: 0.388887mV
          [ADS1x15] AIN1 --> 2040: 0.382512mV
          [ADS1x15] AIN2 --> 2066: 0.387387mV
          [ADS1x15] AIN3 --> 2044: 0.383262mV
     ok ADC[1] at 0x49 inited.
     Init 'ADC[2]' with address 0x4a ...
        Max voltage=6.144000 with multiplier=0.000188
          [ADS1x15] AIN0 --> 1: 0.000188mV
          [ADS1x15] AIN1 --> 4: 0.000750mV
          [ADS1x15] AIN2 --> 2232: 0.418513mV
          [ADS1x15] AIN3 --> 2234: 0.418888mV
     ok ADC[2] at 0x4a inited.
[ok] ADC initialized.
[  ] Init GyroAcc...
     BMI088 is connected
     BMI088 is initialized
[ok] GyroAcc initialized.
[  ] Init CANBus...
     CANBus with rx=19m tx=1a inited.
[ok] CANBus initialized.

-----------------------------------------------------------------
Startup sequence(s) successful. System creating FreeRTOS tasks...
-----------------------------------------------------------------

[  ] Create IOExt as prio-4-task (sleep_polling=100ms, stack=6000) ... done.
[ok] IOExt started (100ms).
[  ] Create Indicator as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] Indicator started (330ms).
[  ] Create ADC as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] ADC started (330ms).
[  ] Create GyroAcc as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] GyroAcc started (330ms).
[  ] Create RTC as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] RTC started (330ms). RTC Time: 08/07/2022 09:25:33
[  ] Create CmdHandler as prio-20-task (sleep_polling=350ms, stack=6000) ... done.
[ok] CmdHandler started (350ms).
[  ] Create CANBus as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] CANBus started (330ms).
[  ] Init 'CarControl'...
done.
[ok] CarControl initialized.
[  ] Create CarControl as prio-10-task (sleep_polling=250ms, stack=6000) ... done.
[ok] CarControl started (250ms).
[  ] Init 'CarSpeed'...
[ok] CarSpeed initialized.
[  ] Create CarSpeed as prio-10-task (sleep_polling=250ms, stack=3000) ... done.
[ok] CarSpeed started (250ms).
[  ] Create EngineerDisplay as prio-10-task (sleep_polling=330ms, stack=4096) ... done.
[ok] EngineerDisplay started (1500ms).
[  ] Init 'Display'...
     Setup 'ILI9341' for 'DriverDisplay' with: SPI_CLK=5, SPI_MOSI=18, SPI_MISO=19, SPI_CS_TFT=32
I (17337) gpio: GPIO[32]| InputEn: 1| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
I (17337) gpio: GPIO[4]| InputEn: 1| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
I (17347) gpio: GPIO[21]| InputEn: 1| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
     ILI9341_RDMADCTL:   0xa
     ILI9341_RDPIXFMT:   0x1
     ILI9341_RDIMGFMT:   0xe0
     ILI9341_RDSELFDIAG: 0xf0
     ILI9341_RDMODE:     0xe5
[  ] Create DriverDisplay as prio-16-task (sleep_polling=330ms, stack=4096) ... done.
     Setup 'ILI9341' for 'DriverDisplay' with: SPI_CLK=5, SPI_MOSI=18, SPI_MISO=19, SPI_CS_TFT=32
I (18287) gpio: GPIO[32]| InputEn: 1| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
I (18287) gpio: GPIO[4]| InputEn: 1| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
I (18297) gpio: GPIO[21]| InputEn: 1| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0 
[ok] DriverDisplay started (330ms).DRIVER_SETUP
     ILI9341_RDMADCTL:   0xa
     ILI9341_RDPIXFMT:   0x1
     ILI9341_RDIMGFMT:   0xe0
     ILI9341_RDSELFDIAG: 0xf0
     ILI9341_RDMODE:     0xe5
[v] 'DriverDisplay' inited: screen E ILI9341 with 240 x 320
-----------------------------------------------------------------
Battery On
PV On
MC Off
EcoMowerMode Eco
Direction ForwardFreeRTOS tasks successfully created. System running.

Break pedal pressed -----------------------------------------------------------------
released
Light toggle
Horn Off
SD card removed.
EcoMowerMode Power
Direction Backward
EcoMowerMode Eco
Direction Forward
Request Paddle Adjust
paddle adjust: 17
paddle adjust: 16
paddle adjust: 15
paddle adjust: 14
paddle adjust: 13
paddle adjust: 12
paddle adjust: 11
paddle adjust: 10
paddle adjust:  9
paddle adjust:  8
paddle adjust:  7
paddle adjust:  6---01------------------------------------------
paddle adjust:  5
paddle adjust:  4
paddle adjust:  3
paddle adjust:  2
paddle adjust:  1
paddle adjust:  0

    ==> dec  1001-14570       ==> acc  1003-17464 => ok
DAC unlocked.
```
