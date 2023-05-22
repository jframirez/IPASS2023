/*
 * P1Meter.cpp
 *
 * Created: 29-4-2023 21:07:28
 * Author : Jordan
 */ 
#include "sam.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "component/pio.h"

#include "SPIDriver.h"
#include "PINDriver.h"
#include "ILI9341Driver.h"

#include "LED.h"
#include "Helper.h"

#include "Font/font-ubuntumono-10.h"
#include "Font/font-ubuntumono-16.h"
#include "Font/font-ubuntumono-22.h"
#include "Font/font-ubuntumono-28.h"
#include "Font/font-ubuntumono-34.h"
#include "Font/font-ubuntumono-40.h"

#include "Img/TestJPG.h"


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

	//unsigned int delay1S = 656254;
	//Helper::delay1_5us(delay1S);
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
	
	
	
	//UART->UART_THR = 'a';
	
	
	Helper::Debug::DebugPrint("TEST PRINT\r\n");
	
	//SPI0
	SPIDriver LCDSpi(SPI0_MISO, SPI0_MOSI, SPI0_SPCK, false, false, SPI0);
	
	//
	ILI9341Driver LCD(DisplaySS, DisplayDC, DisplayRESET, LCDSpi);
	
	//Fill LCD black
	LCD.SetRotation(ILI_ROTATION_MODE::LANDSCAPE, false, true);
	LCD.SendStartCont();
	LCD.SendCommandCont(ILI9341CMD_Memory_Write);
	for (uint i = 0; i < LCD.max_pixel_count; ++i){
		LCD.SendDataCont16_t(ILI_COLORS::BLACK);
	}
	LCD.SendEndCont();
	
	//Fill from bitmapArray
	LCD.SendStartCont();
	LCD.SendCommandCont(ILI9341CMD_Memory_Write);
	for (int i = 0; i < TestJPGLenght; ++i){
		LCD.SendDataCont16_t(TestJPG[i]);
		//16 pixels per
		/* This can be used for compressed format whit predefined colors
		for(uint j = 8; j > 0 ; --j){ // High bit -> Low bit
			if(bitmapArray[i] & (1 << (j -1))){ 
				LCD.SendDataCont16_t(ILI_COLORS::WHITE);
			}else{
				LCD.SendDataCont16_t(ILI_COLORS::BLACK);
			}
		}*/
	}
	LCD.SendEndCont();
	
	//LCD.SetRotation(ILI_ROTATION_MODE::PORTRAIT, false, true);
	LCD.SendStartCont();
	
	//set caset
	LCD.SendCommandCont(ILI9341CMD_Column_Address_Set);
	//start is 50 and end is width - 50;
	LCD.SendDataCont16_t(50); //start
	LCD.SendDataCont16_t((LCD.width - 50) -1); //end
	
	//set paset
	LCD.SendCommandCont(ILI9341CMD_Page_Address_Set);
	//set paset to start 120
	// end is max
	LCD.SendDataCont16_t(120); //start
	LCD.SendDataCont16_t(LCD.height -1); //end
	
	LCD.SendCommandCont(ILI9341CMD_Memory_Write);
	
	LCD.SendEndCont();
	
	LCD.colStart = 50;
	LCD.colEnd = LCD.width - 50;
	LCD.colLenght = LCD.colEnd - LCD.colStart;
	
	LCD.rowStart = 120;
	LCD.rowEnd = LCD.height;
	LCD.rowLenght = LCD.rowEnd - LCD.rowStart;

	//char buffer[256]; -> can be used in the future
	//uint offset_width = 0;
	uint offset_height = 0;
	
	uint fullHeightOffset = LCD.rowStart;
	uint8_t currentGlyph;
	uint bufferPrintOffset = 0;
	uint glyphCounter = 0;
	
	ILI_COLORS backgroundFill = ILI_COLORS::BLACK;

	const char testLine[] = "JFR P1 Meter  - V1.0 -\nIPASS 2023";
	uint lineLenghtGlyphs = sizeof(testLine) / sizeof(char);
	
	font curFont = font_ubuntumono_22;
	uint curResize = curFont.height - curFont.size; // 16
	
	LCD.SendStartCont();
	LCD.SendCommandCont(ILI9341CMD_Memory_Write);
	
	do{
		offset_height = 0;
		for (int i = 0; i < curFont.height; ++i){
			
			glyphCounter = 0;
			currentGlyph = testLine[glyphCounter + bufferPrintOffset]; ///initial buffer pos for line.

			uint line_lenght = 0;
			//for all glyphs
			while(line_lenght <= LCD.colLenght){	
			
				if((currentGlyph == '\0') | (currentGlyph == '\n')){ //special chars, these chars do not get printed, \r could be supported but would need to reset all offsets
					//fill line
					for(uint k = 0; k < (LCD.colLenght - line_lenght); ++k){
						LCD.SendDataCont16_t(backgroundFill); //BACKGROUND FILL
					}
					glyphCounter += 1;
					break;
				}else{
					currentGlyph = currentGlyph - ' ';
					line_lenght += curFont.glyphs[currentGlyph]->advance;
				}
				//offset_width += font_UbuntuMonoB_10.glyphs[currentGlyph]->cols;	
				if(line_lenght <= (LCD.colLenght - 1 )){ //should be <= but for testing
					int col_offset = curFont.glyphs[currentGlyph]->cols;
					
					//print left blanking .left 
					for(int leftC = 0; leftC < curFont.glyphs[currentGlyph]->left; ++leftC){
						//line_lenght += 1;
						LCD.SendDataCont16_t(backgroundFill);
					}
						
					for (int j = 0; j < col_offset; ++j){
						int lineCheckBackFill = (curFont.height - offset_height) - (curFont.glyphs[currentGlyph]->top + curResize);
						int lineCheckPrint = offset_height - (curFont.height - (curFont.glyphs[currentGlyph]->top + curResize));
						if(lineCheckBackFill > 0){
							LCD.SendDataCont16_t(backgroundFill); //BACKGROUND FILL
						}else if (lineCheckPrint < curFont.glyphs[currentGlyph]->rows){
							uint8_t grayscale = curFont.glyphs[currentGlyph]->bitmap[j + (lineCheckPrint * curFont.glyphs[currentGlyph]->cols)];
							// divide grayscale glyph by R >> 3, G >> 2, B >> 3
							uint16_t RGB = ( (0xF800 & ((grayscale >> 3) << 11)) | (0x07E0 & ((grayscale >> 2) << 5)) | (0x001F & (grayscale >> 3)) );
							LCD.SendDataCont16_t(RGB);
							//0xF800 - R mask, 0x07E0 - G mask, 0x001F - B mask
						}else{
							LCD.SendDataCont16_t(backgroundFill); //BACKGROUND FILL
						}
					}				
					//print right blanking .advance - (.left + .col)
					for(int leftC = 0; leftC < (curFont.glyphs[currentGlyph]->advance - (curFont.glyphs[currentGlyph]->left + curFont.glyphs[currentGlyph]->cols)); ++leftC){
						LCD.SendDataCont16_t(backgroundFill);
					}
				}else{
					//send empty to fill line width or fill empty space in assigned field
					for(uint k = 0; k < (LCD.colLenght - (line_lenght - (curFont.glyphs[currentGlyph]->advance)) ); ++k){
						LCD.SendDataCont16_t(backgroundFill); //BACKGROUND FILL
					}
					break;
				}
			
				glyphCounter += 1;
				currentGlyph = testLine[glyphCounter + bufferPrintOffset];
			}
			offset_height += 1;
			fullHeightOffset += 1;
			if(fullHeightOffset >= LCD.rowEnd){
				break; //Break immediate on end of buffer
			}
		}
		bufferPrintOffset += glyphCounter;
		if(fullHeightOffset >= LCD.rowEnd){
			break; //If buffer is on last row break
		}
	} while (bufferPrintOffset < lineLenghtGlyphs); // 57 < 78
	
	LCD.SendEndCont();

	
	
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::SECOND);
	
    while (1){
				
		if(TC2->TC_CHANNEL[2].TC_CV - startTime >= (1 * Helper::Time::TIME_UNIT_1_5US::SECOND)){
			if(LEDState){
				debugLED1.OFF();
				LEDState = false;
				
				LCD.SetRotation(ILI_ROTATION_MODE::LANDSCAPE, false, true);
				LCD.SendStartCont();
				LCD.SendCommandCont(ILI9341CMD_Memory_Write);
				for (uint i = 0; i < LCD.max_pixel_count; ++i){
					LCD.SendDataCont16_t(ILI_COLORS::RED);
				}
				LCD.SendEndCont();
				
			}else{
				debugLED1.ON();
				LEDState = true;
				
				LCD.SetRotation(ILI_ROTATION_MODE::PORTRAIT, false, true);
				LCD.SendStartCont();
				LCD.SendCommandCont(ILI9341CMD_Memory_Write);
				for (uint i = 0; i < LCD.max_pixel_count; ++i){
					LCD.SendDataCont16_t(ILI_COLORS::BLUE);
				}
				LCD.SendEndCont();
			}
			debugLED2.ON();
			startTime = TC2->TC_CHANNEL[2].TC_CV;
			
			//Test uart
			Helper::Debug::DebugPrint("TEST PRINT\r\n");
		}	
	}
}
