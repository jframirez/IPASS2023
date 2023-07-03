/* 
* Helper.cpp
*
* Created: 11-5-2023 18:00:04
* Author: Jordan
*/


#include "Helper.h"

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
		
		void init(){
			
			PMC->PMC_PCER0 |= (PMC_PCER0_PID29); //Timer Counter Channel 2
			
			/* TC2 -> Channel 2 : used for Helper::Time functions */
			TC2->TC_CHANNEL[2].TC_CMR =	TC_CMR_TCCLKS_TIMER_CLOCK4 | //~1.5uSec per tick
			TC_CMR_WAVE |
			TC_CMR_WAVSEL_UP ;
			
			TC2->TC_CHANNEL[2].TC_CCR |= TC_CCR_CLKEN; //Enable TC2
			TC2->TC_CHANNEL[2].TC_CCR |= TC_CCR_SWTRG;
			//TC2->TC_BCR |= TC_BCR_SYNC;
		}
	
	}
	namespace Debug{
		
		void DebugPrint(char pString){	
				UART->UART_THR = pString;
				while(!(UART->UART_SR & UART_SR_TXEMPTY)){}
		}
		
		
		void DebugPrint(const char * pString){
			while(*pString != '\0'){
				UART->UART_THR = *pString;
				while(!(UART->UART_SR & UART_SR_TXEMPTY)){}
				++pString;
			}
		}
		
		void DebugPrintVA(const char * pString, ... ){
			char buffer[1024]; //make sure to not make this go beyond heap size
			char * bufferP = buffer;
			va_list args;
			va_start(args, pString);
			vsnprintf(buffer, 200, pString, args);
			va_end(args);

			//Hard set end of buffer with \0 to always end print line;
			buffer[199] = '\0';

			while( *bufferP != '\0'){
				UART->UART_THR = *bufferP;
				while(!(UART->UART_SR & UART_SR_TXEMPTY)){}
				++bufferP;
			}
		}
		
		void DebugPrint(const std::string & x){
			const char * bufferP = x.c_str();

			while( *bufferP != '\0'){
				UART->UART_THR = *bufferP;
				while(!(UART->UART_SR & UART_SR_TXEMPTY)){}
				++bufferP;
			}
		}
		
		void init(){
			/* UART Debug port setup */
			PMC->PMC_PCER0 |= (	PMC_PCER0_PID8 ); //UART
			
			PinDriver uartRX(PIOA, 8);
			PinDriver uartTX(PIOA, 9);
			
			uartRX.controllerPioDisable();
			uartTX.controllerPioDisable();
			
			uartRX.peripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_A);
			uartTX.peripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_A);
			
			UART->UART_BRGR = UART_BRGR_CD(46); //Baud rate ~115200
			UART->UART_CR |= (UART_CR_TXEN);
			
			//wait for UART ready
			while(!(UART->UART_SR & UART_SR_TXRDY)){}
		}
		
	}
}
	
