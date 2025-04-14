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
float pressureHamilton;

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
  Serial.println("Type in upper window, press ENTER to start the acquisition.");
  float pressureHamilton;
}


//===============================================================================
//  Main loop
//===============================================================================
void loop() 
{

  if (Serial.available())         // A char(byte) has been entered in the Serial Monitor
  {
    Serial.read();  // Read the byte
    unsigned long startTime = millis(); //Time since the char was received

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

    //Printing data read at every sensor, during 25min
    unsigned long timeElapsed = millis()-startTime;
    while ((timeElapsed) < 1500000){ //25min in miliseconds
      long* startOutput; //Pointer, to receive an array of 2 numbers
      startOutput = sfm3300.getFlowMeasurement(); //Sensirion SFM3300 flow measurement
      pressureHamilton = medir_presion_SPI(press_max, press_min); //Honeywell pressure sensore (hamilton)
      if ((*startOutput==2147483647)){ //0x7FFFFFFF, means data not readable
        Serial.println("Data not readable, please perform a hardware reset.");
      }
      else { //Prints data on serial terminal
        Serial.print(*startOutput);
        Serial.print(",");
        Serial.println(pressureHamilton);
      }
      timeElapsed = millis()-startTime;
    }
    Serial.print("Done. Time elapsed (ms):");
    Serial.println(timeElapsed);
  }
}


//==============================================================================
//  Pressure sensor read function (Hamilton transducer)
//  Author: Eduardo Valdivia
//  Date: 2020
//==============================================================================
float medir_presion_SPI(int p_max, int p_min) {
  
  byte data[2] = {NULL_BYTE, NULL_BYTE};
  float pressure = 0;
  unsigned int pressure_raw = 0;
  byte pressure_mask = 0x3F;
  byte stat = 0x00;

  //const int out_min = 1638;
  //const int out_max = 14745;

  SPI.beginTransaction (SPISettings (800000, MSBFIRST, SPI_MODE0));

  digitalWrite(10, LOW); 
  SPI.transfer(data, sizeof(data));
  digitalWrite(10, HIGH);
 
  
  stat = data[0] >> 6;
  pressure_raw = ((data[0] & pressure_mask) << 8) + data[1];
  //pressure = 0.1 * ((((float)pressure_raw - out_min) * (p_max - p_min) * 0.0000762951) + p_min);

  return pressure_raw;
}


