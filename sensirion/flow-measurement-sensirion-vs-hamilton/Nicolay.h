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

    // Methods
    unsigned char testCommand();
    unsigned long* getArticleNo();
    unsigned char start();
    long* getFlowMeasurement();

    //Typdef for error handling
    typedef enum{
      CHECKSUM_ERROR = 0x04
    }etError;

  private:
    unsigned char _slaveAddress;
    int _rxPin;
    int _txPin;    
    int _ctrlPin;
    int _ledPin;

    //Variables and object related to RS485 communication. To be modifyen in future versions.
    const int RS485_TRANSMIT = HIGH;
    const int RS485_RECEIVE = LOW;
    SoftwareSerial RS485Serial = SoftwareSerial(5, 6);
};


#endif