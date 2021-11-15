# chase_car_telemetry

solarenergyracers.ch chase car telemetry system

- Receives data from a serial port
- uploads data to an influx db
- offers UI with real-time data
- can send messages to solar car

Requires an influxdb

Made with python 3.8 including a virtual environment

serial data format is based on json syntax
example:
**Solar Car**
```json
{
    "t": <timestamp>,
    "d": [
            ["TMP1", 35.4],
            ["TMP2", 36.7],
            ["CUR1", 5.21],
            ["CAN", 0x539, 0x0102030401020304]
         ]           
}\n
```

## Get data between two timestamps
**Chase Car**
```json
{
	"req": 1,
	"m": "GET",
	"t": [1619958360, 1619958400]
}\n
```

**Solar Car**
```json
{
	"req": 1,
	"t": 1619958360,
	"d": {
		"TMP1": 35.4,
		"TMP2": 36.7,
		"CUR1": 5.21,
		"CAN": [0x539, 0x0102030401020304]
	}          
}\n
```
...

```json
{
	"req": 1,
	"t": 1619958399,
	"d": {
		"TMP1": 34.4,
		"TMP2": 37.7,
		"CUR1": 5.57,
		"CAN": [0x539, 0x0102030401020304]
	}
}\n
```


## Get data past a timestamp
**Chase Car**
```json
{
	"req": 2,
	"m": "GET",
	"t": [1619958360]
}\n
```


**Solar Car**
```json
{
	"req": 2,
	"t": 1619958360,
	"d": {
		"TMP1": 35.4,
		"TMP2": 36.7,
		"CUR1": 5.21,
		"CAN": [0x539, 0x0102030401020304]
	}  
}\n
```

## Set text on steeringwheel
**Chase Car**
```json
{
	"req": 3,
	"m": "SET",
	"fld": "txt",
	"d": "Lorem Ipsum"
}\n
```

**Solar Car**
```json
{
	"req": 3,
	"status": 0
}\n
```