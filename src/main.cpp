/*
 * P1Meter.cpp
 *
 * Created: 29-4-2023 21:07:28
 * Author : Jordan
 */ 

// #define MAIN_MENU 0
// #define DISP_MENU 1
// #define DELTA_MENU 2


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
#include "Button.h"
#include "RotaryEncoder.h"
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

#include "Interruptible.h"

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




void TC0_Handler(){
	
}

uint32_t value_ISR_PIOD;
uint32_t value_ISR_PIOB;
//uint32_t debugISRPrev;

Interrupt piodCalls;

void PIOD_Handler(){
	uint32_t isrVal = PIOD->PIO_ISR;
	
	piodCalls.updateInterrupt(PIOB_IRQn, isrVal);
	
	value_ISR_PIOD = isrVal;
}

Interrupt piobCalls;

void PIOB_Handler(){
	uint32_t isrVal = PIOB->PIO_ISR;
	
	piobCalls.updateInterrupt(PIOD_IRQn, isrVal);
	
	value_ISR_PIOB = isrVal;
}

enum class SelfTestErrorCode{
	P1DecodeError = -1,
	P1ObjectListError = -2,
	P1CosemObjectError = -2,
	
};

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
	TC2->TC_CHANNEL[2].TC_CMR =	TC_CMR_TCCLKS_TIMER_CLOCK4 | //~1.5uSec per tick
								TC_CMR_WAVE |
								TC_CMR_WAVSEL_UP ;
		
	TC2->TC_CHANNEL[2].TC_CCR |= TC_CCR_CLKEN; //Enable TC2
	TC2->TC_CHANNEL[2].TC_CCR |= TC_CCR_SWTRG;
	//TC2->TC_BCR |= TC_BCR_SYNC;
	
	/* Setup leds */
	//Led debugLED1(PIOB, 27);
	//Led debugLED2(PIOA, 21, Led::LEDTYPE::INVERTED);
	//Led debugLED3(PIOC, 30, Led::LEDTYPE::INVERTED);
	
	Led powerLed(PIOD, 9, Led::LEDTYPE::OPENCOLLECTOR);
	Led heartbeatLed(PIOD, 10, Led::LEDTYPE::OPENCOLLECTOR);
	Led p1UartReceiveLed(PIOC, 2, Led::LEDTYPE::OPENCOLLECTOR);
	Led debugLED7(PIOC, 4, Led::LEDTYPE::OPENCOLLECTOR);
	
	//debugLED1.off();
	//debugLED2.off();
	//debugLED3.off();	
	
	/* UART Debug port setup */
	
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
	
	/* Rotary encoder setup */
	PinDriver rotaryRight(PIOD, 7); //Arduino due pin 11
	PinDriver rotaryLeft(PIOD, 8); //Arduino due pin 12
	Button rotaryButton(PIOB, 27); //Arduino due pin 13
	
	rotaryLeft.controllerPioEnable();
	rotaryRight.controllerPioEnable();
	rotaryButton.controllerPioEnable();
	
	rotaryLeft.setInputFilter(PIO_INPUT_FILTER::GLITCH);
	rotaryRight.setInputFilter(PIO_INPUT_FILTER::GLITCH);
	rotaryButton.setInputFilter(PIO_INPUT_FILTER::GLITCH);
	
	rotaryLeft.interruptEnable();
	rotaryRight.interruptEnable();
	rotaryButton.interruptEnable();
	
	rotaryLeft.enablePullUp();
	rotaryRight.enablePullUp();
	
	rotaryLeft.disableAdditionalInterruptMode(); //Default triggers on both edges
	rotaryRight.disableAdditionalInterruptMode(); //Default triggers on both edges
	
	//rotaryLeft.enableAdditionalInterruptMode(PIO_LEVEL_SELECT::LOW_LEVEL);
	//rotaryRight.enableAdditionalInterruptMode(PIO_LEVEL_SELECT::LOW_LEVEL);
	rotaryButton.enableAdditionalInterruptMode(PIO_LEVEL_SELECT::RISING_EDGE);
	
	piodCalls.registerInterrupt(&rotaryRight);
	piodCalls.registerInterrupt(&rotaryLeft);
	piobCalls.registerInterrupt(&rotaryButton);
	
	RotaryEncoder testEncoder(	PinDriver(PIOD, 8), PIOD_IRQn,
								PinDriver(PIOD, 7), PIOD_IRQn);
	
	piodCalls.registerInterrupt(&testEncoder);

	NVIC_EnableIRQ(PIOD_IRQn);
	NVIC_SetPriority(PIOD_IRQn, 2);
	
	NVIC_EnableIRQ(PIOB_IRQn);
	NVIC_SetPriority(PIOB_IRQn, 2);

	/* Pins used for the display setup */
	PinDriver SPI0_MISO(PIOA, 25);
	PinDriver SPI0_MOSI(PIOA, 26);
	PinDriver SPI0_SPCK(PIOA, 27);
	PinDriver DisplayCS(PIOC, 22);
	PinDriver DisplayDC(PIOC, 29);
	PinDriver DisplayRESET(PIOC, 21);
	
	SPI0_MISO.controllerPioDisable();
	SPI0_MOSI.controllerPioDisable();
	SPI0_SPCK.controllerPioDisable();
	
	DisplayCS.controllerPioEnable();
	DisplayCS.setOutput(PIO_PIN_STATE::LOW); //Low is enable on ILI9341
	
	DisplayDC.controllerPioEnable();
	DisplayDC.setOutput(PIO_PIN_STATE::LOW); //1: data, 0: command
	
	DisplayRESET.controllerPioEnable();
	DisplayRESET.setOutput(PIO_PIN_STATE::LOW); // Active low, must be used to init chip
	DisplayRESET.setOutput(PIO_PIN_STATE::HIGH);
	
	SPI0_MISO.peripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_A);
	SPI0_MOSI.peripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_A);
	SPI0_SPCK.peripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_A);

	

	
		
	/* USART3 -> Receive P1 telegram */

	PinDriver usart3TX(PIOD, 4);
	PinDriver usart3RX(PIOD, 5);
	
	usart3TX.controllerPioDisable();
	usart3RX.controllerPioDisable();
	
	usart3TX.peripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_B);
	usart3RX.peripheralABSelect(PIO_ABSR_SELECT::PIO_ABSR_B);
	
	USART3->US_BRGR = UART_BRGR_CD(46);
	USART3->US_CR |= (US_CR_RXEN);
	USART3->US_MR |= US_MR_USART_MODE_NORMAL | US_MR_CHRL_8_BIT | US_MR_PAR_NO | US_MR_NBSTOP_1_BIT | US_MR_CHMODE_NORMAL;
	USART3->US_RTOR = US_RTOR_TO(500);
	
	
	/* Application setup */
	//debugLED1.on();
	
	//SPI0
	SpiDriver lcdSpi(SPI0, false, false);
	
	//lcd
	ILI9341Driver lcd(320, 240, DisplayCS, DisplayDC, DisplayRESET, lcdSpi);

	/* Self test */	
	if(rotaryButton.getState() == PIO_PIN_STATE::HIGH){
		//should return selftest erro code enum
		SelfTestErrorCode result = SelfTest(lcd, lcdSpi, USART3);
		
		if(static_cast<int>(result) < 0){
			Helper::Debug::DebugPrint(	"Self test detected an error: " 
										+ std::to_string(static_cast<int>(result)));
		}
	}
	
	//Led Flasher blocking
	for(int i = 0; i < 20; ++i){
		powerLed.toggle();
		heartbeatLed.toggle();
		p1UartReceiveLed.toggle();
		debugLED7.toggle();
		Helper::Time::delay1_5us(150 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	}

	powerLed.on();
	heartbeatLed.off();
	p1UartReceiveLed.off();
	debugLED7.off();
	
	//Menu
 	MenuManager p1_screen(lcd);
 	
 	p1_screen.setMenu(&menuPageSplash, ILI_COLORS::BLACK);
 	p1_screen.writeTextLabel(0, font_ubuntumono_10, "TEST PRINT", false);
	 
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	
	//Enable all interrupts that have been set in the NVIC.
	__enable_irq();
	
	//Add UART receive as IRQ on prio 0
	
	p1_screen.setMenu(&menuPageMain, ILI_COLORS::BLACK);
	
	std::string receive_buffer;
	
	USART3->US_CR |= US_CR_STTTO;
	
	unsigned int startTime = TC2->TC_CHANNEL[2].TC_CV;
	
	

	unsigned int blockTime = TC2->TC_CHANNEL[2].TC_CV;
	
	static unsigned int button_block_time = TC2->TC_CHANNEL[2].TC_CV;
	static bool block_button = false;

	
	enum APP_STATE{
		MAIN_MENU = 0,
		DISP_MENU = 1,
		DELTA_MENU = 2,
	};
	
	static int app_state = MAIN_MENU;

	static bool goLeft = false;
	static bool goRight = false;
	static bool goButton = false;
	static int goButtonHold = 0;
	static int parseList = 0;


	testEncoder.reset();
	rotaryButton.resetButton();
	//rotaryLeft.resetButton();
	//rotaryRight.resetButton();
	
	testEncoder.reset();
	
	static bool drawMenu = true;
	
	static bool show_delta = false;
	
	static bool mm_selected = true;
	
	struct deltaObject{
		int channel_n;
		ObisType delta_1;
		ObisType delta_2;
	};
	
	deltaObject dsp_delta = {0, ObisType::PDelivered, ObisType::PReceived};
	
	struct displayObject{
		ObisType type;
		int channelN;
	};
	
	static std::vector<displayObject> display_list;
	
    while (1){
		
		auto setAppState = [](int n)	{
			app_state = n;
			goLeft = false;
			goRight = false;
			goButton = false;
			goButtonHold = 0;
			//reset state for this menu
			drawMenu = true;
		};
		
		auto resetButtonBlock = [](){
			block_button = true;
			button_block_time = TC2->TC_CHANNEL[2].TC_CV;
			goButton = false;
		};
		
		if(TC2->TC_CHANNEL[2].TC_CV - blockTime >= (350 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND)){
			if(testEncoder.getRotation()){
				int val = testEncoder.getRotation();
				if(val < 0){
					//Left
					goLeft = true;
				}else{
					//Right
					goRight = true;
				}
				
				testEncoder.reset();
			}
			
			if(rotaryButton.getStateBool()){
				goButtonHold += 1;
			}else{
				goButtonHold = 0;
			}
			
			
			blockTime = TC2->TC_CHANNEL[2].TC_CV;
		}
		
		if(TC2->TC_CHANNEL[2].TC_CV - button_block_time >= (350 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND)){
			block_button = false;
		}
		
		if(!block_button){
			if(rotaryButton.wasPressed()){
				goButton = true;
				Helper::Debug::DebugPrint("rotaryButton press detected\r\n");
				rotaryButton.resetButton();
			}
		}
		
	
		
			
		/* Heartbeat on led */	
		if(TC2->TC_CHANNEL[2].TC_CV - startTime >= (1 * Helper::Time::TIME_UNIT_1_5US::SECOND)){
			heartbeatLed.toggle();
			startTime = TC2->TC_CHANNEL[2].TC_CV;			
		}
		
		/* Start uart msg receive */
		if((USART3->US_CSR & US_CSR_RXRDY)){
			USART3->US_CR |= US_CR_RETTO;
			receive_buffer += static_cast<char>(USART3->US_RHR & 0xFF);
			//get lower 8 bit, might be implicit in cast to char
			//p1UartReceiveLed.On();
		}
		
		/* End of message */
		if(USART3->US_CSR & US_CSR_TIMEOUT){
			//RESET TIMEOUT;
			USART3->US_CR |= US_CR_STTTO;
			
			//p1UartReceiveLed.Off();
			
			if(app_state == MAIN_MENU){
			//if(1){
				
				//p1_screen.setMenu(&menuPageMain, ILI_COLORS::BLACK);
				
				Helper::Debug::DebugPrint(receive_buffer);
				
				P1Decoder p1msg;
				int p1_decode_value = p1msg.decodeP1String(receive_buffer.c_str());
				
				if(drawMenu){
					p1_screen.setMenu(&menuPageMain, ILI_COLORS::BLACK, true);
					drawMenu = false;
				}
				
				if(p1_decode_value == 0){
					
					
					
					int no_reset = false;
					
					for(displayObject to_disp: display_list){
						std::string item_ = p1msg.getCosemStringFromChannel(to_disp.channelN, to_disp.type);
						p1_screen.writeTextLabel(0, font_ubuntumono_16, item_, no_reset);
						no_reset = true;
					} 
					
					if(show_delta){
						std::string deltaVal = p1msg.getDeltaString(dsp_delta.channel_n, dsp_delta.delta_1, dsp_delta.delta_2);
						p1_screen.writeTextLabel(0, font_ubuntumono_16, std::string("DeltaV: ") + deltaVal, no_reset);
						no_reset = true;	
					}
					
					p1_screen.writeTextLabel(1, font_ubuntumono_16, "MENU", false, mm_selected);

					p1_screen.writeTextLabel(3, font_ubuntumono_16, "D-MENU", false, !mm_selected);
					
					if(!no_reset){
						p1_screen.writeTextLabel(0, font_ubuntumono_16, "SELECT ITEMS IN MENU", false);
					}
					
					p1_screen.clearAfterWrite(0);
					
				}else{
					Helper::Debug::DebugPrint("ERROR PARSING P1\r\n");
					p1_screen.writeTextLabel(0, font_ubuntumono_16, "PARSE ERROR P1", false);
				}
				
				
						
			}
			
			//Actions that do not require the menu
			//Any additional logic can be added here
			
			receive_buffer.clear();

		}

		if(app_state == MAIN_MENU){
			
 			
 			
 			
 			//p1_screen.writeTextLabel(1, font_ubuntumono_16, "MENU", false, mm_selected);

 			//p1_screen.writeTextLabel(3, font_ubuntumono_16, "D-MENU", false, !mm_selected);
			
			if(goLeft){
				goLeft = false;
				
				mm_selected = !mm_selected;
			}
			
			if(goRight){
				goRight = false;
				
				mm_selected = !mm_selected;
			}
			
			if(goButton){
				resetButtonBlock();
				//goButton = false;
				
				if(mm_selected){
					setAppState(DISP_MENU);
				}
				
				if(!mm_selected){
					setAppState(DELTA_MENU);
				}
						
				
			}
			
		}//END MAIN_MENU
			
		
		if(app_state == DISP_MENU){
			
			static int selected_menu = 0;
			static bool submenu_ = false;
			static int channel_ = 0;
			
			auto selectedMenuInverted = [](int n)	{
				if(selected_menu == n){
					return true;
				}
				return false;
			};
						
			if(drawMenu){
				p1_screen.setMenu(&menuPageMain, ILI_COLORS::BLACK, true);
				drawMenu = false;
				
				parseList = 0;
				submenu_ = false;
			}
			
			std::string listedString = "ADD ITEM";
			
			for(displayObject &ref: display_list){
				getObisTypeString(static_cast<ObisType>(parseList));
				std::cout << ref.channelN << std::endl;
				if(	ref.type == static_cast<ObisType>(parseList) &&
					ref.channelN == channel_){
						listedString = "REMOVE THIS";
						break;
					}
			}
			
			p1_screen.writeTextLabel(0, font_ubuntumono_16, listedString, false);
			
			if(selectedMenuInverted(0) && submenu_){
				p1_screen.setInvertedBackgroundColor(ILI_COLORS::ORANGE);
			}
			p1_screen.writeTextLabel(0, font_ubuntumono_16, "<- SELECT DISPLAY ->", true, selectedMenuInverted(0));	
			p1_screen.setInvertedBackgroundColor(ILI_COLORS::WHITE);
			
			
			if(selectedMenuInverted(1) && submenu_){
				p1_screen.setInvertedBackgroundColor(ILI_COLORS::ORANGE);
			}
			p1_screen.writeTextLabel(1, font_ubuntumono_16, std::string("CH: ") + std::to_string(channel_), true, selectedMenuInverted(1));
			p1_screen.setInvertedBackgroundColor(ILI_COLORS::WHITE);
			
			p1_screen.writeTextLabel(2, font_ubuntumono_16, "ADD/REM", true, selectedMenuInverted(2));
			p1_screen.writeTextLabel(3, font_ubuntumono_16, "EXIT", true, selectedMenuInverted(3));
			
			if(goLeft){
				goLeft = false;
				
				if(!submenu_){
					if(!(selected_menu <= 0)){
						selected_menu -= 1;
					}else{
						selected_menu = 0;
					}
				}
				
				if(submenu_ && selected_menu == 0){
					parseList -= 1;
					
					if(parseList < 0){
						parseList = 0;
					}
				}
				
				if(submenu_ && selected_menu == 1){
					channel_ -= 1;
					
					if(channel_ < 0){
						channel_ = 9;
					}
				}
							
				
				
			}else if(goRight){
				goRight = false;
				
				if(!submenu_){
					if(!(selected_menu >= 3)){
						selected_menu += 1;
						}else{
						selected_menu = 3;
					}
				}
				
				if(submenu_ && selected_menu == 0){
					parseList += 1;
					
					int c = static_cast<int>(ObisType::COUNT);
					if(parseList >= c){
						parseList = c -1;
					}
				}		
				
				if(submenu_ && selected_menu == 1){
					channel_ += 1;
					
					if(channel_ > 9){
						channel_ = 0;
					}
				}
								
				
			}
			
			p1_screen.writeTextLabel(0, font_ubuntumono_16, getObisTypeString(static_cast<ObisType>(parseList)), true);
			p1_screen.clearAfterWrite(0);
			
			if(goButton){
				resetButtonBlock();
				//goButton = false;
				
				if(selected_menu == 0){
					submenu_ = !submenu_;
				}
				
				if(selected_menu == 1){
					submenu_ = !submenu_;
				}
				
				if(selected_menu == 3){
					setAppState(MAIN_MENU);
					channel_ = 0;
				}
				
				if(selected_menu == 2){
					Helper::Debug::DebugPrint("ADD OR REMOVE OBISString\r\n");
					//Remove obis object
					std::vector<displayObject>::iterator it = display_list.begin();
					
					bool removedItem = false;
					
					while(it != display_list.end()){
						if( it->type == static_cast<ObisType>(parseList) &&
							it->channelN == channel_){
							it = display_list.erase(it);
							removedItem = true;
						}else{
							++it;
						}
					}										if(!removedItem){
						//ADD
						display_list.push_back({static_cast<ObisType>(parseList), channel_});
					}

				}
			}
			
			if(goButtonHold == 3 && selected_menu == 3){
				setAppState(MAIN_MENU);
				channel_ = 0;
			}
		}//END DISP_MENU
	
		if(app_state == DELTA_MENU){			
 			enum internalstate{
 				SHOW_DELTA = 0,
				CHANNEL = 1,	
 				SELECT_D1 = 2,
 				SELECT_D2 = 3,
 				EXIT = 4,
 			};
 			
 			static internalstate in_state = SHOW_DELTA;
			
			static bool submenu = false;
			//static int channel_number = 0;
 			
 			auto deltaMenuSelected = [](internalstate n)	{
 				if(in_state == n){
 					return true;
 				}
 				return false;
 			};
 			
 			if(drawMenu){
 				p1_screen.setMenu(&menuPageMain, ILI_COLORS::BLACK, true);
 				drawMenu = false;
 			}
 			
 			//p1_screen.writeTextLabel(SHOW_DELTA, font_ubuntumono_16, "DELTA MENU", false);
 			
 			std::string d;
 			if(show_delta){
 				d = "yes";
 			}else{
 				d = "no";
 			}
 			
 			p1_screen.writeTextLabel(0, font_ubuntumono_16, "Show delta: " + d, false, deltaMenuSelected(SHOW_DELTA));
 			
			if(deltaMenuSelected(CHANNEL) && submenu){
				p1_screen.setInvertedBackgroundColor(ILI_COLORS::ORANGE);
			}
			p1_screen.writeTextLabel(0, font_ubuntumono_16, "Channel: " + std::to_string(dsp_delta.channel_n), true, deltaMenuSelected(CHANNEL));
			p1_screen.setInvertedBackgroundColor(ILI_COLORS::WHITE);
			 
			//Make text D1 value of delta 1
			if(deltaMenuSelected(SELECT_D1) && submenu){
				p1_screen.setInvertedBackgroundColor(ILI_COLORS::ORANGE);
			}
			p1_screen.writeTextLabel(0, font_ubuntumono_16, std::string("D1: ") + getObisTypeString(dsp_delta.delta_1), true, deltaMenuSelected(SELECT_D1));
			p1_screen.setInvertedBackgroundColor(ILI_COLORS::WHITE);
 			
			//Make text D1 value of delta 2
			if(deltaMenuSelected(SELECT_D2) && submenu){
				p1_screen.setInvertedBackgroundColor(ILI_COLORS::ORANGE);
			}
			p1_screen.writeTextLabel(0, font_ubuntumono_16, std::string("D2: ") + getObisTypeString(dsp_delta.delta_2), true, deltaMenuSelected(SELECT_D2));
 			p1_screen.setInvertedBackgroundColor(ILI_COLORS::WHITE);
			 
			 p1_screen.clearAfterWrite(0);
			 
 			p1_screen.writeTextLabel(3, font_ubuntumono_16, "EXIT", false, deltaMenuSelected(EXIT));
 			
 			if(goLeft){
 				goLeft = false;
 				
				if(!submenu){
					if(in_state > 0){
						in_state = static_cast<internalstate>(static_cast<unsigned int>(in_state) - 1 );
					}
				}
				
				if(submenu && deltaMenuSelected(SELECT_D1)){
					dsp_delta.delta_1 = getPrevType(dsp_delta.delta_1);
				}
				
				if(submenu && deltaMenuSelected(SELECT_D2)){
					dsp_delta.delta_2 = getPrevType(dsp_delta.delta_1);
				}
				
				if(submenu && deltaMenuSelected(CHANNEL)){
					dsp_delta.channel_n -= 1;
					
					if(dsp_delta.channel_n < 0){
						dsp_delta.channel_n = 9;
					}
				}
 				
 			}
 			
 			if(goRight){
 				goRight = false;
 				
				if(!submenu){
					if(in_state < 4){
						in_state = static_cast<internalstate>(static_cast<unsigned int>(in_state) + 1 );
					}
				}
				
				if(submenu && deltaMenuSelected(SELECT_D1)){
					dsp_delta.delta_1 = getNextType(dsp_delta.delta_1);
				}
 				
				 if(submenu && deltaMenuSelected(SELECT_D2)){
					 dsp_delta.delta_2 = getNextType(dsp_delta.delta_1);
				 }
				 
				 if(submenu && deltaMenuSelected(CHANNEL)){
					 dsp_delta.channel_n += 1;
					 
					 if(dsp_delta.channel_n > 9){
						 dsp_delta.channel_n = 0;
					 }
				 }
 			}
 			
 			if(goButton){
 				resetButtonBlock();
 				//goButton = false;
								
				switch(in_state){
					case SHOW_DELTA:
						show_delta = !show_delta;
						break;
					case SELECT_D1:
						submenu = !submenu;
						break;
					case SELECT_D2:
						submenu = !submenu;
						break;
					case EXIT:
						setAppState(MAIN_MENU);
						break;
					case CHANNEL:
						submenu = !submenu;
						break;
				}
 				
 			}
 			
 		}//END DELTA_MENU
		 
	}
}



