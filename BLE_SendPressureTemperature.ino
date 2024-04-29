// Author: Luke Gockowski
// Updated: Apr 26, 2024 
// Function: Send sensor data across BLE to a phone app.
// Credits: This is a modification of the Arduino BLE "Server" example.

// Bluetooth Stuff -------------------------------------------------------
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b" // if needed, can generate a unique key here: https://www.uuidgenerator.net/
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

// BMP Sensor Stuff ---------------------------------------------------------
// For this project, I used a BMP 180 (pressure & temperature sensor).
#include "Wire.h"
#include "Adafruit_BMP085.h"
Adafruit_BMP085 bmp;

float temp; 
float pressure;

// Preallocation we use later for sending floats as strings via BLE
char tempString[10];      // size of character array I expect from distance reading XX.XX +1...
char pressureString[10];  // ...but it's safer just to have extra space for characters.

// Void Setup -----------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  while(!Serial); // wait until serial begins
  bmp.begin(); // start up BMP 180 sensor
  pinMode(LED_BUILTIN, OUTPUT); // turns on yellow LED, comment this out if you want to save power
  Serial.println();
  Serial.println("Serial began successfully..."); // DEBUG CODE - REMOVE IF NEEDED

  // Create the BLE Device, Server, Service, Characteristic, and Descriptor
  BLEDevice::init("XIAO ESP32"); // device name --> helps ID your device using the nRF mobile app
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising presence of device
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Setup complete."); // DEBUG CODE - REMOVE IF NEEDED
}


// Void Loop -----------------------------------------------------------------
void loop() { 
  while(!deviceConnected){
    Serial.println("Waiting to connect to a client..."); // DEBUG CODE - REMOVE IF NEEDED
    delay(5000);
  }

  // notify of changed value
  if (deviceConnected) {
    temp = bmp.readTemperature();
    pressure = bmp.readPressure();

    // I'm using the nRF app on iPhone to read my data.
    // If you crack open the BLECharacteristic.h library though, you'll see that the setValue() 
    // function (which we'll use in a moment) only accepts C-style strings. Thus, formatting:
    dtostrf(temp,-1,2,tempString);          // dtostrf(float_value, min_width, num_digits_after_decimal, where_to_store_string)
    dtostrf(pressure,-6,1,pressureString);  // note: negative "min_width" is left-justified
    std::string data_send = "T: ";
    data_send += tempString;
    data_send += " degC, P: ";
    data_send += pressureString;
    data_send += " Pa.";

    pCharacteristic->setValue(data_send);
    pCharacteristic->notify(); // this initiates the actual sending of data

    // For debugging:
    /*
    Serial.println("Success! Sent some data...");
    Serial.print("P: "); 
    Serial.print(pressure);
    Serial.print(" , ");
    Serial.print("T: ");
    Serial.println(temp);
    Serial.println(" ");
    delay(1000);
    */
  }

  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }

  // connecting:
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
}
