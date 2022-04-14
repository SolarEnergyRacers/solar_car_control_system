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

## Boot Display Log - 2022.04 13

