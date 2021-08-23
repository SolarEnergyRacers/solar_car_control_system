# PCF8574: 8 bit IO Extender with Interrupt

- [Buy: e.g. amazon](https://www.amazon.de/-/en/Mountaineer-PCF8574-IO-Expansion-Expander-Development/dp/B06W564ZSD/ref=sr_1_4?dchild=1&keywords=PCF8574-Modul&qid=1622910129&sr=8-4)
- [Tutorial](https://www.instructables.com/PCF8574-GPIO-Extender-With-Arduino-and-NodeMCU/)

## Pinout and Usage

![img](.PCF8574--IOExt0_IOExt1_IOExt2-infos/FLYUZMLJPSNYFFX.jpg)

Board size: 49x16, height with jumpers and pins : 11

Usage in steering wheel:

- 8 bits and interrupt from yellow pins (alls PCF8574 can use the same interrupt wire)
- VCC, GND, SDA, SCL from plug (chainable)

Usage in car hull:

- ###TODO

## Daisy-Chain

![img](.PCF8574--IOExt0_IOExt1_IOExt2-infos/FRLI1IMJPSNYF2K.jpg)

## Diagram

- Push buttons for steering wheel
- GND, VCC, I2C (SDA,SCL), Interrupt

![img](.PCF8574--IOExt0_IOExt1_IOExt2-infos/IO-Ext-CurcuitDiagram.drawio.png)

### Board Assembly

![image-20210823131457974](.PCF8574--IOExt0_IOExt1_IOExt2-infos/image-20210823131457974.png)

