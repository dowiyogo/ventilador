/* Nicolay RS485 clip-on cap library.
 * 
 * @author Jairo Gonzalez 
 * @date 21.10.2021
 * 
 */

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "Nicolay.h"
#include "Crc8.h"



//===============================================================================
//  CRC Cheksum variables
//==============================================================================
#define POLYNOMIAL 0x131

//==============================================================================
//  Nicolay Class methods
//==============================================================================


// Constructor
Nicolay::Nicolay(unsigned char slaveAddress, int rxPin, int txPin, int ctrlPin, int ledPin){
    pinMode(ctrlPin, OUTPUT); 
    digitalWrite(ctrlPin, RS485_RECEIVE); 
    RS485Serial.begin(115200);
    _slaveAddress = slaveAddress;
    _rxPin = rxPin;
    _txPin = txPin;    
    _ctrlPin = ctrlPin;
    _ledPin = ledPin;
}


// testCommand: send nothing and receive a fixed data
unsigned char Nicolay::testCommand(){
    unsigned char bToWrite[3] = {_slaveAddress,0x05,0x00}; //bits to be sent
    unsigned char byteReceived[6]; //Array to save incoming data
    unsigned char checksum = calculateCrc(bToWrite, 3); 
    unsigned char bToWrite_and_cheksum[4] = {bToWrite[0],bToWrite[1],bToWrite[2],checksum}; //concatenating bToWrite with his cheksum
    digitalWrite(_ctrlPin, RS485_TRANSMIT); // Put RS485 in Transmit mode
    RS485Serial.write(bToWrite_and_cheksum,4); // Send bytes to slave (4 bytes)
    digitalWrite(_ctrlPin, RS485_RECEIVE); // Put RS485 back into Receive mode

    while (true){
      if (RS485Serial.available())            //Data from the Slave is available
      {
        for (int i = 0; i<6; i++){
          digitalWrite(_ledPin, HIGH);          // Show activity on LED
          byteReceived[i] = RS485Serial.read();    // Read received byte
          delay(10);
          digitalWrite(_ledPin, LOW);           // Turn LED back off
        }
        unsigned char crc_calculation = checkCrc(byteReceived, 5, byteReceived[5]); //check if the 5 of the 6 numbers are ok (the 6th is the cheksum!)
        return crc_calculation;
      }
    }  
}

// getArticleNo: returns a 4 items array: [0]: last number, [1]: mid number, [2]: first number and [3]: cheksum state
unsigned long* Nicolay::getArticleNo(){
    static unsigned long serialNumber[4];
    unsigned char bToWrite[3] = {_slaveAddress,0x0A,0x00}; //bits to be sent
    unsigned char byteReceived[8]; //Array to save incoming data
    unsigned char checksum = calculateCrc(bToWrite, 3);
    unsigned char bToWrite_and_cheksum[4] = {bToWrite[0],bToWrite[1],bToWrite[2],checksum}; //concatenating bToWrite with his cheksum
    digitalWrite(_ctrlPin, RS485_TRANSMIT); // Put RS485 in Transmit mode
    RS485Serial.write(bToWrite_and_cheksum,4); // Send bytes to slave
    digitalWrite(_ctrlPin, RS485_RECEIVE); // Put RS485 back into Receive mode

    while (true){
      if (RS485Serial.available())            //Data from the Slave is available
      {
        for (int i = 0; i<8; i++){
          digitalWrite(_ledPin, HIGH);          // Show activity on LED
          byteReceived[i] = RS485Serial.read();    // Read received byte
          delay(10);
          digitalWrite(_ledPin, LOW);           // Turn LED back off
        }
        unsigned char crc_calculation = checkCrc(byteReceived, 7, byteReceived[7]);//check if the 7 of the 8 numbers are ok (the 8th is the cheksum!)
        serialNumber[3] = crc_calculation; //Saving checksum validation
        serialNumber[2] = byteReceived[6]>>4; // first number (just 4 bits)
        serialNumber[1] = byteReceived[6]&0x0F; //mid number (4 bits of 20)
        serialNumber[1] = (serialNumber[1]<<8)^byteReceived[5]; //mid number, 8 more bits
        serialNumber[1] = (serialNumber[1]<<8)^byteReceived[4]; //mid number, 8 more bits
        serialNumber[0] = byteReceived[3]; //last number
        return serialNumber;
      }
    }  
}

//start: Start the flow measurement
unsigned char Nicolay::start(){
    unsigned char bToWrite[3] = {_slaveAddress,0x0E,0x00}; //bits to be sent
    unsigned char byteReceived[4]; //Array to save incoming data
    unsigned char checksum = calculateCrc(bToWrite, 3);
    unsigned char bToWrite_and_cheksum[4] = {bToWrite[0],bToWrite[1],bToWrite[2],checksum}; //concatenating bToWrite with his cheksum
    digitalWrite(_ctrlPin, RS485_TRANSMIT); // Put RS485 in Transmit mode
    RS485Serial.write(bToWrite_and_cheksum,4); // Send bytes to slave
    digitalWrite(_ctrlPin, RS485_RECEIVE); // Put RS485 back into Receive mode

    while (true){
      if (RS485Serial.available())            //Data from the Slave is available
      {
        for (int i = 0; i<4; i++){
          digitalWrite(_ledPin, HIGH);          // Show activity on LED
          byteReceived[i] = RS485Serial.read();    // Read received byte
          delay(10);
          digitalWrite(_ledPin, LOW);           // Turn LED back off
        }
        unsigned char crc_calculation = checkCrc(byteReceived, 3, byteReceived[3]); //check if the 3 of the 4 numbers are ok (the 4th is the cheksum!)
        return crc_calculation;
      }
    }  
}

//getFlowMeasurement: returns a 2-item array. [0]: flow measurement, [1]: checksum state
long* Nicolay::getFlowMeasurement(){
    static long measurement[2];
    unsigned char bToWrite[3] = {_slaveAddress,0x10,0x00}; //bits to be sent
    unsigned char byteReceived[8]; //Array to save incoming data
    unsigned char checksum = calculateCrc(bToWrite, 3);
    unsigned char bToWrite_and_cheksum[4] = {bToWrite[0],bToWrite[1],bToWrite[2],checksum}; //concatenating bToWrite with his cheksum
    digitalWrite(_ctrlPin, RS485_TRANSMIT); // Put RS485 in Transmit mode
    RS485Serial.write(bToWrite_and_cheksum,4); // Send bytes to slave
    digitalWrite(_ctrlPin, RS485_RECEIVE); // Put RS485 back into Receive mode

    while (true){
      if (RS485Serial.available())            //Data from the Slave is available
      {
        for (int i = 0; i<8; i++){
          digitalWrite(_ledPin, HIGH);          // Show activity on LED
          byteReceived[i] = RS485Serial.read();    // Read received byte
          delay(10);
          digitalWrite(_ledPin, LOW);           // Turn LED back off
        }
        unsigned char crc_calculation = checkCrc(byteReceived, 7, byteReceived[7]); //check if the 7 of the 8 numbers are ok (the 8th is the cheksum!)
        measurement[1] = crc_calculation; //Saving checksum validation
        measurement[0] = byteReceived[6]; // MS byte
        measurement[0] = (measurement[0]<<8)^byteReceived[5]; // Mid byte
        measurement[0] = (measurement[0]<<8)^byteReceived[4]; // Mid byte
        measurement[0] = (measurement[0]<<8)^byteReceived[3]; // LS byte
        return measurement;
      }
    }  
}