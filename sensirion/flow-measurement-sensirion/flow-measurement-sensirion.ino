/* This script reads a pressure sensor connected to a Hamilton flow transducer
 * and reads also the output of a sensirion SFM3300 flow meter, for
 * characterization of the Hamilton transducer.
 * 
 * @author Jairo Gonzalez 
 * @date 26.10.2021
 */

//===============================================================================
//  Includes
//==============================================================================
#include <SoftwareSerial.h>
#include "Nicolay.h"
#include "Crc8.h"
#include <SPI.h>

//==============================================================================
//  Pressure sensor defines (Hamilton transducer)
//==============================================================================
#define NULL_BYTE 0x00
#define press_max 2
#define press_min -2
const int slaveSelect = 10;

//==============================================================================
//  RS485 variables
//==============================================================================
const int SSERIAL_RX_PIN = 5;  //Soft Serial Receive pin
const int SSERIAL_TX_PIN = 6;  //Soft Serial Transmit pin
const int SSERIAL_CTRL_PIN= 3;   //RS485 Direction control
const int LED_PIN = 4;
const int SLAVE_ADDR = 1;

// Create Nicolay object and define pins to use
Nicolay sfm3300(SLAVE_ADDR, SSERIAL_RX_PIN, SSERIAL_TX_PIN, SSERIAL_CTRL_PIN, LED_PIN);


//===============================================================================
//  Script Initialization
//===============================================================================
void setup()
{
  SPI.begin();
  pinMode(slaveSelect,OUTPUT);
  Serial.begin(115200);           // Start the built-in serial port
  Serial.println("Master Device");
  // Printing SFM3300 Article number, no reason why.
    unsigned long* articleNo = sfm3300.getArticleNo(); //Pointer to receive array of 4 items
    while( *(articleNo+3) == 4){ //4 means checksum error
      Serial.println("Cheksum failed. Not starting yet.");
      articleNo = sfm3300.getArticleNo();
    }
    Serial.print("\n-------------------------------\nSensirion SFM-3300 | Article Number: ");
    Serial.print(*(articleNo+2));
    Serial.print("-");
    Serial.print(*(articleNo+1));
    Serial.print("-");
    Serial.println(*(articleNo));
    Serial.print("-------------------------------\n");
  // Starting SFM3300 acquisition
    while (sfm3300.start()==4){
      Serial.println("Cheksum failed. Not starting yet.");
    }
    Serial.println("Starting data acquisition.");

}


//===============================================================================
//  Main loop
//===============================================================================
void loop() 
{
  unsigned long integral =0.;
  
  startOutput = sfm3300.getFlowMeasurement();
  if ( *startOutput>1 && *startOutput!=2147483647)         
  {
    unsigned long startTime = millis(); //Time since the char was received

    //Printing data read at every sensor, during 1min
    unsigned long timeElapsed = millis()-startTime;
    while ((timeElapsed) < 60000){ //25min in miliseconds
      unsigned long inicio_t = millis();
      long* startOutput; //Pointer, to receive an array of 2 numbers
      startOutput = sfm3300.getFlowMeasurement(); //Sensirion SFM3300 flow measurement
      
      if ((*startOutput==2147483647)){ //0x7FFFFFFF, means data not readable
        Serial.println("Data not readable, please perform a hardware reset.");
      }
      else { //Prints data on serial terminal
        unsigned long fin_t = millis();
        integral = integral + (*startOutput)*(fin_t-inicio_t)*1000;
        Serial.print(*startOutput);
        Serial.print(",");
        Serial.println(integral);
      }
      timeElapsed = millis()-startTime;
    }
    Serial.print("Done. Volume: ");
    Serial.println(integral);
  }
}
