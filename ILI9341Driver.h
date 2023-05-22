/* 
* ILI9341Driver.h
*
* Created: 10-5-2023 20:11:57
* Author: Jordan
*/


#ifndef __ILI9341DRIVER_H__
#define __ILI9341DRIVER_H__

#include "ILI9341Commands.h"

#include "Helper.h"

#include "Font/font-ubuntumono-10.h"

#include <stdio.h>
#include <stdlib.h>

#include "PINDriver.h"
#include "SPIDriver.h"
//Always in 4-wire 8-bit data serial interface
//Define memory definitions
//typedef enum ILI9341Commands{
	
//	};

//#define SPI_CSR_SCBR(value) ((SPI_CSR_SCBR_Msk & ((value) << SPI_CSR_SCBR_Pos)))

enum class ILI_SLEEP_MODE{
	SLEEP_OUT = 0,
	SLEEP_IN = 1
};

enum class ILI_ROTATION_MODE{
	LANDSCAPE = 0,
	PORTRAIT = 1,
	LANDSCAPE_INVERTED = 2,
	PORTRAIT_INVERTED = 3
};

enum class ILI_COLORS{
	GREEN = 0x07E0,
	RED = 0xF800,
	BLUE = 0x001F,
	WHITE = 0xFFFF,
	BLACK = 0x0000,
	ORANGE = 0xfc20,
	PURPLE = 0xc1f5,
	YELLOW = 0xe704,
};

uint16_t operator>>(ILI_COLORS lhs, uint shift);
uint16_t operator&(ILI_COLORS lhs, uint rhs);



class ILI9341Driver{

public:
	ILI9341Driver(	PINDriver & chipselect, 
					PINDriver & dc, 
					PINDriver & reset,
					SPIDriver & spi);

	void HardwareReset();
	void SoftwareReset();
	
	void SetSleepMode(ILI_SLEEP_MODE);
	
	void SetRotation(ILI_ROTATION_MODE, bool Cont, bool SetCASETPASET);
	
	void SendCommand(uint8_t byte);
	void SendCommandWithParamter(uint8_t command, int parameterN, ...);
	
	void SendStartCont();
	void SendEndCont();
	void SendCommandCont(uint8_t byte);
	void SendDataCont(uint8_t byte);
	inline void SendDataCont16_t(uint16_t twoBytes){
		SendDataCont(twoBytes >> 8);
		SendDataCont(twoBytes & 0xFF);
	}
	inline void SendDataCont16_t(ILI_COLORS twoBytes){
		SendDataCont16_t((uint16_t)twoBytes);
	}
	
	inline void CS_Enable(){
		displaySS.SetOutput(PIO_OUTPUT::LOW);
	}
	
	inline void CS_Disable(){
		displaySS.SetOutput(PIO_OUTPUT::HIGH);
	}
	
	inline void DC_SetData(){
		displayDC.SetOutput(PIO_OUTPUT::HIGH);
	}
	
	inline void DC_SetCommand(){
		displayDC.SetOutput(PIO_OUTPUT::LOW);
	}
	
	~ILI9341Driver();
	
	uint width = 320;
	uint height = 240;
	uint max_pixel_count = width * height;
	
	uint colStart = 0;
	uint colEnd = 319;
	uint colLenght = 319;
	uint rowStart = 0;
	uint rowEnd = 239;
	uint rowLenght = 239;
	
private:
	ILI9341Driver( const ILI9341Driver &c );
	ILI9341Driver& operator=( const ILI9341Driver &c );
	
	//PINS
	PINDriver & displaySS;
	PINDriver & displayDC;
	PINDriver & displayRESET;
	//Peri SPI
	SPIDriver & mySPI;
	
	ILI_SLEEP_MODE currentSleepMode = ILI_SLEEP_MODE::SLEEP_IN;

}; //ILI9341Driver

#endif //__ILI9341DRIVER_H__
