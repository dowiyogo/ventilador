#include "Arduino.h"
#include <SoftwareSerial.h>
#include "Nicolay.h"
#include "Crc8.h"



//===============================================================================
//  CRC Cheksum variables
//==============================================================================
#define POLYNOMIAL 0x131

// typedef enum{
//   CHECKSUM_ERROR = 0x04
// }etError;


// unsigned char SMF3000_CalculateCrc (unsigned char data[], unsigned char nbrOfBytes);
// unsigned char SMF3000_CheckCrc (unsigned char data[], unsigned char nbrOfBytes, unsigned char checksum);

//==============================================================================
//  Nicolay Class methods
//==============================================================================

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

unsigned char Nicolay::testCommand(){
    unsigned char bToWrite[3] = {_slaveAddress,0x05,0x00};
    unsigned char byteReceived[6];
    unsigned char checksum = calculateCrc(bToWrite, 3);
    unsigned char bToWrite_and_cheksum[4] = {bToWrite[0],bToWrite[1],bToWrite[2],checksum};
    digitalWrite(_ctrlPin, RS485_TRANSMIT); // Put RS485 in Transmit mode
    RS485Serial.write(bToWrite_and_cheksum,4); // Send bytes to slave
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
        unsigned char crc_calculation = checkCrc(byteReceived, 5, byteReceived[5]);
        return crc_calculation;
      }
    }  
}

unsigned long* Nicolay::getArticleNo(){
    static unsigned long serialNumber[4];
    unsigned char bToWrite[3] = {_slaveAddress,0x0A,0x00};
    unsigned char byteReceived[8];
    unsigned char checksum = calculateCrc(bToWrite, 3);
    unsigned char bToWrite_and_cheksum[4] = {bToWrite[0],bToWrite[1],bToWrite[2],checksum};
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
        unsigned char crc_calculation = checkCrc(byteReceived, 7, byteReceived[7]);
        serialNumber[3] = crc_calculation;
        serialNumber[2] = byteReceived[6]>>4;
        serialNumber[1] = byteReceived[6]&0x0F;
        serialNumber[1] = (serialNumber[1]<<8)^byteReceived[5];
        serialNumber[1] = (serialNumber[1]<<8)^byteReceived[4];
        serialNumber[0] = byteReceived[3];
        return serialNumber;
      }
    }  
}

unsigned char Nicolay::start(){
    unsigned char bToWrite[3] = {_slaveAddress,0x0E,0x00};
    unsigned char byteReceived[4];
    unsigned char checksum = calculateCrc(bToWrite, 3);
    unsigned char bToWrite_and_cheksum[4] = {bToWrite[0],bToWrite[1],bToWrite[2],checksum};
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
        unsigned char crc_calculation = checkCrc(byteReceived, 3, byteReceived[3]);
        return crc_calculation;
      }
    }  
}

long* Nicolay::getFlowMeasurement(){
    static long measurement[2];
    unsigned char bToWrite[3] = {_slaveAddress,0x10,0x00};
    unsigned char byteReceived[8];
    unsigned char checksum = calculateCrc(bToWrite, 3);
    unsigned char bToWrite_and_cheksum[4] = {bToWrite[0],bToWrite[1],bToWrite[2],checksum};
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
        unsigned char crc_calculation = checkCrc(byteReceived, 7, byteReceived[7]);
        measurement[1] = crc_calculation;
        measurement[0] = byteReceived[6];
        measurement[0] = (measurement[0]<<8)^byteReceived[5];
        measurement[0] = (measurement[0]<<8)^byteReceived[4];
        measurement[0] = (measurement[0]<<8)^byteReceived[3];
        return measurement;
      }
    }  
}