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

#include "CosemObject.h"

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
	
	piodCalls.updateInterrupt(PIOD_IRQn, isrVal);
	
	value_ISR_PIOD = isrVal;
}

Interrupt piobCalls;

void PIOB_Handler(){
	uint32_t isrVal = PIOB->PIO_ISR;
	
	piobCalls.updateInterrupt(PIOB_IRQn, isrVal);
	
	value_ISR_PIOB = isrVal;
}

enum class SelfTestErrorCode{
	P1SelfTestPassed = 0,
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
						
	
	Helper::Time::init();
	
	/* Setup leds */
	Led powerLed(PIOD, 9, Led::LEDTYPE::OPENCOLLECTOR);
	Led heartbeat_led(PIOD, 10, Led::LEDTYPE::OPENCOLLECTOR);
	Led p1UartReceiveLed(PIOC, 2, Led::LEDTYPE::OPENCOLLECTOR);
	Led debugLED7(PIOC, 4, Led::LEDTYPE::OPENCOLLECTOR);
	
	Helper::Debug::init();
	
	/* Rotary encoder setup */
	Button rotary_button(PIOB, 27); 

	rotary_button.controllerPioEnable();
	rotary_button.setInputFilter(PIO_INPUT_FILTER::GLITCH);
	rotary_button.interruptEnable();
	rotary_button.enableAdditionalInterruptMode(PIO_LEVEL_SELECT::RISING_EDGE);
	
	piobCalls.registerInterrupt(&rotary_button);
	
	RotaryEncoder menu_encoder(	PinDriver(PIOD, 8), PIOD_IRQn, //Arduino due pin 11
								PinDriver(PIOD, 7), PIOD_IRQn);//Arduino due pin 13
	
	piodCalls.registerInterrupt(&menu_encoder);

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
	
	//SPI0
	SpiDriver lcdSpi(SPI0, false, false);
	
	//p1Lcd
	ILI9341Driver p1Lcd(320, 240, DisplayCS, DisplayDC, DisplayRESET, lcdSpi);

