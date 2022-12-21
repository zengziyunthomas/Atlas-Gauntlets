/*********
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/esp32-ble-server-client/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

#include "BLEDevice.h"
#include <Wire.h>
#include <string.h>
#include <math.h>

//BLE Server name (the other ESP32 name running the server sketch)
#define bleServerName "NRF_BOARD"

/* UUID's of the service, characteristic that we want to read*/
// BLE Service
static BLEUUID bmeServiceUUID("85e43f4d-b4a7-4c6f-ba86-2db3c40a2c83");
static BLEUUID My_CharCharacteristicUUID("85e47182-b4a7-4c6f-ba86-2db3c40a2c83");
//static BLEUUID My_CharCharacteristicUUID2("85e47382-b4a7-4c6f-ba86-2db3c40a2c83");

//Flags stating if should begin connecting and if the connection is up
static boolean doConnect = false;
static boolean connected = false;

//Address of the peripheral device. Address will be found during scanning...
static BLEAddress *pServerAddress;
 
//Characteristicd that we want to read
static BLERemoteCharacteristic* My_CharCharacteristic;
//static BLERemoteCharacteristic* My_CharCharacteristic2;

//Variables to store temperature and humidity
std::string value;
float pre_angle_pitch = 0;
float angle_pitch = 0;
float angle_yaw = 0;
int sign_mode = 1;
int sign_change = 0;
int change_cycles = 0;

const char * value_sent;

//Connect to the BLE Server that has the name, Service, and Characteristics
bool connectToServer(BLEAddress pAddress) {
   BLEClient* pClient = BLEDevice::createClient();
 
  // Connect to the remove BLE Server.
  pClient->connect(pAddress);
  Serial.println(" - Connected to server");
  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(bmeServiceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(bmeServiceUUID.toString().c_str());
    return (false);
  }
  // Obtain a reference to the characteristics in the service of the remote BLE server.
  My_CharCharacteristic = pRemoteService->getCharacteristic(My_CharCharacteristicUUID);
  //My_CharCharacteristic2 = pRemoteService->getCharacteristic(My_CharCharacteristicUUID2);
  if (My_CharCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID");
    return false;
  }
  Serial.println(" - Found our characteristics");
  if(My_CharCharacteristic->canRead()) {
      value = My_CharCharacteristic->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }
  return true;
}

//Callback function that gets called, when another device's advertisement has been received
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == bleServerName) { //Check if the name of the advertiser matches
      advertisedDevice.getScan()->stop(); //Scan can be stopped, we found what we are looking for
      pServerAddress = new BLEAddress(advertisedDevice.getAddress()); //Address of advertiser is the one we need
      doConnect = true; //Set indicator, stating that we are ready to connect
      Serial.println("Device found. Connecting!");
    }
  }
};
//function that prints the latest sensor readings in the OLED display
void setup() {
  //Start serial communication
  Serial.begin(2000000);
  Serial.println("Starting Arduino BLE Client application...");
  // 13,12 mode shift. Mode1: curser Mode2: writing Mode3: printing
  // 27,33,15,32,14,22 updown, leftright
  // 23,21,17,16 gesture
  // 19 sign_move_updown 18 sign_move_leftright
  // 13-40, 12-41, 27-42, 33-43, 15-44, 32-45, 14-46, 22-47, 23-48
  // 21-49, 17-50, 16-51 19-52 18-36
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
  digitalWrite(13,LOW);
  digitalWrite(12,HIGH);
  digitalWrite(27,LOW);
  digitalWrite(33,LOW);
  digitalWrite(15,LOW);
  digitalWrite(32,LOW);
  digitalWrite(14,LOW);
  digitalWrite(22,LOW);
  digitalWrite(23,LOW);
  digitalWrite(21,LOW);
  digitalWrite(17,LOW);
  digitalWrite(16,LOW);
  digitalWrite(19,LOW);
  digitalWrite(18,LOW);
  //Init BLE device
  BLEDevice::init("");
 
  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 30 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(15);
}

