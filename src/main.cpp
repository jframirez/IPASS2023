/*
 * P1Meter.cpp
 *
 * Created: 29-4-2023 21:07:28
 * Author : Jordan
 */ 




#include "sam.h"
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <cstdarg>
#include <string>

#include "component/pio.h"

#include "../include/SPIDriver.h"
#include "../include/PINDriver.h"
#include "MenuManager.h"
#include "../include/ILI9341Driver.h"

#include "../include/LED.h"
#include "../include/Helper.h"

#include "../Font/include/font-ubuntumono-10.h"
#include "../Font/include/font-ubuntumono-16.h"
#include "../Font/include/font-ubuntumono-22.h"
#include "../Font/include/font-ubuntumono-28.h"
#include "../Font/include/font-ubuntumono-34.h"
#include "../Font/include/font-ubuntumono-40.h"

#include "../Img/include/TestJPG.h"

#include "MenuManager.h"
#include "menuPageSplash.h"

//#include "P1Controller.h"

#include "P1Decoder.h"

void TC0_Handler(){
	
}

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	
	//Disable watchdog
	WDT->WDT_MR |= WDT_MR_WDDIS;
	
	//Disable write protect
	//PIOB->PIO_WPMR &= ((0x50494F << 8) & 0xFF);
	//PIOB->PIO_WPMR &= ~(PIO_WPMR_WPEN);
	//PIOA->PIO_WPMR = 0x50494F01;

	LED debugLED1(PIOB, 27);
	LED debugLED2(PIOA, 21, true);
	LED debugLED3(PIOC, 30, true);
	
	debugLED1.OFF();
	debugLED2.OFF();
	debugLED3.ON();
	
	//SPI 
	//PA25 SPI0_MISO A18
	//PA26 SPI0_MOSI A19
	//PA27 SPI0_SPCK A20
	//SPI0 Peripheral Identifier 24
	
	//PMC Peripheral Clock Enable Register
	//PMC->PMC_PCER0 |= (1 << 24); //SPI
	
	
	PMC->PMC_PCER0 |= (	PMC_PCER0_PID8  | //UART
						PMC_PCER0_PID20 | //USART3
						PMC_PCER0_PID24 | //SPI0
						PMC_PCER0_PID25 | //SPI1
						PMC_PCER0_PID27 | //Timer Counter Channel 0
						PMC_PCER0_PID28 | //Timer Counter Channel 1
						PMC_PCER0_PID29 | //Timer Counter Channel 2
						PMC_PCER0_PID30 | //Timer Counter Channel 3
						PMC_PCER0_PID31 );//Timer Counter Channel 4

	PMC->PMC_PCER1 |= (	PMC_PCER1_PID32 | //Timer Counter Channel 5
						PMC_PCER1_PID33 | //Timer Counter Channel 6
						PMC_PCER1_PID34 | //Timer Counter Channel 7
						PMC_PCER1_PID35 );//Timer Counter Channel 8					
			
	
	//TC2 -> Channel 2 : used for Helper::Time functions
	TC2->TC_CHANNEL[2].TC_CMR =		TC_CMR_TCCLKS_TIMER_CLOCK4 | //~1.5uSec per tick
									TC_CMR_WAVE |
									TC_CMR_WAVSEL_UP ;
	
	TC2->TC_CHANNEL[2].TC_CCR |= TC_CCR_CLKEN; //Enable TC2
	TC2->TC_CHANNEL[2].TC_CCR |= TC_CCR_SWTRG;
	
	//TC2->TC_BCR |= TC_BCR_SYNC;

	PINDriver SPI0_MISO(PIOA, 25);
	PINDriver SPI0_MOSI(PIOA, 26);
	PINDriver SPI0_SPCK(PIOA, 27);
	PINDriver DisplaySS(PIOA, 28);
	PINDriver DisplayDC(PIOC, 21);
	PINDriver DisplayRESET(PIOC, 22);
	
	SPI0_MISO.ControllerPIODisable();
	SPI0_MOSI.ControllerPIODisable();
	SPI0_SPCK.ControllerPIODisable();
	
	DisplaySS.ControllerPIOEnable();
	DisplaySS.SetOutput(PIO_OUTPUT::LOW); //Low is enable on ILI9341
	
	DisplayDC.ControllerPIOEnable();
	DisplayDC.SetOutput(PIO_OUTPUT::LOW); //1: data, 0: command
	
	DisplayRESET.ControllerPIOEnable();
	DisplayRESET.SetOutput(PIO_OUTPUT::LOW); // Active low, must be used to init chip
	DisplayRESET.SetOutput(PIO_OUTPUT::HIGH);
	
	SPI0_MISO.PeripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_A);
	SPI0_MOSI.PeripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_A);
	SPI0_SPCK.PeripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_A);

	bool LEDState = false;
	//bool one_shot = true;
	unsigned int startTime = TC2->TC_CHANNEL[2].TC_CV;

	//UART Debug
	
	//UART 0
	//A.9 = UTXD TX0
	//A.8 = URXD RX0
	
	PINDriver uartRX(PIOA, 8);
	PINDriver uartTX(PIOA, 9);
	
	uartRX.ControllerPIODisable();
	uartTX.ControllerPIODisable();
	
	uartRX.PeripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_A);
	uartTX.PeripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_A);
	
	UART->UART_BRGR = UART_BRGR_CD(46); //Baud rate ~115200
	UART->UART_CR |= (UART_CR_TXEN);
	
	//wait for UART ready
	while(!(UART->UART_SR & UART_SR_TXRDY)){}
		
	//USART3 -> Test P1 telegram from arduino nano
	//D.4 = TXD3
	//D.5 = RXD3
	
	PINDriver usart3TX(PIOD, 4);
	PINDriver usart3RX(PIOD, 5);
	
	usart3TX.ControllerPIODisable();
	usart3RX.ControllerPIODisable();
	
	usart3TX.PeripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_B);
	usart3RX.PeripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_B);
	
	USART3->US_BRGR = UART_BRGR_CD(46);
	USART3->US_CR |= (US_CR_RXEN);
	USART3->US_MR |= US_MR_USART_MODE_NORMAL | US_MR_CHRL_8_BIT | US_MR_PAR_NO | US_MR_NBSTOP_1_BIT | US_MR_CHMODE_NORMAL;
	USART3->US_RTOR = US_RTOR_TO(500);
	
	Helper::Debug::DebugPrint("IN RECEIVE LOOP v0.2: ");
	
