# About

This is the code for setting up the BLE client on Adafruit esp32 feather as well as processing the data sent from sensors and generating binary output.

# Usage

Using Arduino IDE with `#include "BLEDevice.h"`. Need to download and install libraries for esp32 board first. Then 

```c++
static BLEUUID bmeServiceUUID("85e43f4d-b4a7-4c6f-ba86-2db3c40a2c83");
static BLEUUID My_CharCharacteristicUUID("85e47182-b4a7-4c6f-ba86-2db3c40a2c83");
```

setting the UUID to be the UUID you used on the server. Then set up the characteristic on the server and do the connection:

```c++
bool connectToServer(BLEAddress pAddress)
```

Then you can receive the char data.

# Char data

The char sent from the server is saved to `value` and could be transfer to `char`:

```c++
value = My_CharCharacteristic->readValue();
value_sent = value.c_str();
```

You can do operation on it.

# Output port

In this project we use the following digital GPIO to be outputs:

```c++
pinMode(13,OUTPUT);
pinMode(12,OUTPUT);
pinMode(27,OUTPUT);
pinMode(33,OUTPUT);
pinMode(15,OUTPUT);
pinMode(32,OUTPUT);
pinMode(14,OUTPUT);
pinMode(22,OUTPUT);
pinMode(23,OUTPUT);
pinMode(21,OUTPUT);
pinMode(17,OUTPUT);
pinMode(16,OUTPUT);
pinMode(19,OUTPUT);
pinMode(18,OUTPUT);
```

You can change them to other digital GPIO on esp32 feather as you want.