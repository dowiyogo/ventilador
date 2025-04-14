/* CRC-8 Checksum functions.
 *
 * Source: Sensirion CRC Checksum application note.
 * https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/5_Mass_Flow_Meters/Application_Notes/Sensirion_GF_AN_SFM-04_CRC_Checksum_D1.pdf
 *
 *
 * @author Jairo Gonzalez 
 * @date 22.10.2021
 * 
 */

#include "Crc8.h"

//===============================================================================
//  CRC-8 Cheksum function
//===============================================================================
#define POLYNOMIAL 0x131 //P(x)=x^8+x^5+x^4+1 = 100110001
//============================================================
unsigned char checkCrc(unsigned char data[], unsigned char nbrOfBytes, unsigned char checksum)
//============================================================
//calculates checksum for n bytes of data
//and compares it with expected checksum
//input: data[] checksum is built based on this data
// nbrOfBytes checksum is built for n bytes of data
// checksum expected checksum
//return: error: CHECKSUM_ERROR = checksum does not match
// 0 = checksum matches
//
//============================================================
{
  unsigned char crc = 0;
  unsigned char byteCtr;
  //calculates 8-Bit checksum with given polynomial
  for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
  {
      crc ^= (data[byteCtr]);
      for (unsigned char bit = 8; bit > 0; --bit)
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

//===============================================================================
//  CRC-8 Cheksum calculation function
//===============================================================================
unsigned char calculateCrc(unsigned char data[], unsigned char nbrOfBytes){
  unsigned char crc = 0;
  unsigned char byteCtr;
  //calculates 8-Bit checksum with given polynomial
  for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
  {
      crc ^= (data[byteCtr]);
      for (unsigned char bit = 8; bit > 0; --bit)
      {
        if (crc & 0x80) 
          crc = (crc << 1) ^ POLYNOMIAL;
        else 
          crc = (crc << 1);
    }
  }
  return crc;
}
