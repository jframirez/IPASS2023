/*
* CRCinline.h
*
* Created: 5-6-2023 16:31:00
* Author: Jordan
*/

#ifndef __CRCP1_H__
#define __CRCP1_H__

#include <stddef.h>
#include <stdint.h>

struct CRCreturn{
	char16_t crcValue = 0;
	int crcErrorCode = 0;
	uint32_t crcByteLenght = 0;
};


class CRCp1
{
	private:
	static const char16_t poly = 0x1021;

	public:

	CRCreturn static crcGen(const char * start, char end){
		CRCreturn crc;
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
					crc.crcValue = (crc.crcValue >> 1) ^ 0xa001; //reversed poly 0x8005
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

		//check if last symbol is equel to end otherwise there is a fail condition
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