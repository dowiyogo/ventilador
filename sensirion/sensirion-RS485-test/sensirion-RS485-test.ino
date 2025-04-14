/* Source: protosupplies.com
 * RS485 Master Software
 * Exercise the MAX485 Module.
 * This uses the SoftSerial.h library which comes with the Arduino IDE
 * Pins used for the soft serial port are fairly arbitrary and can be changed
 * as needed.  Just redefine them below.
 * 
 * Modified by: Jairo Gonzalez 
 * Date:        28.9.2021
 */
#include <SoftwareSerial.h>
const int SSERIAL_RX_PIN = 10;  //Soft Serial Receive pin
const int SSERIAL_TX_PIN = 11;  //Soft Serial Transmit pin
const int SSERIAL_CTRL_PIN= 3;   //RS485 Direction control
const int LED_PIN = 13;
const int RS485_TRANSMIT = HIGH;
const int RS485_RECEIVE = LOW;
//const int CMD = 17104945;        //Test CMD in decimal notation

// Create Soft Serial Port object and define pins to use
SoftwareSerial RS485Serial(SSERIAL_RX_PIN, SSERIAL_TX_PIN); // RX, TX

byte byteReceived[6];
byte buff[4] = {0x01,0x05,0x00,0x31};
int i;

//===============================================================================
//  Initialization
//===============================================================================
void setup()
{
  Serial.begin(115200);           // Start the built-in serial port
  Serial.println("Master Device");
  Serial.println("Type in upper window, press ENTER. This script will send test command to slave.");
  
  pinMode(LED_PIN, OUTPUT);     // Configure any output pins
  pinMode(SSERIAL_CTRL_PIN, OUTPUT);    
  
  digitalWrite(SSERIAL_CTRL_PIN, RS485_RECEIVE);  // Put RS485 in receive mode  
  
  RS485Serial.begin(115200);   // Start the RS485 soft serial port
}
//===============================================================================
//  Main
//===============================================================================
void loop() 
{
  if (Serial.available())         // A char(byte) has been entered in the Serial Monitor
  {
    Serial.read();  // Read the byte
    digitalWrite(SSERIAL_CTRL_PIN, RS485_TRANSMIT);  // Put RS485 in Transmit mode
    Serial.println("A test command has been sent.");
    RS485Serial.write(buff,4);                 // Send bytes to Remote Arduino
    digitalWrite(SSERIAL_CTRL_PIN, RS485_RECEIVE);   // Put RS485 back into Receive mode    
  }
  
  if (RS485Serial.available())            //Data from the Slave is available
   {
    for (i = 0; i<6; i++){
      digitalWrite(LED_PIN, HIGH);          // Show activity on LED
      byteReceived[i] = RS485Serial.read();    // Read received byte
      Serial.println(byteReceived[i], HEX);
      delay(10);
      digitalWrite(LED_PIN, LOW);           // Turn LED back off
     }
   }  
}
