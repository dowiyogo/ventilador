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

#ifndef Crc8_h
#define Crc8_h

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum{
  CHECKSUM_ERROR = 0x04
}etError;


extern unsigned char checkCrc(unsigned char data[], unsigned char nbrOfBytes, unsigned char checksum);
extern unsigned char calculateCrc(unsigned char data[], unsigned char nbrOfBytes);

#ifdef __cplusplus
}
#endif

#endif