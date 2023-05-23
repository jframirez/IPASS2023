/* 
* Helper.cpp
*
* Created: 11-5-2023 18:00:04
* Author: Jordan
*/


#include "../include/Helper.h"

namespace Helper{
	namespace Time{
		void delay1_5us(uint32_t us){
			uint32_t startTime = TC2->TC_CHANNEL[2].TC_CV;
			//while(TC2->TC_CHANNEL[2].TC_CV - startTime <= us){};
			while(TC2->TC_CHANNEL[2].TC_CV <= (us + startTime)){};
		}
		
		void delay1_5us(TIME_UNIT_1_5US us){
		uint32_t startTime = TC2->TC_CHANNEL[2].TC_CV;
		while(TC2->TC_CHANNEL[2].TC_CV <= ((uint32_t)us + startTime)){};
		}
		
		
		uint32_t operator*(uint32_t lhs, const TIME_UNIT_1_5US us){
			return (lhs * uint32_t(us));
		}
		
		uint32_t operator>=(uint32_t lhs, const TIME_UNIT_1_5US us){
			return (lhs >= uint32_t(us));
		}
		
		uint32_t operator<=(uint32_t lhs, const TIME_UNIT_1_5US us){
			return (lhs <= uint32_t(us));
		}
		
		uint32_t operator==(uint32_t lhs, const TIME_UNIT_1_5US us){
			return (lhs == uint32_t(us));
		}
	
	}
	namespace Debug{
		
		void DebugPrint(char pString){	
				UART->UART_THR = pString;
				while(!(UART->UART_SR & UART_SR_TXEMPTY)){}
		}
		
		
		void DebugPrint(const char * pString, ... ){
			while(*pString != '\0'){
				UART->UART_THR = *pString;
				while(!(UART->UART_SR & UART_SR_TXEMPTY)){}
				++pString;
			}
			
			/*
			
			char buffer[1024];
			va_list args;
			va_start(args, pString);
			vsnprintf(buffer, 1024, pString, args);
			va_end(args);
			
			int counter = 0;
			
			while((buffer[counter] != '\0') | (counter < 1024)){
				UART->UART_THR = buffer[counter];	
				while(!(UART->UART_SR & UART_SR_TXEMPTY)){}		
				++counter;
			}
			
			*/
		}
		
		void DebugPrintEX(const char * pString, ... ){
			char buffer[200]; //make sure to not make this go beyond heap size
			char * bufferP = buffer;
			va_list args;
			va_start(args, pString);
			vsnprintf(buffer, 20, pString, args);
			va_end(args);

			while( *bufferP != '\0'){
				UART->UART_THR = *bufferP;
				while(!(UART->UART_SR & UART_SR_TXEMPTY)){}
				++bufferP;
			}
		}
		
		
	}
}
	
