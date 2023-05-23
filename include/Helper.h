/* 
* Helper.h
*
* Created: 11-5-2023 18:00:04
* Author: Jordan
*/


#ifndef __HELPER_H__
#define __HELPER_H__

#include "stdint.h"
#include "sam.h"
#include <cstdarg>
#include <cstdio>

namespace Helper{
	
	namespace Time{
		enum class TIME_UNIT_1_5US: uint32_t{
			SECOND = 656254,
			MILLISECOND = 667
		};
		
		uint32_t operator*(uint32_t lhs, const TIME_UNIT_1_5US us);
		uint32_t operator>=(uint32_t lhs, const TIME_UNIT_1_5US us);
		uint32_t operator<=(uint32_t lhs, const TIME_UNIT_1_5US us);
		uint32_t operator==(uint32_t lhs, const TIME_UNIT_1_5US us);
		
		void delay1_5us(uint32_t us);
		void delay1_5us(TIME_UNIT_1_5US us);
	}
	
	namespace Debug{
		void DebugPrint(const char * pString, ... );
		void DebugPrintEX(const char * pString, ... );
		void DebugPrint(char pString);
	}
}

#endif //__HELPER_H__
