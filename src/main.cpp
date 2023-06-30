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

#include "SpiDriver.h"
#include "PinDriver.h"
#include "MenuManager.h"
#include "ILI9341Driver.h"

#include "Led.h"
#include "Helper.h"

#include "font-ubuntumono-10.h"
#include "font-ubuntumono-16.h"
#include "font-ubuntumono-22.h"
#include "font-ubuntumono-28.h"
#include "font-ubuntumono-34.h"
#include "font-ubuntumono-40.h"

#include "MenuManager.h"
#include "menuPageSplash.h"

//#include "P1Controller.h"

#include "P1Decoder.h"

//String encoded for PC testing purpose
//testP1Telegram is the example telegram from P1 compendium document,
//The example is wrong in this document, this is the corrected version.
const char * testP1Telegram = "/ISk5\\2MT382-1000\r\n\r\n1-3:0.2.8(50)\r\n0-0:1.0.0(101209113020W)\r\n0-0:96.1.1(4B384547303034303436333935353037)\r\n1-0:1.8.1(123456.789*kWh)\r\n1-0:1.8.2(123456.789*kWh)\r\n1-0:2.8.1(123456.789*kWh)\r\n1-0:2.8.2(123456.789*kWh)\r\n0-0:96.14.0(0002)\r\n1-0:1.7.0(01.193*kW)\r\n1-0:2.7.0(00.000*kW)\r\n0-0:96.7.21(00004)\r\n0-0:96.7.9(00002)\r\n1-0:99.97.0(2)(0-0:96.7.19)(101208152415W)(0000000240*s)(101208151004W)(0000000301*s)\r\n1-0:32.32.0(00002)\r\n1-0:52.32.0(00001)\r\n1-0:72.32.0(00000)\r\n1-0:32.36.0(00000)\r\n1-0:52.36.0(00003)\r\n1-0:72.36.0(00000)\r\n0-0:96.13.0(303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F)\r\n1-0:32.7.0(220.1*V)\r\n1-0:52.7.0(220.2*V)\r\n1-0:72.7.0(220.3*V)\r\n1-0:31.7.0(001*A)\r\n1-0:51.7.0(002*A)\r\n1-0:71.7.0(003*A)\r\n1-0:21.7.0(01.111*kW)\r\n1-0:41.7.0(02.222*kW)\r\n1-0:61.7.0(03.333*kW)\r\n1-0:22.7.0(04.444*kW)\r\n1-0:42.7.0(05.555*kW)\r\n1-0:62.7.0(06.666*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(3232323241424344313233343536373839)\r\n0-1:24.2.1(101209112500W)(12785.123*m3)\r\n!E47C";

const char * testP1Telegram2 = "/KFM5KAIFA-METER\r\n\r\n1-3:0.2.8(42)\r\n0-0:1.0.0(170124213128W)\r\n0-0:96.1.1(4530303236303030303234343934333135)\r\n1-0:1.8.1(000306.946*kWh)\r\n1-0:1.8.2(000210.088*kWh)\r\n1-0:2.8.1(000000.000*kWh)\r\n1-0:2.8.2(000000.000*kWh)\r\n0-0:96.14.0(0001)\r\n1-0:1.7.0(02.793*kW)\r\n1-0:2.7.0(00.000*kW)\r\n0-0:96.7.21(00001)\r\n0-0:96.7.9(00001)\r\n1-0:99.97.0(1)(0-0:96.7.19)(000101000006W)(2147483647*s)\r\n1-0:32.32.0(00000)\r\n1-0:52.32.0(00000)\r\n1-0:72.32.0(00000)\r\n1-0:32.36.0(00000)\r\n1-0:52.36.0(00000)\r\n1-0:72.36.0(00000)\r\n0-0:96.13.1()\r\n0-0:96.13.0()\r\n1-0:31.7.0(003*A)\r\n1-0:51.7.0(005*A)\r\n1-0:71.7.0(005*A)\r\n1-0:21.7.0(00.503*kW)\r\n1-0:41.7.0(01.100*kW)\r\n1-0:61.7.0(01.190*kW)\r\n1-0:22.7.0(00.000*kW)\r\n1-0:42.7.0(00.000*kW)\r\n1-0:62.7.0(00.000*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(4730303331303033333738373931363136)\r\n0-1:24.2.1(170124210000W)(00671.790*m3)\r\n!29ED";

const char * testP1Telegram3 = "/JFR5SIM-P1-METER\r\n\r\n1-3:0.2.8(50)\r\n0-0:1.0.0(170124213128W)\r\n0-0:96.1.1(4530303236303030303234343934333135)\r\n1-0:1.8.1(000411.141*kWh)\r\n!0431";