void loop() {
  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
  // connected we set the connected flag to be true.
  
  char  char_angle_pitch[6] = {'0','0','0','0','0'};
  char  char_angle_yaw[6] = {'0','0','0','0','0'};
  char  char_gesture_tuple[5] = {'0','0','0','0'};
  if (doConnect == true) {
    if (connectToServer(*pServerAddress)) {
      Serial.println("We are now connected to the BLE Server.");
      //Activate the Notify property of each Characteristic
      //My_CharCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
      connected = true;
    } else {
      Serial.println("We have failed to connect to the server; Restart your device to scan for nearby BLE server again.");
    }
    doConnect = false;
  }
  Serial.println("The characteristic value was: ");
  if(My_CharCharacteristic->canRead()) {    
      value = My_CharCharacteristic->readValue();
      Serial.print("          characteristic:");
      value_sent = value.c_str();
      for(int i = 0; i<16; i++){
        if(i<5&& value_sent[i] != ','){
          char_angle_pitch[i] = value_sent[i];
          }
        else if(5<i&&i<11&& value_sent[i] != ','){
          char_angle_yaw[i-6] = value_sent[i];
          }
        else if(11<i&& value_sent[i] != ','){
          char_gesture_tuple[i-12] = value_sent[i];
          }
        }
      angle_yaw = atof(char_angle_yaw);
      angle_pitch = atof(char_angle_pitch);     
      //mode shift
      if(change_cycles == 0){
        sign_change = 0;
        }
      if(char_gesture_tuple[0]== '1' && char_gesture_tuple[1]== '1' && char_gesture_tuple[2]== '1' && change_cycles == 0){
        if(change_cycles == 0){
          change_cycles = 5;
          sign_change = 1;
          }
        sign_mode += 1;
        if(sign_mode >3){
          sign_mode = 1;
          }   
         switch(sign_mode){
            case 1:
              digitalWrite(13,LOW);
              digitalWrite(12,HIGH);
            break;
            case 2:
              digitalWrite(13,HIGH);
              digitalWrite(12,LOW);
            break;
            case 3:
              digitalWrite(13,HIGH);
              digitalWrite(12,HIGH);
            break;
          } 
        }                    
      // up_down UP
      if(angle_pitch-pre_angle_pitch > 5 && angle_pitch-pre_angle_pitch<=15){
        digitalWrite(27,LOW);
        digitalWrite(33,LOW);
        digitalWrite(15,LOW); 
        digitalWrite(19,HIGH);    
        }
      else if(angle_pitch-pre_angle_pitch > 15 && angle_pitch-pre_angle_pitch<=30){
        digitalWrite(27,LOW);
        digitalWrite(33,LOW);
        digitalWrite(15,HIGH);
        digitalWrite(19,HIGH); 
        }
      else if(angle_pitch-pre_angle_pitch > 30 && angle_pitch-pre_angle_pitch<=45){
        digitalWrite(27,LOW);
        digitalWrite(33,HIGH);
        digitalWrite(15,LOW); 
        digitalWrite(19,HIGH);       
        }
      else if(angle_pitch-pre_angle_pitch > 45){
        digitalWrite(27,LOW);
        digitalWrite(33,HIGH);
        digitalWrite(15,HIGH);
        digitalWrite(19,HIGH);       
        }
      else if(angle_pitch-pre_angle_pitch < -5 && angle_pitch-pre_angle_pitch>=-15){
        digitalWrite(27,HIGH);
        digitalWrite(33,LOW);
        digitalWrite(15,LOW);
        digitalWrite(19,HIGH);         
        }
      else if(angle_pitch-pre_angle_pitch < -15 && angle_pitch-pre_angle_pitch>=-30){
        digitalWrite(27,HIGH);
        digitalWrite(33,LOW);
        digitalWrite(15,HIGH);
        digitalWrite(19,HIGH);         
        }
      else if(angle_pitch-pre_angle_pitch < -30 && angle_pitch-pre_angle_pitch>=-45){
        digitalWrite(27,HIGH);
        digitalWrite(33,HIGH);
        digitalWrite(15,LOW); 
        digitalWrite(19,HIGH);         
        }
      else if(angle_pitch-pre_angle_pitch < -45){
        digitalWrite(27,HIGH);
        digitalWrite(33,HIGH);
        digitalWrite(15,HIGH);
        digitalWrite(19,HIGH);    
        } 
      //non move   
      else if(angle_pitch-pre_angle_pitch<=5 && angle_pitch-pre_angle_pitch>=-5){
        digitalWrite(19,LOW);
        }
      // left_right
      if(angle_yaw> 10 && angle_yaw<=30){
        digitalWrite(32,LOW);
        digitalWrite(14,LOW);
        digitalWrite(22,LOW);
        digitalWrite(18,HIGH);      
        }
      else if(angle_yaw> 30 && angle_yaw<=50){
        digitalWrite(32,LOW);
        digitalWrite(14,LOW);
        digitalWrite(22,HIGH); 
        digitalWrite(18,HIGH); 
        }
      else if(angle_yaw> 50 && angle_yaw<=70){
        digitalWrite(32,LOW);
        digitalWrite(14,HIGH);
        digitalWrite(22,LOW);
        digitalWrite(18,HIGH);        
        }
      else if(angle_yaw> 70){
        digitalWrite(32,LOW);
        digitalWrite(14,HIGH);
        digitalWrite(22,HIGH);
        digitalWrite(18,HIGH);       
        }
      else if(angle_yaw < -10 && angle_yaw>=-30){
        digitalWrite(32,HIGH);
        digitalWrite(14,LOW);
        digitalWrite(22,LOW);
        digitalWrite(18,HIGH);         
        }
      else if(angle_yaw < -30 && angle_yaw>=-50){
        digitalWrite(32,HIGH);
        digitalWrite(14,LOW);
        digitalWrite(22,HIGH);
        digitalWrite(18,HIGH);         
        }
    else if(angle_yaw < -50 && angle_yaw>=-70){
        digitalWrite(32,HIGH);
        digitalWrite(14,HIGH);
        digitalWrite(22,LOW);
        digitalWrite(18,HIGH);         
      }
    else if(angle_yaw < -70){
        digitalWrite(32,HIGH);
        digitalWrite(14,HIGH);
        digitalWrite(22,HIGH); 
        digitalWrite(18,HIGH);  
      } 
    else if(angle_yaw<=5 && angle_yaw>=-5){
        digitalWrite(18,LOW);
        } 
    //gesture
    //fingure 1 
    if(char_gesture_tuple[0] == '0'){
       digitalWrite(23,LOW);
      }
    else if(char_gesture_tuple[0] == '1'){
       digitalWrite(23,HIGH);
      }
    //fingure 2 
    if(char_gesture_tuple[1] == '0'){
       digitalWrite(21,LOW);
      }
    else if(char_gesture_tuple[1] == '1'){
       digitalWrite(21,HIGH);
      }
    //fingure 3 
    if(char_gesture_tuple[2] == '0'){
       digitalWrite(17,LOW);
      }
    else if(char_gesture_tuple[2] == '1'){
       digitalWrite(17,HIGH);
      }
    //fingure 4 
    if(char_gesture_tuple[3] == '0'){
       digitalWrite(16,LOW);
      }
    else if(char_gesture_tuple[3] == '1'){
       digitalWrite(16,HIGH);
      }
            
      Serial.println(value_sent);
      //Serial.println(char_angle_pitch);
      Serial.println(angle_pitch);
      Serial.println(angle_yaw);
      Serial.println(char_gesture_tuple);
      Serial.println(sign_mode);
      Serial.println(change_cycles);
      pre_angle_pitch = angle_pitch;
    }
  //digitalWrite(18,HIGH);
  if(sign_change == 1 && change_cycles >0){
    change_cycles -= 1;
    }
  delay(50); // Delay 0.05 second between loops.
}