SelfTestErrorCode SelfTest(ILI9341Driver & LCD, SpiDriver & LCDspi, Usart * p1Telegram){
	Helper::Debug::DebugPrint("Running self test: \r\n");
	
	//Menu
	MenuManager p1Screen(LCD);
	
	//Test pattern display
	//LCD.SetRotation(ILI_ROTATION_MODE::LANDSCAPE, false, false);
	LCD.sendTestPatternColorBlocks();
	
	//Menu
	MenuManager testMenu(LCD);
	
	testMenu.setMenu(&menuPageSelfTest, ILI_COLORS::BLACK);
	testMenu.writeTextLabel(0, font_ubuntumono_10, "Test full image in menu context", false);
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	
	//testMenu.setMenu(&fullImageTestPage, ILI_COLORS::BLACK);
	
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	
	testMenu.setMenu(&menuPageSelfTest, ILI_COLORS::BLACK);
	
	
	testMenu.writeTextLabel(0, font_ubuntumono_10, "TEST PRINT", true);
	testMenu.writeTextLabel(0, font_ubuntumono_10, "!!!!!!!!!!", true);
	testMenu.writeTextLabel(0, font_ubuntumono_10, "@@@@@@@@@@", true);
	testMenu.writeTextLabel(0, font_ubuntumono_10, "##########", true);
	testMenu.writeTextLabel(0, font_ubuntumono_10, "$$$$$$$$$$", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	testMenu.writeTextLabel(0, font_ubuntumono_10, "%%%%%%%%%%", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	testMenu.writeTextLabel(0, font_ubuntumono_10, "^^^^^^^^^^", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	testMenu.writeTextLabel(0, font_ubuntumono_10, "&&&&&&&&&&", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	testMenu.writeTextLabel(0, font_ubuntumono_10, "**********", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	testMenu.writeTextLabel(0, font_ubuntumono_10, "((((((((((", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	testMenu.writeTextLabel(0, font_ubuntumono_10, "))))))))))", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	testMenu.writeTextLabel(0, font_ubuntumono_10, "__________", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	testMenu.writeTextLabel(0, font_ubuntumono_10, "++++++++++", true);
		
	
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	testMenu.clearTextLabel(0, ILI_COLORS::BLACK);
	
	/* P1 decoder test */
	testMenu.writeTextLabel(0, font_ubuntumono_10, "P1 Decoder test:", true);
	
	P1Decoder p1test;
	
	/* Decode a correct string */
	std::string currentTestP1Telegram = testP1TelegramCapture;
	
	testMenu.writeTextLabel(0, font_ubuntumono_10, "P1 Decoder test:", true);
	int P1DecodeValue = p1test.decodeP1String(currentTestP1Telegram.c_str());
	
	testMenu.writeTextLabel(0, font_ubuntumono_10, ("Decode value : " + std::to_string(P1DecodeValue) + "expected 0"), true);
	if (P1DecodeValue != 0){
		return SelfTestErrorCode::P1DecodeError;
	}
	testMenu.writeTextLabel(0, font_ubuntumono_10, ("-Passed 1/X-"), true);

	testMenu.writeTextLabel(0, font_ubuntumono_10, ("ID : " + p1test.getDeviceIdentifier() + "expected \\2M550E-1012"), true);
	if(p1test.getDeviceIdentifier() == "\\2M550E-1012"){	
		testMenu.writeTextLabel(0, font_ubuntumono_10, ("-Passed 2/X-"), true);
	}
	
	/* Test for channel count */
	int channelCount = p1test.getCosemChannelCount();
	
	if (channelCount != 2){
		testMenu.writeTextLabel(0, font_ubuntumono_10, ("Channel count : " + std::to_string(channelCount) + "expected 2"), true);
		return SelfTestErrorCode::P1DecodeError;
	}
	
	testMenu.writeTextLabel(0, font_ubuntumono_10, ("Channel count : " + std::to_string(channelCount) + "expected 2"), true);
	testMenu.writeTextLabel(0, font_ubuntumono_10, ("-Passed 3/X-"), true);
	
	//PART TWO
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	
	testMenu.clearTextLabel(0, ILI_COLORS::BLACK);
	
	/* Test for correct amount of objects in channel */

	if(p1test.getCosemChannelSize(0) != 20){
		return SelfTestErrorCode::P1ObjectListError;
	}
	
	testMenu.writeTextLabel(0, font_ubuntumono_10, ("-Passed 4/X-"), true);
	
	if(p1test.getCosemChannelSize(1) != 3){
		return SelfTestErrorCode::P1ObjectListError;
	}
	
	testMenu.writeTextLabel(0, font_ubuntumono_10, ("-Passed 5/X-"), true);
	
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	
	testMenu.clearTextLabel(0, ILI_COLORS::BLACK);
	
	
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
	testMenu.writeTextLabel(0, font_ubuntumono_10, ("Cosem object <float> Passed 1/X-"), true);
	
	/* Test CosemObject: 2
	 * Verify delta calculation of two objects works.
	 */
	
	auto deltaString = p1test.getDeltaString(0, ObisType::PDelivered, ObisType::PReceived);
	
	//std::string deltaCalc = deltaP1->getDelta(deltaP2);
	if(deltaString != "0.240kW"){
		return SelfTestErrorCode::P1CosemObjectError;
	}
	testMenu.writeTextLabel(0, font_ubuntumono_10, ("Cosem object <float> Passed 2/X-"), true);
	
	/* Test CosemObject: 3
	 * Verify delta of two objects that can't be calculated
	 */
	
	
	return SelfTestErrorCode::P1DecodeError;
}
