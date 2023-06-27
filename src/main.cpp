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

#include "MenuManager.h"
#include "menuPageSplash.h"

//#include "P1Controller.h"

#include "P1Decoder.h"

void TC0_Handler(){
	
}

uint32_t debugISR;
uint32_t debugISR_PIOB;
uint32_t debugISRPrev;

void PIOD_Handler(){
	uint32_t isrVal = PIOD->PIO_ISR;
	
	debugISR = isrVal;
}

void PIOB_Handler(){
	uint32_t isrVal = PIOB->PIO_ISR;
	
	debugISR_PIOB = isrVal;
}

int SelfTest(ILI9341Driver & LCD, SPIDriver & LCDspi, Usart * p1Telegram);

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	
	/* Disable watchdog timer */
	WDT->WDT_MR |= WDT_MR_WDDIS;
	
	/* Enable clocks for used peripherals */
	PMC->PMC_PCER0 |= (	PMC_PCER0_PID8  | //UART
						PMC_PCER0_PID11 | //PIOA
						PMC_PCER0_PID12 | //PIOB
						PMC_PCER0_PID13 | //PIOC
						PMC_PCER0_PID14 | //PIOD
						PMC_PCER0_PID15 | //PIOE
						PMC_PCER0_PID16 | //PIOF
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
						
	/* TC2 -> Channel 2 : used for Helper::Time functions */
	TC2->TC_CHANNEL[2].TC_CMR =		TC_CMR_TCCLKS_TIMER_CLOCK4 | //~1.5uSec per tick
	TC_CMR_WAVE |
	TC_CMR_WAVSEL_UP ;
		
	TC2->TC_CHANNEL[2].TC_CCR |= TC_CCR_CLKEN; //Enable TC2
	TC2->TC_CHANNEL[2].TC_CCR |= TC_CCR_SWTRG;
	//TC2->TC_BCR |= TC_BCR_SYNC;
	
	/* Setup leds */
	LED debugLED1(PIOB, 27);
	LED debugLED2(PIOA, 21, LED::LEDTYPE::INVERTED);
	LED debugLED3(PIOC, 30, LED::LEDTYPE::INVERTED);
	
	LED powerLed(PIOD, 9, LED::LEDTYPE::OPENCOLLECTOR);
	LED heartbeatLed(PIOD, 10, LED::LEDTYPE::OPENCOLLECTOR);
	LED p1UartReceiveLed(PIOC, 2, LED::LEDTYPE::OPENCOLLECTOR);
	LED debugLED7(PIOC, 4, LED::LEDTYPE::OPENCOLLECTOR);
	
	debugLED1.Off();
	debugLED2.Off();
	debugLED3.Off();	
	
	/* UART Debug port setup */
	
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
	
	/* Rotary encoder setup */
	PINDriver rotaryRight(PIOD, 7); //Arduino due pin 11
	PINDriver rotaryLeft(PIOD, 8); //Arduino due pin 12
	PINDriver rotaryButton(PIOB, 27); //Arduino due pin 13
	
	rotaryLeft.ControllerPIOEnable();
	rotaryRight.ControllerPIOEnable();
	rotaryButton.ControllerPIOEnable();
	
	rotaryLeft.SetInputFilter(PIO_INPUT_FILTER::GLITCH);
	rotaryRight.SetInputFilter(PIO_INPUT_FILTER::GLITCH);
	rotaryButton.SetInputFilter(PIO_INPUT_FILTER::GLITCH);
	
	rotaryLeft.InterruptEnable();
	rotaryRight.InterruptEnable();
	rotaryButton.InterruptEnable();
	
	rotaryLeft.EnablePullUp();
	rotaryRight.EnablePullUp();
	
	rotaryLeft.EnableAdditionalInterruptMode(PIO_LEVEL_SELECT::LOW_LEVEL);
	rotaryRight.EnableAdditionalInterruptMode(PIO_LEVEL_SELECT::LOW_LEVEL);
	rotaryButton.EnableAdditionalInterruptMode(PIO_LEVEL_SELECT::RISING_EDGE);
	
	NVIC_EnableIRQ(PIOD_IRQn);
	NVIC_SetPriority(PIOD_IRQn, 2);
	
	NVIC_EnableIRQ(PIOB_IRQn);
	NVIC_SetPriority(PIOB_IRQn, 2);

	/* Pins used for the display setup */
	PINDriver SPI0_MISO(PIOA, 25);
	PINDriver SPI0_MOSI(PIOA, 26);
	PINDriver SPI0_SPCK(PIOA, 27);
	PINDriver DisplaySS(PIOC, 22);
	PINDriver DisplayDC(PIOC, 29);
	PINDriver DisplayRESET(PIOC, 21);
	
	//CS = 8 = C.22
	//RESET = 9 = C.21
	//D/C = 10 = A.28 / C.29
	
	//Orig
	//PINDriver DisplaySS(PIOA, 28);
	//PINDriver DisplayDC(PIOC, 21);
	//PINDriver DisplayRESET(PIOC, 22);
	
	SPI0_MISO.ControllerPIODisable();
	SPI0_MOSI.ControllerPIODisable();
	SPI0_SPCK.ControllerPIODisable();
	
	DisplaySS.ControllerPIOEnable();
	DisplaySS.SetOutput(PIO_PIN_STATE::LOW); //Low is enable on ILI9341
	
	DisplayDC.ControllerPIOEnable();
	DisplayDC.SetOutput(PIO_PIN_STATE::LOW); //1: data, 0: command
	
	DisplayRESET.ControllerPIOEnable();
	DisplayRESET.SetOutput(PIO_PIN_STATE::LOW); // Active low, must be used to init chip
	DisplayRESET.SetOutput(PIO_PIN_STATE::HIGH);
	
	SPI0_MISO.PeripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_A);
	SPI0_MOSI.PeripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_A);
	SPI0_SPCK.PeripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_A);

	

	
		
	/* USART3 -> Receive P1 telegram */
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
	
	
	debugLED1.On();
	//SPI0
	SPIDriver LCDSpi(SPI0_MISO, SPI0_MOSI, SPI0_SPCK, false, false, SPI0);
	
	//LCD
	ILI9341Driver LCD(320, 240, DisplaySS, DisplayDC, DisplayRESET, LCDSpi);
	
	
	
	/* Self test */	
	if(rotaryButton.GetState() == PIO_PIN_STATE::HIGH){
		//should return selftest erro code enum
		int result = SelfTest(LCD, LCDSpi, USART3);
		
		if(result < 0){
			Helper::Debug::DebugPrint("Self test detected an error: ");
			//case result
			// describe error code from enum
		}
	}
	
	
	

	
	
	//LED Flasher blocking
	for(int i = 0; i < 20; ++i){
		powerLed.Toggle();
		heartbeatLed.Toggle();
		p1UartReceiveLed.Toggle();
		debugLED7.Toggle();
		Helper::Time::delay1_5us(200 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	}

	powerLed.On();
	heartbeatLed.Off();
	p1UartReceiveLed.Off();
	debugLED7.Off();
	
	//Menu
 	MenuManager p1Screen(LCD);
 	
 	p1Screen.SetMenu(&menuPageSplash, ILI_COLORS::BLACK);
 	p1Screen.WriteTextLabel(0, font_ubuntumono_10, "TEST PRINT");
	p1Screen.WriteTextLabel(0, font_ubuntumono_10, "!!!!!!!!!!", true);
	p1Screen.WriteTextLabel(0, font_ubuntumono_10, "@@@@@@@@@@", true);
	p1Screen.WriteTextLabel(0, font_ubuntumono_10, "##########", true);
	p1Screen.WriteTextLabel(0, font_ubuntumono_10, "$$$$$$$$$$", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	p1Screen.WriteTextLabel(0, font_ubuntumono_10, "%%%%%%%%%%", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	p1Screen.WriteTextLabel(0, font_ubuntumono_10, "^^^^^^^^^^", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	p1Screen.WriteTextLabel(0, font_ubuntumono_10, "&&&&&&&&&&", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	p1Screen.WriteTextLabel(0, font_ubuntumono_10, "**********", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	p1Screen.WriteTextLabel(0, font_ubuntumono_10, "((((((((((", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	p1Screen.WriteTextLabel(0, font_ubuntumono_10, "))))))))))", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	p1Screen.WriteTextLabel(0, font_ubuntumono_10, "__________", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	p1Screen.WriteTextLabel(0, font_ubuntumono_10, "+++++++++++", true);
	
	

	
	//Enable all interrupts that have been set in the NVIC.
	__enable_irq();
	
	//Add UART receive as IRQ on prio 0
	
	
	p1Screen.SetMenu(&menuPageMain, ILI_COLORS::BLACK);
	
	std::string receiveBuffer;
	
	USART3->US_CR |= US_CR_STTTO;
	
	unsigned int startTime = TC2->TC_CHANNEL[2].TC_CV;
	
	
	
	uint8_t quickCountUp = 0;
	uint8_t quickCountDown = UINT8_MAX;
	
    while (1){
		
		if(debugISR){
			Helper::Debug::DebugPrint("Debug ISR val : ");
			Helper::Debug::DebugPrint(std::to_string(debugISR));
			Helper::Debug::DebugPrint("\r\n");
			
			debugISR = 0;
		}
			
		/* Heartbeat on led */	
		if(TC2->TC_CHANNEL[2].TC_CV - startTime >= (1 * Helper::Time::TIME_UNIT_1_5US::SECOND)){
			heartbeatLed.Toggle();
			startTime = TC2->TC_CHANNEL[2].TC_CV;			
		}
		
		if((USART3->US_CSR & US_CSR_RXRDY)){
			USART3->US_CR |= US_CR_RETTO;
			receiveBuffer += ((char)(USART3->US_RHR & 0xFF));
			//get lower 8 bit, might be implicit in cast to char
			//p1UartReceiveLed.On();
		}
		
		if(USART3->US_CSR & US_CSR_TIMEOUT){
			//SEND TIMEOUT;
			USART3->US_CR |= US_CR_STTTO;
			
			//p1UartReceiveLed.Off();
			
			//Helper::Debug::DebugPrint("\r\n\t");
			//Helper::Debug::DebugPrint(receiveBuffer + "\r\n\t");	
			
			P1Decoder p1msg;
			int P1DecodeValue = P1Decoder::decodeP1(receiveBuffer.c_str(), p1msg);
			
			if(P1DecodeValue == 0){
				//int deltaDisplayVal = 0;

				OBISObject * deltaP1 = nullptr;
				OBISObject * deltaP2 = nullptr;
			 	for(auto &ptr: p1msg.OBISChannelList){
			 		//Helper::Debug::DebugPrintVA("CHANNEL NUMBER: %i\r\n", ptr->getChannelNumber());
			 		//std::list<OBISObject*> tempList = ptr->getOBISObjectList();
			 		for(auto &Optr: ptr->getOBISObjectList()){
			 			if(Optr->getType() == OBISType::PDelivered){
							//Helper::Debug::DebugPrint(Optr->print() + "\r\n");	
			 				//p1Screen.WriteTextLabel(1, font_ubuntumono_22, std::string("PDel.(+P):\n" + Optr->printValue()).c_str());
							//p1Screen.WriteTextLabel(1, font_ubuntumono_22, std::string("PDel.(+P):\n" + Optr->printValue()));	
							deltaP1 = Optr;
							//deltaDisplayVal +=1;
			 			}
						if(Optr->getType() == OBISType::PReceived){
							//Helper::Debug::DebugPrint(Optr->print() + "\r\n");
							//p1Screen.WriteTextLabel(1, font_ubuntumono_22, std::string("PDel.(+P):\n" + Optr->printValue()).c_str());
							//p1Screen.WriteTextLabel(1, font_ubuntumono_22, std::string("PDel.(+P):\n" + Optr->printValue()));
							deltaP2 = Optr;
							//deltaDisplayVal +=1;
						}
			 		}	 
			 	}
				if((deltaP1 != nullptr) && (deltaP2 != nullptr)){
					//Helper::Debug::DebugPrint("DELTA OF P+- P1\r\n");
					p1Screen.WriteTextLabel(0, font_ubuntumono_16, std::string("PDel.(DP): " + deltaP1->getMagicDelta(deltaP2)), true);
				}
			}else{
				Helper::Debug::DebugPrint("ERROR PARSING P1\r\n");
				p1Screen.WriteTextLabel(0, font_ubuntumono_22, std::string("PDel.(DP):\nNaN"));
			}
			
			receiveBuffer = "";
			
			p1Screen.WriteTextLabel(1, font_ubuntumono_16, std::string("L: " + std::to_string(quickCountUp)), false);
			p1Screen.WriteTextLabel(2, font_ubuntumono_16, std::string(":CENTER:"), false);
			p1Screen.WriteTextLabel(3, font_ubuntumono_16, std::string("R: " + std::to_string(quickCountDown)), false);
			
			quickCountUp++;
			quickCountDown--;
		}
	}
}

int SelfTest(ILI9341Driver & LCD, SPIDriver & LCDspi, Usart * p1Telegram){
	Helper::Debug::DebugPrint("Running self test: \r\n");
	
	//Menu
	MenuManager p1Screen(LCD);
	
	//Test pattern display
	//LCD.SetRotation(ILI_ROTATION_MODE::LANDSCAPE, false, false);
	LCD.SendTestPatternColorBlocks();
	
	//Menu
	MenuManager testMenu(LCD);
	
	testMenu.SetMenu(&fullImageTestPage, ILI_COLORS::BLACK);
	
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	
	testMenu.SetMenu(&menuPageSelfTest, ILI_COLORS::BLACK);
	
	
	testMenu.WriteTextLabel(0, font_ubuntumono_10, "TEST PRINT");
	testMenu.WriteTextLabel(0, font_ubuntumono_10, "!!!!!!!!!!", true);
	testMenu.WriteTextLabel(0, font_ubuntumono_10, "@@@@@@@@@@", true);
	testMenu.WriteTextLabel(0, font_ubuntumono_10, "##########", true);
	testMenu.WriteTextLabel(0, font_ubuntumono_10, "$$$$$$$$$$", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	testMenu.WriteTextLabel(0, font_ubuntumono_10, "%%%%%%%%%%", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	testMenu.WriteTextLabel(0, font_ubuntumono_10, "^^^^^^^^^^", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	testMenu.WriteTextLabel(0, font_ubuntumono_10, "&&&&&&&&&&", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	testMenu.WriteTextLabel(0, font_ubuntumono_10, "**********", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	testMenu.WriteTextLabel(0, font_ubuntumono_10, "((((((((((", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	testMenu.WriteTextLabel(0, font_ubuntumono_10, "))))))))))", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	testMenu.WriteTextLabel(0, font_ubuntumono_10, "__________", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	testMenu.WriteTextLabel(0, font_ubuntumono_10, "+++++++++++", true);
	
	return -1;
}