	/* Self test, only if button is hold down at boot */	
	if(rotary_button.getState() == PIO_PIN_STATE::HIGH){
		//should return selftest erro code enum
		SelfTestErrorCode result_ = SelfTest(p1Lcd, lcdSpi, USART3);
		
		if(result_ != SelfTestErrorCode::P1SelfTestPassed){
			Helper::Debug::DebugPrint(	"Self test detected an error: " 
										+ std::to_string(static_cast<int>(result_)));
		}else{
			Helper::Debug::DebugPrint("\r\n - Self test passed - \r\n");
		}
		
		Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	}
	
	
	//Led Flasher blocking
	for(int i = 0; i < 5; ++i){
		powerLed.toggle();
		heartbeat_led.toggle();
		p1UartReceiveLed.toggle();
		debugLED7.toggle();
		Helper::Time::delay1_5us(150 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	}

	powerLed.on();
	heartbeat_led.off();
	p1UartReceiveLed.off();
	debugLED7.off();
	
	//Menu
 	MenuManager p1_screen(p1Lcd);
 	
 	p1_screen.setMenu(&menuPageSplash, ILI_COLORS::BLACK);
 	p1_screen.writeTextLabel(0, font_ubuntumono_10, "IPASS2023 - P1Decoder", false);
	p1_screen.writeTextLabel(0, font_ubuntumono_10, "-- Jordan Ramirez --", true);
	 
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	
	//Enable all interrupts that have been set in the NVIC.
	__enable_irq();
	
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

	static bool go_left = false;
	static bool go_right = false;
	static bool go_button = false;
	static int parse_list = 0;


	menu_encoder.reset();
	rotary_button.resetButton();
	menu_encoder.reset();
	
	static bool draw_menu = true;
	
	static bool show_delta = false;
	
	static bool mm_selected = true;
	
	struct deltaObject{
		int channel_n;
		ObisType delta_1;
		ObisType delta_2;
	};
	
	deltaObject dsp_delta = {0, ObisType::PDelivered, ObisType::PReceived};
	
	struct displayObject{
		ObisType type_;
		int channel_n;
	};
	
	static std::vector<displayObject> display_list;
	
    while (1){
		
		/* Set app state and reset all state variables */
		auto setAppState = [](int n)	{
			app_state = n;
			go_left = false;
			go_right = false;
			
			//reset state for this menu
			draw_menu = true;
		};
		
		/* Reset button with block time */
		auto resetButtonBlock = [](){
			block_button = true;
			button_block_time = TC2->TC_CHANNEL[2].TC_CV;
			go_button = false;
		};
		
		/* Reset rotation every 350ms */
		if(TC2->TC_CHANNEL[2].TC_CV - blockTime >= (350 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND)){
			if(menu_encoder.getRotation()){
				int val_ = menu_encoder.getRotation();
				if(val_ < 0){
					//Left
					go_left = true;
				}else{
					//Right
					go_right = true;
				}
				
				menu_encoder.reset();
			}			
			blockTime = TC2->TC_CHANNEL[2].TC_CV;
		}
		
		/* Reset button delay 350ms */
		if(TC2->TC_CHANNEL[2].TC_CV - button_block_time >= (350 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND)){
			block_button = false;
		}
		
		/* If button has been pressed it goes into a blocked state */
		if(!block_button){
			/* When unblocked it can be set again */
			if(rotary_button.wasPressed()){
				go_button = true;
				Helper::Debug::DebugPrint("rotaryButton press detected\r\n");
				rotary_button.resetButton();
			}
		}
			
		/* Heartbeat on led */	
		if(TC2->TC_CHANNEL[2].TC_CV - startTime >= (1 * Helper::Time::TIME_UNIT_1_5US::SECOND)){
			heartbeat_led.toggle();
			startTime = TC2->TC_CHANNEL[2].TC_CV;			
		}
		
		/* Start uart msg receive */
		if((USART3->US_CSR & US_CSR_RXRDY)){ //Received a character
			USART3->US_CR |= US_CR_RETTO; //RESET TIMEOUT timer
			receive_buffer += static_cast<char>(USART3->US_RHR & 0xFF);
			//get lower 8 bit, might be implicit in cast to char
			//p1UartReceiveLed.On();
		}
		
		/* End of message */
		if(USART3->US_CSR & US_CSR_TIMEOUT){ //Timeout
			USART3->US_CR |= US_CR_STTTO;//Set timeout trigger
			
			//p1UartReceiveLed.Off();
			
			if(app_state == MAIN_MENU){
				
				//Helper::Debug::DebugPrint(receive_buffer);
				
				P1Decoder p1_msg;
				int p1_decode_value = p1_msg.decodeP1String(receive_buffer.c_str());
				
				if(draw_menu){
					p1_screen.setMenu(&menuPageMain, ILI_COLORS::BLACK, true);
					draw_menu = false;
				}
				
				if(p1_decode_value == 0){					
					
					int no_reset = false;
					
					for(displayObject to_disp: display_list){
						std::string item_ = p1_msg.getCosemStringFromChannel(to_disp.channel_n, to_disp.type_);
						p1_screen.writeTextLabel(0, font_ubuntumono_16, item_, no_reset);
						no_reset = true;
					} 
					
					if(show_delta){
						std::string deltaVal = p1_msg.getDeltaString(dsp_delta.channel_n, dsp_delta.delta_1, dsp_delta.delta_2);
						p1_screen.writeTextLabel(0, font_ubuntumono_16, std::string("DeltaV: ") + deltaVal, no_reset);
						no_reset = true;	
					}
					
					p1_screen.writeTextLabel(1, font_ubuntumono_16, "MENU", false, mm_selected);

					p1_screen.writeTextLabel(3, font_ubuntumono_16, "D-MENU", false, !mm_selected);
					
					
					/* If no items are displayed on the screen, send this msg */
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
			
			if(go_left){
				go_left = false;
				
				mm_selected = !mm_selected; //Swap selected menu in main menu
			}
			
			if(go_right){
				go_right = false;
				
				mm_selected = !mm_selected; //Swap selected menu in main menu
			}
			
			if(go_button){
				resetButtonBlock();
				
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
						
			if(draw_menu){
				p1_screen.setMenu(&menuPageMain, ILI_COLORS::BLACK, true);
				draw_menu = false;
				
				parse_list = 0;
				submenu_ = false;
			}
			
			std::string listed_string = "ADD ITEM";
			
			for(displayObject &ref: display_list){
				Obis::getObisTypeString(static_cast<ObisType>(parse_list));
				std::cout << ref.channel_n << std::endl;
				if(	ref.type_ == static_cast<ObisType>(parse_list) &&
					ref.channel_n == channel_){
						listed_string = "REMOVE THIS";
						break;
					}
			}
			
			p1_screen.writeTextLabel(0, font_ubuntumono_16, listed_string, false);
			
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
			
			if(go_left){
				go_left = false;
				
				if(!submenu_){ //Switch between the different submenus
					if(!(selected_menu <= 0)){
						selected_menu -= 1;
					}else{
						selected_menu = 0;
					}
				}
				
				if(submenu_ && selected_menu == 0){ //If in submenu , this rotation affects the submenu
					parse_list -= 1;
					
					if(parse_list < 0){
						parse_list = 0;
					}
				}
				
				if(submenu_ && selected_menu == 1){
					channel_ -= 1;
					
					if(channel_ < 0){
						channel_ = 9;
					}
				}
							
				
				
			}else if(go_right){
				go_right = false;
				
				if(!submenu_){
					if(!(selected_menu >= 3)){
						selected_menu += 1;
						}else{
						selected_menu = 3;
					}
				}
				
				if(submenu_ && selected_menu == 0){
					parse_list += 1;
					
					int c = static_cast<int>(ObisType::COUNT);
					if(parse_list >= c){
						parse_list = c -1;
					}
				}		
				
				if(submenu_ && selected_menu == 1){
					channel_ += 1;
					
					if(channel_ > 9){
						channel_ = 0;
					}
				}
								
				
			}
			
			p1_screen.writeTextLabel(0, font_ubuntumono_16, Obis::getObisTypeString(static_cast<ObisType>(parse_list)), true);
			p1_screen.clearAfterWrite(0);
			
			if(go_button){
				resetButtonBlock();
				
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
					//Helper::Debug::DebugPrint("ADD OR REMOVE OBISString\r\n");
					//Remove obis object
					std::vector<displayObject>::iterator it = display_list.begin();
					
					bool removed_item = false;
					
					while(it != display_list.end()){
						if( it->type_ == static_cast<ObisType>(parse_list) &&
							it->channel_n == channel_){
							it = display_list.erase(it);
							removed_item = true;
						}else{
							++it;
						}
					}										if(!removed_item){
						//ADD
						display_list.push_back({static_cast<ObisType>(parse_list), channel_});
					}

				}
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
			
			static bool delta_submenu = false;
			//static int channel_number = 0;
 			
 			auto deltaMenuSelected = [](internalstate n)	{
 				if(in_state == n){
 					return true;
 				}
 				return false;
 			};
 			
 			if(draw_menu){
 				p1_screen.setMenu(&menuPageMain, ILI_COLORS::BLACK, true);
 				draw_menu = false;
 			}
 			
 			std::string d;
 			if(show_delta){
 				d = "yes";
 			}else{
 				d = "no";
 			}
 			
 			p1_screen.writeTextLabel(0, font_ubuntumono_16, "Show delta: " + d, false, deltaMenuSelected(SHOW_DELTA));
 			
			if(deltaMenuSelected(CHANNEL) && delta_submenu){
				p1_screen.setInvertedBackgroundColor(ILI_COLORS::ORANGE);
			}
			p1_screen.writeTextLabel(0, font_ubuntumono_16, "Channel: " + std::to_string(dsp_delta.channel_n), true, deltaMenuSelected(CHANNEL));
			p1_screen.setInvertedBackgroundColor(ILI_COLORS::WHITE);
			 
			//Make text D1 value of delta 1
			if(deltaMenuSelected(SELECT_D1) && delta_submenu){
				p1_screen.setInvertedBackgroundColor(ILI_COLORS::ORANGE);
			}
			p1_screen.writeTextLabel(0, font_ubuntumono_16, std::string("D1: ") + Obis::getObisTypeString(dsp_delta.delta_1), true, deltaMenuSelected(SELECT_D1));
			p1_screen.setInvertedBackgroundColor(ILI_COLORS::WHITE);
 			
			//Make text D1 value of delta 2
			if(deltaMenuSelected(SELECT_D2) && delta_submenu){
				p1_screen.setInvertedBackgroundColor(ILI_COLORS::ORANGE);
			}
			p1_screen.writeTextLabel(0, font_ubuntumono_16, std::string("D2: ") + Obis::getObisTypeString(dsp_delta.delta_2), true, deltaMenuSelected(SELECT_D2));
 			p1_screen.setInvertedBackgroundColor(ILI_COLORS::WHITE);
			 
			 p1_screen.clearAfterWrite(0);
			 
 			p1_screen.writeTextLabel(3, font_ubuntumono_16, "EXIT", false, deltaMenuSelected(EXIT));
 			
 			if(go_left){
 				go_left = false;
 				
				if(!delta_submenu){
					if(in_state > 0){
						in_state = static_cast<internalstate>(static_cast<unsigned int>(in_state) - 1 );
					}
				}
				
				if(delta_submenu && deltaMenuSelected(SELECT_D1)){
					dsp_delta.delta_1 = Obis::getPrevType(dsp_delta.delta_1);
				}
				
				if(delta_submenu && deltaMenuSelected(SELECT_D2)){
					dsp_delta.delta_2 = Obis::getPrevType(dsp_delta.delta_1);
				}
				
				if(delta_submenu && deltaMenuSelected(CHANNEL)){
					dsp_delta.channel_n -= 1;
					
					if(dsp_delta.channel_n < 0){
						dsp_delta.channel_n = 9;
					}
				}
 				
 			}
 			
 			if(go_right){
 				go_right = false;
 				
				if(!delta_submenu){
					if(in_state < 4){
						in_state = static_cast<internalstate>(static_cast<unsigned int>(in_state) + 1 );
					}
				}
				
				if(delta_submenu && deltaMenuSelected(SELECT_D1)){
					dsp_delta.delta_1 = Obis::getNextType(dsp_delta.delta_1);
				}
 				
				 if(delta_submenu && deltaMenuSelected(SELECT_D2)){
					 dsp_delta.delta_2 = Obis::getNextType(dsp_delta.delta_1);
				 }
				 
				 if(delta_submenu && deltaMenuSelected(CHANNEL)){
					 dsp_delta.channel_n += 1;
					 
					 if(dsp_delta.channel_n > 9){
						 dsp_delta.channel_n = 0;
					 }
				 }
 			}
 			
 			if(go_button){
 				resetButtonBlock();
								
				switch(in_state){
					case SHOW_DELTA:
						show_delta = !show_delta;
						break;
					case SELECT_D1:
						delta_submenu = !delta_submenu;
						break;
					case SELECT_D2:
						delta_submenu = !delta_submenu;
						break;
					case EXIT:
						setAppState(MAIN_MENU);
						break;
					case CHANNEL:
						delta_submenu = !delta_submenu;
						break;
				}
 				
 			}
 		}//END DELTA_MENU

	}
}



SelfTestErrorCode SelfTest(ILI9341Driver & lcd_, SpiDriver & LCDspi, Usart * p1Telegram){
	Helper::Debug::DebugPrint("Running self test: \r\n");
	
	//Menu
	MenuManager p1_screen(lcd_);
	
	//Test pattern display
	lcd_.sendTestPatternColorBlocks();
	
	//Menu
	MenuManager test_menu(lcd_);
	
	test_menu.setMenu(&menuPageSelfTest, ILI_COLORS::BLACK);
	test_menu.writeTextLabel(0, font_ubuntumono_10, "Test menu switching ... >", false);
	//test_menu.writeTextLabel(0, font_ubuntumono_10, "Test full image in menu context", false);
	//Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	
	//test_menu.setMenu(&fullImageTestPage, ILI_COLORS::BLACK);
	
	//Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	//Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	
	test_menu.setMenu(&menuPageSelfTest, ILI_COLORS::BLACK);
	
	
	test_menu.writeTextLabel(0, font_ubuntumono_10, "TEST PRINT", false);
	test_menu.writeTextLabel(0, font_ubuntumono_10, "!!!!!!!!!!", true);
	test_menu.writeTextLabel(0, font_ubuntumono_10, "@@@@@@@@@@", true);
	test_menu.writeTextLabel(0, font_ubuntumono_10, "##########", true);
	test_menu.writeTextLabel(0, font_ubuntumono_10, "$$$$$$$$$$", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	test_menu.writeTextLabel(0, font_ubuntumono_10, "%%%%%%%%%%", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	test_menu.writeTextLabel(0, font_ubuntumono_10, "^^^^^^^^^^", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	test_menu.writeTextLabel(0, font_ubuntumono_10, "&&&&&&&&&&", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	test_menu.writeTextLabel(0, font_ubuntumono_10, "**********", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	test_menu.writeTextLabel(0, font_ubuntumono_10, "((((((((((", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	test_menu.writeTextLabel(0, font_ubuntumono_10, "))))))))))", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	test_menu.writeTextLabel(0, font_ubuntumono_10, "__________", true);
	Helper::Time::delay1_5us(500 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	test_menu.writeTextLabel(0, font_ubuntumono_10, "++++++++++", true);
		
	
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	test_menu.clearTextLabel(0, ILI_COLORS::BLACK);
	
	/* P1 decoder test */
	test_menu.writeTextLabel(0, font_ubuntumono_10, "P1 Decoder test:", true);
	
	P1Decoder p1_test;
	
	/* Decode a correct string */
	std::string current_test_p1_telegram = testP1TelegramCapture;
	
	test_menu.writeTextLabel(0, font_ubuntumono_10, "P1 Decoder test:", true);
	int p1_decode_value = p1_test.decodeP1String(current_test_p1_telegram.c_str());
	
	test_menu.writeTextLabel(0, font_ubuntumono_10, ("Decode value : " + std::to_string(p1_decode_value) + "expected 0"), true);
	if (p1_decode_value != 0){
		return SelfTestErrorCode::P1DecodeError;
	}
	test_menu.writeTextLabel(0, font_ubuntumono_10, ("-Passed 1/5-"), true);

	test_menu.writeTextLabel(0, font_ubuntumono_10, ("ID : " + p1_test.getDeviceIdentifier() + "expected \\2M550E-1012"), true);
	if(p1_test.getDeviceIdentifier() == "\\2M550E-1012"){	
		test_menu.writeTextLabel(0, font_ubuntumono_10, ("-Passed 2/5-"), true);
	}
	
	/* Test for channel count */
	int channel_count = p1_test.getCosemChannelCount();
	
	if (channel_count != 2){
		test_menu.writeTextLabel(0, font_ubuntumono_10, ("Channel count : " + std::to_string(channel_count) + "expected 2"), true);
		return SelfTestErrorCode::P1DecodeError;
	}
	
	test_menu.writeTextLabel(0, font_ubuntumono_10, ("Channel count : " + std::to_string(channel_count) + "expected 2"), true);
	test_menu.writeTextLabel(0, font_ubuntumono_10, ("-Passed 3/5-"), true);
	
	//PART TWO
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	
	test_menu.clearTextLabel(0, ILI_COLORS::BLACK);
	
	/* Test for correct amount of objects in channel */

	if(p1_test.getCosemChannelSize(0) != 20){
		return SelfTestErrorCode::P1ObjectListError;
	}
	
	test_menu.writeTextLabel(0, font_ubuntumono_10, ("-Passed 4/5-"), true);
	
	/* Test for correct amount of objects in channel and multiple channels work*/
	
	if(p1_test.getCosemChannelSize(1) != 3){
		return SelfTestErrorCode::P1ObjectListError;
	}
	
	test_menu.writeTextLabel(0, font_ubuntumono_10, ("-Passed 5/5-"), true);
	
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	
	test_menu.clearTextLabel(0, ILI_COLORS::BLACK);
	
	
	/* Testing Cosem objects in channels*/
	
	/* Test CosemObject: 1
	 * Verify if values in deltaP1 & deltaP2 are correct.
	 */
	
	auto delta_p1_string = p1_test.getCosemStringFromChannel(0, ObisType::PDelivered);
	auto deltaP2String = p1_test.getCosemStringFromChannel(0, ObisType::PReceived);
	
	if(delta_p1_string != "Power delivered (+P): 0.368kW"){
		return SelfTestErrorCode::P1CosemObjectError;
	}
	if(deltaP2String != "Power received (-P): 0.128kW"){
		return SelfTestErrorCode::P1CosemObjectError;
	}
	test_menu.writeTextLabel(0, font_ubuntumono_10, ("Cosem object <float> Passed 1/3-"), true);
	
	/* Test CosemObject: 2
	 * Verify delta calculation of two objects works.
	 */
	
	auto delta_string = p1_test.getDeltaString(0, ObisType::PDelivered, ObisType::PReceived);
	
	//std::string deltaCalc = deltaP1->getDelta(deltaP2);
	if(delta_string != "0.240kW"){
		return SelfTestErrorCode::P1CosemObjectError;
	}
	test_menu.writeTextLabel(0, font_ubuntumono_10, ("Cosem object <float> Passed 2/3-"), true);
	
	/* Test CosemObject: 3
	 * Verify delta of two objects that can't be calculated
	 */
	auto delta_not_string = p1_test.getDeltaString(0, ObisType::Version, ObisType::PReceived);
	if(delta_not_string != "NaN"){
		return SelfTestErrorCode::P1CosemObjectError;
	}
	test_menu.writeTextLabel(0, font_ubuntumono_10, ("Cosem object <float> Passed 3/3-"), true);
	
	
	test_menu.writeTextLabel(0, font_ubuntumono_10, ("Self Test passed."), true);
	
	return SelfTestErrorCode::P1SelfTestPassed;
}