// 	char cBuffer[4096];
// 	int bufCount = 0;
// 	int highMark = 0;
// 	USART3->US_CR |= US_CR_STTTO;
// 	while(1){
// 		if((USART3->US_CSR & US_CSR_RXRDY)){
// 			USART3->US_CR |= US_CR_RETTO;
// 			cBuffer[bufCount] = ((char)(USART3->US_RHR & 0xFF));
// 			++bufCount;
// 		}
// 	
// 		if((USART3->US_CSR & US_CSR_TIMEOUT)){
// 			//SEND TIMEOUT;
// 			USART3->US_CR |= US_CR_STTTO;
// 			cBuffer[bufCount] = '\0'; 
// 			if(bufCount > highMark){
// 				highMark = bufCount;
// 			}
// 			bufCount = 0;
// 			Helper::Debug::DebugPrintEX("\r\nHigh Mark: %i\r\n", highMark);
// 			Helper::Debug::DebugPrint(cBuffer);	
// 		}
// 		//get lower 8 bit, might be implicit in cast to char
// 	}


	
	//while(1){
		
		 
	//}
	
	//P1Controller::decodeP1(testP1Telegram);

	//SPI0
	SPIDriver LCDSpi(SPI0_MISO, SPI0_MOSI, SPI0_SPCK, false, false, SPI0);
	
	//
	ILI9341Driver LCD(DisplaySS, DisplayDC, DisplayRESET, LCDSpi);
	
 	MenuManager p1Screen(LCD);
 	
 	p1Screen.SetMenu(&menuPageSplash, ILI_COLORS::BLACK);
 	p1Screen.WriteTextLabel(0, font_ubuntumono_22, "TEST PRINT");
// 	int iTest = -10;
// 	//p1Screen.WriteTextLabel(1, font_ubuntumono_22, "VAL: %i", iTest);
// 	
 	