const char * testP1Telegram4 = "/JFR5SIM-P1-METER\r\n\r\n1-3:0.2.8(50)\r\n0-0:1.0.0(170124213128W)\r\n0-0:96.1.1(4530303236303030303234343934333135)\r\n1-0:1.7.0(000482.745*kWh)\r\n!1793";

const char * testP1TelegramCapture = "/ISK5\\2M550E-1012\r\n\r\n1-3:0.2.8(50)\r\n0-0:1.0.0(230615210817S)\r\n0-0:96.1.1(4530303433303037343231343730323139)\r\n1-0:1.8.1(008378.196*kWh)\r\n1-0:1.8.2(004019.445*kWh)\r\n1-0:2.8.1(000499.523*kWh)\r\n1-0:2.8.2(001403.870*kWh)\r\n0-0:96.14.0(0001)\r\n1-0:1.7.0(00.368*kW)\r\n1-0:2.7.0(00.128*kW)\r\n0-0:96.7.21(00008)\r\n0-0:96.7.9(00005)\r\n1-0:99.97.0(3)(0-0:96.7.19)(190514051940S)(0000000554*s)(200924114954S)(0000008885*s)(211208115451W)(0000011472*s)\r\n1-0:32.32.0(00007)\r\n1-0:32.36.0(00001)\r\n0-0:96.13.0()\r\n1-0:32.7.0(238.2*V)\r\n1-0:31.7.0(002*A)\r\n1-0:21.7.0(00.368*kW)\r\n1-0:22.7.0(00.000*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(4730303339303031393335343838303139)\r\n0-1:24.2.1(230615210459S)(04490.555*m3)\r\n!8CFC";

const char * testP1TelegramCaptureNegVal = "/ISK5\\2M550E-1012\r\n\r\n1-3:0.2.8(50)\r\n0-0:1.0.0(230615210817S)\r\n0-0:96.1.1(4530303433303037343231343730323139)\r\n1-0:1.8.1(008378.196*kWh)\r\n1-0:1.8.2(004019.445*kWh)\r\n1-0:2.8.1(000499.523*kWh)\r\n1-0:2.8.2(001403.870*kWh)\r\n0-0:96.14.0(0001)\r\n1-0:1.7.0(00.368*kW)\r\n1-0:2.7.0(00.828*kW)\r\n0-0:96.7.21(00008)\r\n0-0:96.7.9(00005)\r\n1-0:99.97.0(3)(0-0:96.7.19)(190514051940S)(0000000554*s)(200924114954S)(0000008885*s)(211208115451W)(0000011472*s)\r\n1-0:32.32.0(00007)\r\n1-0:32.36.0(00001)\r\n0-0:96.13.0()\r\n1-0:32.7.0(238.2*V)\r\n1-0:31.7.0(002*A)\r\n1-0:21.7.0(00.368*kW)\r\n1-0:22.7.0(00.000*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(4730303339303031393335343838303139)\r\n0-1:24.2.1(230615210459S)(04490.555*m3)\r\n!BDD3";

const char * testP1TelegramCaptureError = "/ISK5\\2M550E-1012\r\n\r\n1-3:0.2.8(50)\r\n0-0:1.0.0(230616144327S)\r\n0-0:96.1.1(4530303433303037343231343730323139)\r\n1-0:1.8.1(008382.642*kWh)\r\n1-0:1.8.2(004019.737*kWh)\r\n1-0:2.8.1(000499.523*kWh)\r\n1-0:2.8.2(001406.900*kWh)\r\n0-0:96.14.0(0002)\r\n1-0:1.7.0(00.000*kW)\r\n1-0:2.7.0(00.499*kW)\r\n0-0:96.7.21(00008)\r\n0-0:96.7.9(00005)\r\n1-0:99.97.0(3)(0-0:96.7.19)(190514051940S)(0000000554*s)(200924114954S)(0000008885*s)(211208115451W)(0000011472*s)\r\n1-0:32.32.0(00007)\r\n1-0:32.36.0(00001)\r\n0-0:96.13.0()\r\n1-0:32.7.0(238.6*V)\r\n1-0:31.7.0(002*A)\r\n1-0:21.7.0(00.000*kW)\r\n1-0:22.7.0(00.493*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(4730303339303031393335343838303139)\r\n0-1:24.2.1(230616144002S)(04490.851*m3)\r\n!B1F2\r\n";


enum class SelfTestErrorCode{
	P1DecodeError = -1,
	P1ObjectListError = -2,
	P1CosemObjectError = -2,
	
};

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

