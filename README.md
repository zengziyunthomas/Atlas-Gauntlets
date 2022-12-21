# Sensor Glove Controlled LED Matrix

## Abstract
In this project we built a sensor glove which could read the gesture and the movement of the hand. Then to dig more about how it could be applied in daily life, we connected it to a LED matrix board to enable us to control the behavior of the LED matrix. Up to now we designed and implemented three modes for it. They are “Cursor”, “Writing”, “Printing” respectively with different functionality. The “Cursor” mode means we can control the movement of a cursor on the LED board with respect to the movement of the hand. The “Writing” mode means that we can write locus on the board, and based on the gesture, we can also erase the writings out. The “Print” mode means that the board can print different slogans based on different gestures of the hand.


## Required Content for Scoring
Report
- https://docs.google.com/document/d/1lrwQqitYt119p2mN4PGDiaZlA-H4oOvN/edit

Presentation
- https://youtu.be/OTnhA-OjA6o

Poster
- https://drive.google.com/file/d/14UvpmjGqeN1zCTDOllGIZBgKGC2CLaDc/view


## Sensor Glove
The basic idea is to build the connection between the real world and the digital. Several sensors are used to measure the physical quantities and convert signals to digital numbers. The basic idea is to use the LF we learned in the lab to coordinate each section and integrate them into a single characteristic and then transit it to ESP32, which will process our data and give command to the LED matrix.

## LED Module

The LED module is built with Adafruit 64x32 RGB LED matrix, Adafruit esp32 feather and Arduino Mega 2560. 

The LED module is designed to receive the BLE advertisement and then encode different behaviors into binary codes and then control the LED matrix to respond timely and correctly to the sensor glove. The Adafruit LED matrix is highly integrated and has a well defined behavior implemented with the arduino library.

The function of the esp32 feather is to communicate with the nrf board in sensor glove through BLE and encode the behavior into binary codes which could be sent to arduino mega.
## Reference
[1]	The Embedded Lab for nRF52DK and BLE (https://drive.google.com/file/d/1Gn-7xy1B7y2GeDa0YqWi9zEPA0GmeFLg/view)

[2]	Introduction to Embedded Systems, 2nd Edition, Edward Ashford Lee and Sanjit Arunkumar Seshia

[3]	32x16 and 32x32 RGB LED Matrix (https://cdn-shop.adafruit.com/product-files/2276/MIT35P6RGBE-AA_6432_11.pdf)

[4]	Arduino Mega (https://docs.arduino.cc/static/88aaeaaded25477450fb5f36cb7be27d/A000067-datasheet.pdf)

[5] Basic example code in lab VM and the lab handout	

[6] BLE code template (https://github.com/lab11/nrf52xbase/blob/master/lib/simple_ble)