// 
// 	int P1DecodeValue = P1Decoder::decodeP1(testP1Telegram, p1msg);
// 
// 	std::list<OBISChannel*>::iterator it;
// 	if(P1DecodeValue == 0){
// 		for(auto &ptr: p1msg.OBISChannelList){
// 			Helper::Debug::DebugPrintEX("CHANNEL NUMBER: %i\r\n", ptr->getChannelNumber());
// 			//std::list<OBISObject*> tempList = ptr->getOBISObjectList();
// 			for(auto &Optr: ptr->getOBISObjectList()){
// 				Helper::Debug::DebugPrintEXSTRING(Optr->print());
// 				Helper::Debug::DebugPrintEX("\r\n");
// 			}
// 		}	
// 	}

	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	
	p1Screen.SetMenu(&menuPageMain, ILI_COLORS::BLACK);
	
	std::string receiveBuffer;
	
	USART3->US_CR |= US_CR_STTTO;
	
    while (1){
				
		if(TC2->TC_CHANNEL[2].TC_CV - startTime >= (1 * Helper::Time::TIME_UNIT_1_5US::SECOND)){
			if(LEDState){
				debugLED1.OFF();
				LEDState = false;
				
			}else{
				debugLED1.ON();
				LEDState = true;
				
			}
			debugLED2.ON();
			startTime = TC2->TC_CHANNEL[2].TC_CV;
			
			//Test uart
// 			Helper::Debug::DebugPrint("TEST PRINT\r\n");
// 			iTest +=1;
// 			
// 			if(P1DecodeValue == 0){
// 				for(auto &ptr: p1msg.OBISChannelList){
// 					Helper::Debug::DebugPrintEX("CHANNEL NUMBER: %i\r\n", ptr->getChannelNumber());
// 					//std::list<OBISObject*> tempList = ptr->getOBISObjectList();
// 					for(auto &Optr: ptr->getOBISObjectList()){
// 						if(Optr->getType() == OBISType::PDelivered){
// 							p1Screen.WriteTextLabel(1, font_ubuntumono_22, std::string("PDel.(+P):\n" + Optr->printValue()).c_str());	
// 						}
// 						
// 					}
// 				}
// 			}
			
		}
		
		if((USART3->US_CSR & US_CSR_RXRDY)){
			USART3->US_CR |= US_CR_RETTO;
			receiveBuffer += ((char)(USART3->US_RHR & 0xFF));
			//get lower 8 bit, might be implicit in cast to char
		}
		
		if(USART3->US_CSR & US_CSR_TIMEOUT){
			//SEND TIMEOUT;
			USART3->US_CR |= US_CR_STTTO;
			
			Helper::Debug::DebugPrint("\r\n\t");
			Helper::Debug::DebugPrint(receiveBuffer + "\r\n\t");	
			
			P1Decoder p1msg;
			int P1DecodeValue = P1Decoder::decodeP1(receiveBuffer.c_str(), p1msg);
			
			if(P1DecodeValue == 0){
				//int deltaDisplayVal = 0;

				OBISObject * deltaP1 = nullptr;
				OBISObject * deltaP2 = nullptr;
			 	for(auto &ptr: p1msg.OBISChannelList){
			 		Helper::Debug::DebugPrintVA("CHANNEL NUMBER: %i\r\n", ptr->getChannelNumber());
			 		//std::list<OBISObject*> tempList = ptr->getOBISObjectList();
			 		for(auto &Optr: ptr->getOBISObjectList()){
			 			if(Optr->getType() == OBISType::PDelivered){
							Helper::Debug::DebugPrint(Optr->print() + "\r\n");	
			 				//p1Screen.WriteTextLabel(1, font_ubuntumono_22, std::string("PDel.(+P):\n" + Optr->printValue()).c_str());
							//p1Screen.WriteTextLabel(1, font_ubuntumono_22, std::string("PDel.(+P):\n" + Optr->printValue()));	
							deltaP1 = Optr;
							//deltaDisplayVal +=1;
			 			}
						if(Optr->getType() == OBISType::PReceived){
							Helper::Debug::DebugPrint(Optr->print() + "\r\n");
							//p1Screen.WriteTextLabel(1, font_ubuntumono_22, std::string("PDel.(+P):\n" + Optr->printValue()).c_str());
							//p1Screen.WriteTextLabel(1, font_ubuntumono_22, std::string("PDel.(+P):\n" + Optr->printValue()));
							deltaP2 = Optr;
							//deltaDisplayVal +=1;
						}
			 		}	 
			 	}
				if((deltaP1 != nullptr) && (deltaP2 != nullptr)){
					p1Screen.WriteTextLabel(0, font_ubuntumono_22, std::string("PDel.(DP):\n" + deltaP1->getMagicDelta(deltaP2)));

				}
			}else{
				Helper::Debug::DebugPrint("ERROR PARSING P1\r\n");
				p1Screen.WriteTextLabel(0, font_ubuntumono_22, std::string("PDel.(DP):\nNaN"));
			}
			
			receiveBuffer = "";
		}
	}
}
