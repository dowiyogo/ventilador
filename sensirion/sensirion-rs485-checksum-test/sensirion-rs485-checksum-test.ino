/* CRC-8 test script
 * Source: Sensirion CRC Checksum application note.
 * https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/5_Mass_Flow_Meters/Application_Notes/Sensirion_GF_AN_SFM-04_CRC_Checksum_D1.pdf
 * 
 * Author: Jairo Gonzalez 
 * Date:        28.9.2021
 */
#define POLYNOMIAL 0x131

typedef enum{
  CHECKSUM_ERROR = 0x04
}etError;

typedef unsigned char u8t;

int checksum = 0x31;
byte data[3] = {0x01,0x05,0x00};
int crc_calculation;

//===============================================================================
//  Initialization
//===============================================================================
void setup()
{
  Serial.begin(115200);           // Start the built-in serial port
  Serial.println("Test the CRC-8 algorithm.");
}
//===============================================================================
//  Main
//===============================================================================
void loop() 
{
  crc_calculation = SMF3000_CheckCrc (data, 3, checksum);
  Serial.println(crc_calculation,HEX);
}

//CRC
#define POLYNOMIAL 0x131 //P(x)=x^8+x^5+x^4+1 = 100110001
//============================================================
u8t SMF3000_CheckCrc (u8t data[], u8t nbrOfBytes, u8t checksum)
//============================================================
//calculates checksum for n bytes of data
//and compares it with expected checksum
//input: data[] checksum is built based on this data
// nbrOfBytes checksum is built for n bytes of data
// checksum expected checksum
//return: error: CHECKSUM_ERROR = checksum does not match
// 0 = checksum matches
//============================================================
{
  u8t crc = 0;
  u8t byteCtr;
  //calculates 8-Bit checksum with given polynomial
  for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
  {
      crc ^= (data[byteCtr]);
      for (u8t bit = 8; bit > 0; --bit)
      {
        if (crc & 0x80) 
          crc = (crc << 1) ^ POLYNOMIAL;
        else 
          crc = (crc << 1);
    }
  }
  if (crc != checksum)
  {
    return CHECKSUM_ERROR;
  }
  else
  {
    return 0;
  }
}

