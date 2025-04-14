/* Nicolay RS485 clip-on cap library.
 * 
 * @author Jairo Gonzalez 
 * @date 21.10.2021
 * 
 */

#ifndef Nicolay_h
#define Nicolay_h


#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Crc8.h"

class Nicolay{
  public:
    Nicolay(unsigned char slaveAddress, int rxPin, int txPin, int ctrlPin, int ledPin);
    unsigned char testCommand();
    unsigned long* getArticleNo();
    unsigned char start();
    long* getFlowMeasurement();
    typedef enum{
      CHECKSUM_ERROR = 0x04
    }etError;

  private:
    unsigned char _slaveAddress;
    int _rxPin;
    int _txPin;    
    int _ctrlPin;
    int _ledPin;
    const int RS485_TRANSMIT = HIGH;
    const int RS485_RECEIVE = LOW;
    SoftwareSerial RS485Serial = SoftwareSerial(10, 11);
};


#endif