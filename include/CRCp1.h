/*
* CRCinline.h
*
* Created: 5-6-2023 16:31:00
* Author: Jordan
*/

/** \file CRCinline.h
 * Generate the CRC with CRC-16/ARC using a input that does not need to be reversed.
 */

#ifndef __CRCP1_H__
#define __CRCP1_H__

#include <stddef.h>
#include <stdint.h>

/**
* struct holding the crc return value
*/
struct CrcReturn{
	char16_t crcValue = 0; /**< char16_t Holds calculated CRC value. */
	int crcErrorCode = 0; /**< int Holds the error code, 0=no error */
	uint32_t crcByteLenght = 0; /**< uint32_t Holds the amount of bytes parsed during CRC calculation. */
};


class CrcP1
{
	
	public:
		/**
		 * Generate 16 bit CRC.
		 *
		 * Input a char array and calculate the CRC until char end symbol or \0.
		 *
		 * \param start pointer to char array.
		 * \param end char to stop CRC calculation
		 * \return CrCReturn a struct holding errors codes and crc value
		 */
		CrcReturn static CrcGen(const char * start, char end){
			CrcReturn crc;
			if( (*start == end) || (start == NULL)){
				// 0-legth msg
				crc.crcErrorCode = -1;
				crc.crcValue = 0;
				return crc;
			}

			crc.crcByteLenght = 0;
			while(1){
				crc.crcValue ^= *start;
				for (uint16_t i = 0; i < 8; i++) {
					if(crc.crcValue & 1){
						crc.crcValue = (crc.crcValue >> 1) ^ 0xa001; //reversed poly for 0x8005
						//reverse poly allow to input CRC from MSB -> LSB
						}else{
						crc.crcValue = crc.crcValue >> 1;
					}
				}

				if(*start == end){ //executes once more on last
					break;
				}

				if(*start == '\0'){ //executes once more on last
					break;
				}

				++start;
				++crc.crcByteLenght;
			}

			//check if last symbol is equal to end otherwise there is a fail condition
			if(*start == end){
				crc.crcErrorCode = 0;
			}else{
				crc.crcErrorCode = -2;
				crc.crcValue = 0;
			}

			return crc;
		}

};


#endif //__CRCP1_H__