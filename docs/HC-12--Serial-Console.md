# HC-12 Serial Radio Console

## Testcode

Download to both ESP32 mit connected HC-12.

```C++
#include <SoftwareSerial.h>

//#define RXD 14
//#define TXD 15
#define RXD2 16
#define TXD2 17

void setup() {

  Serial.begin(115200);
  delay(1000);
  
  Serial.println("Serial TX0 is on pin: " + String(TX));
  Serial.println("Serial RX0 is on pin: " + String(RX));

  Serial.println("Serial TX2 is on pin: " + String(TXD2));
  Serial.println("Serial RX2 is on pin:  "+ String(RXD2));

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(500);
}

bool payload;
void loop() {
  payload = false;
  while (Serial2.available()){
    Serial.print(char(Serial2.read()));
    payload = true;
  }

//  if(!payload){
//      Serial.println("Nothing to process");
//  } else {
//      Serial.println();
//  }
  String buffer = Serial.readString();
  Serial2.write(buffer.c_str());
  delay(100);
}

```