SelfTestErrorCode SelfTest(ILI9341Driver & LCD, SpiDriver & LCDspi, Usart * p1Telegram);

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
	Led debugLED1(PIOB, 27);
	Led debugLED2(PIOA, 21, Led::LEDTYPE::INVERTED);
	Led debugLED3(PIOC, 30, Led::LEDTYPE::INVERTED);
	
	Led powerLed(PIOD, 9, Led::LEDTYPE::OPENCOLLECTOR);
	Led heartbeatLed(PIOD, 10, Led::LEDTYPE::OPENCOLLECTOR);
	Led p1UartReceiveLed(PIOC, 2, Led::LEDTYPE::OPENCOLLECTOR);
	Led debugLED7(PIOC, 4, Led::LEDTYPE::OPENCOLLECTOR);
	
	debugLED1.Off();
	debugLED2.Off();
	debugLED3.Off();	
	
	/* UART Debug port setup */
	
	//UART 0
	//A.9 = UTXD TX0
	//A.8 = URXD RX0
	
	PinDriver uartRX(PIOA, 8);
	PinDriver uartTX(PIOA, 9);
	
	uartRX.ControllerPIODisable();
	uartTX.ControllerPIODisable();
	
	uartRX.PeripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_A);
	uartTX.PeripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_A);
	
	UART->UART_BRGR = UART_BRGR_CD(46); //Baud rate ~115200
	UART->UART_CR |= (UART_CR_TXEN);
	
	//wait for UART ready
	while(!(UART->UART_SR & UART_SR_TXRDY)){}
	
	/* Rotary encoder setup */
	PinDriver rotaryRight(PIOD, 7); //Arduino due pin 11
	PinDriver rotaryLeft(PIOD, 8); //Arduino due pin 12
	PinDriver rotaryButton(PIOB, 27); //Arduino due pin 13
	
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
	PinDriver SPI0_MISO(PIOA, 25);
	PinDriver SPI0_MOSI(PIOA, 26);
	PinDriver SPI0_SPCK(PIOA, 27);
	PinDriver DisplaySS(PIOC, 22);
	PinDriver DisplayDC(PIOC, 29);
	PinDriver DisplayRESET(PIOC, 21);
	
	//CS = 8 = C.22
	//RESET = 9 = C.21
	//D/C = 10 = A.28 / C.29
	
	//Orig
	//PinDriver DisplaySS(PIOA, 28);
	//PinDriver DisplayDC(PIOC, 21);
	//PinDriver DisplayRESET(PIOC, 22);
	
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
	
	PinDriver usart3TX(PIOD, 4);
	PinDriver usart3RX(PIOD, 5);
	
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
	//SpiDriver LCDSpi(SPI0_MISO, SPI0_MOSI, SPI0_SPCK, false, false, SPI0);
	SpiDriver LCDSpi(SPI0, false, false);
	
	//LCD
	ILI9341Driver LCD(320, 240, DisplaySS, DisplayDC, DisplayRESET, LCDSpi);
	
	
	
	/* Self test */	
	if(rotaryButton.GetState() == PIO_PIN_STATE::HIGH){
		//should return selftest erro code enum
		SelfTestErrorCode result = SelfTest(LCD, LCDSpi, USART3);
		
		if(static_cast<int>(result) < 0){
			Helper::Debug::DebugPrint(	"Self test detected an error: " 
										+ std::to_string(static_cast<int>(result)));
		}
	}
	

	//Led Flasher blocking
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
			int P1DecodeValue = p1msg.decodeP1String(receiveBuffer.c_str());
			
			if(P1DecodeValue == 0){				
				
				std::string deltaVal = p1msg.getDeltaString(0, ObisType::PDelivered,
															0, ObisType::PReceived);
				
				p1Screen.WriteTextLabel(0, font_ubuntumono_16, std::string("PDel.(DP): " + deltaVal), true);
				
			}else{
				Helper::Debug::DebugPrint("ERROR PARSING P1\r\n");
				p1Screen.WriteTextLabel(0, font_ubuntumono_16, std::string("PARSE ERROR P1"));
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



SelfTestErrorCode SelfTest(ILI9341Driver & LCD, SpiDriver & LCDspi, Usart * p1Telegram){
	Helper::Debug::DebugPrint("Running self test: \r\n");
	
	//Menu
	MenuManager p1Screen(LCD);
	
	//Test pattern display
	//LCD.SetRotation(ILI_ROTATION_MODE::LANDSCAPE, false, false);
	LCD.SendTestPatternColorBlocks();
	
	//Menu
	MenuManager testMenu(LCD);
	
	testMenu.SetMenu(&menuPageSelfTest, ILI_COLORS::BLACK);
	testMenu.WriteTextLabel(0, font_ubuntumono_10, "Test full image in menu context");
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	
	//testMenu.SetMenu(&fullImageTestPage, ILI_COLORS::BLACK);
	
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	
	testMenu.SetMenu(&menuPageSelfTest, ILI_COLORS::BLACK);
	
	
	testMenu.WriteTextLabel(0, font_ubuntumono_10, "TEST PRINT", true);
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
		
	
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	testMenu.ClearTextLabel(0, ILI_COLORS::BLACK);
	
	/* P1 decoder test */
	testMenu.WriteTextLabel(0, font_ubuntumono_10, "P1 Decoder test:", true);
	
	P1Decoder p1test;
	
	/* Decode a correct string */
	std::string currentTestP1Telegram = testP1TelegramCapture;
	
	testMenu.WriteTextLabel(0, font_ubuntumono_10, "P1 Decoder test:", true);
	int P1DecodeValue = p1test.decodeP1String(currentTestP1Telegram.c_str());
	
	testMenu.WriteTextLabel(0, font_ubuntumono_10, ("Decode value : " + std::to_string(P1DecodeValue) + "expected 0"), true);
	if (P1DecodeValue != 0){
		return SelfTestErrorCode::P1DecodeError;
	}
	testMenu.WriteTextLabel(0, font_ubuntumono_10, ("-Passed 1/X-"), true);

	testMenu.WriteTextLabel(0, font_ubuntumono_10, ("ID : " + p1test.getDeviceIdentifier() + "expected \\2M550E-1012"), true);
	if(p1test.getDeviceIdentifier() == "\\2M550E-1012"){	
		testMenu.WriteTextLabel(0, font_ubuntumono_10, ("-Passed 2/X-"), true);
	}
	
	/* Test for channel count */
	int channelCount = p1test.getCosemChannelCount();
	
	if (channelCount != 2){
		testMenu.WriteTextLabel(0, font_ubuntumono_10, ("Channel count : " + std::to_string(channelCount) + "expected 2"), true);
		return SelfTestErrorCode::P1DecodeError;
	}
	
	testMenu.WriteTextLabel(0, font_ubuntumono_10, ("Channel count : " + std::to_string(channelCount) + "expected 2"), true);
	testMenu.WriteTextLabel(0, font_ubuntumono_10, ("-Passed 3/X-"), true);
	
	//PART TWO
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	
	testMenu.ClearTextLabel(0, ILI_COLORS::BLACK);
	
	/* Test for correct amount of objects in channel */

	if(p1test.getCosemChannelSize(0) != 20){
		return SelfTestErrorCode::P1ObjectListError;
	}
	
	testMenu.WriteTextLabel(0, font_ubuntumono_10, ("-Passed 4/X-"), true);
	
	if(p1test.getCosemChannelSize(1) != 3){
		return SelfTestErrorCode::P1ObjectListError;
	}
	
	testMenu.WriteTextLabel(0, font_ubuntumono_10, ("-Passed 5/X-"), true);
	
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	
	testMenu.ClearTextLabel(0, ILI_COLORS::BLACK);
	
	
	/* Testing Cosem objects in channels*/
	
	/* Test CosemObject: 1
	 * Verify if values in deltaP1 & deltaP2 are correct.
	 */
	
	auto deltaP1String = p1test.getCosemStringFromChannel(0, ObisType::PDelivered);
	auto deltaP2String = p1test.getCosemStringFromChannel(0, ObisType::PReceived);
	
	if(deltaP1String != "Power delivered (+P): 0.368kW"){
		return SelfTestErrorCode::P1CosemObjectError;
	}
	if(deltaP2String != "Power received (-P): 0.128kW"){
		return SelfTestErrorCode::P1CosemObjectError;
	}
	testMenu.WriteTextLabel(0, font_ubuntumono_10, ("Cosem object <float> Passed 1/X-"), true);
	
	/* Test CosemObject: 2
	 * Verify delta calculation of two objects works.
	 */
	
	auto deltaString = p1test.getDeltaString(0, ObisType::PDelivered, 0, ObisType::PReceived);
	
	//std::string deltaCalc = deltaP1->getDelta(deltaP2);
	if(deltaString != "0.240kW"){
		return SelfTestErrorCode::P1CosemObjectError;
	}
	testMenu.WriteTextLabel(0, font_ubuntumono_10, ("Cosem object <float> Passed 2/X-"), true);
	
	/* Test CosemObject: 3
	 * Verify delta of two objects that can't be calculated
	 */
	
	
	return SelfTestErrorCode::P1DecodeError;
}
