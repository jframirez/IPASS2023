/* ILI9341Driver.h
* Definitions for ILI9341 Driver
* Created: 10-5-2023 20:11:57
* Author: Jordan
*/

/** \file ILI9341Driver.h
 * ILI Driver definitions.
 */


#ifndef __ILI9341DRIVER_H__
#define __ILI9341DRIVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "ILI9341Commands.h"
#include "Helper.h"
#include "PinDriver.h"
#include "SpiDriver.h"

//Always in 4-wire 8-bit data serial interface

/**
 * enum to hold sleep mode.
 */
enum class ILI_SLEEP_MODE{
	SLEEP_OUT = 0,	///< Display is out of sleep mode.
	SLEEP_IN = 1	///< Display is in sleep mode.
};

/**
 * enum to hold rotation mode.
 */
enum class ILI_ROTATION_MODE{
	LANDSCAPE = 0,			/**< Rotation is in landscape. */
	PORTRAIT = 1,			/**< Rotation is in portrait. */
	LANDSCAPE_INVERTED = 2,	/**< Rotation is in landscape (inverted). */
	PORTRAIT_INVERTED = 3	/**< Rotation is in portrait (inverted). */
};

/**
 * enum to hold colors that can be quickly referenced.
 */
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

/**
 * Operator to shift colors to the right hand side.
 */
uint16_t operator>>(ILI_COLORS lhs, uint shift);

/**
 * Operator to combine two color values.
 */
uint16_t operator&(ILI_COLORS lhs, uint rhs);


/**
 * Driver for a display using the ILI9341 Driver.
 */
class ILI9341Driver{

public:
	
	/**
	 * Constructor for the ILI9341 Driver..
	 *
	 * \param width width in pixels of the display connected to ILI9341.
	 * \param height height in pixels of the display connected to ILI9341.
	 * \param chipselect pin for chipselect
	 * \param dc pin for data or command selection
	 * \param reset pin connected to the hardware reset
	 * \param spi SPI peripheral
	 */
	ILI9341Driver(	uint width,
					uint height,
					PinDriver & chipselect, 
					PinDriver & dc, 
					PinDriver & reset,
					SpiDriver & spi);

	/**
	 * Reset ILI9341 with a hardware reset.
	 */
	void hardwareReset();
	
	/**
	 * Perform a software reset of the ILI9341.
	 */
	void softwareReset();
	
	/**
	 * Set display mode of the display.
	 *
	 * \param mode display sleep mode.
	 */
	void setSleepMode(ILI_SLEEP_MODE mode);
	
	/**
	 * Set display rotation.
	 *
	 * Change the order the data memory is written to the display.
	 * set cont to true to use after a Continues (Cont) command.
	 * Can also reset Column and Page counter to the current height and width
	 *  setting setCASETPASET to true.
	 *
	 * \param mode display rotation mode..
	 * \param cont a bool to set continues mode.
	 * \param setCASETPASET a bool to set Column and Page to current width and height
	 */
	void setRotation(ILI_ROTATION_MODE mode, bool cont, bool setCASETPASET);
	
	/**
	 * Send a command.
	 *
	 * \param byte a uint8_t to send 8 bits of command data to the display.
	 */
	void sendCommand(uint8_t byte);
	
	/**
	 * Send a command with parameters.
	 *
	 * Send a command with n parameters to the ILI9341.
	 *
	 * \param command an uint8_t that will be send as command.
	 * \param parameterN an integer that holds the number of parameters in the ... .
	 * \param ... variadic argument list, will be interpreted as uint8_t.
	 */
	void sendCommandWithParamter(uint8_t command, int parameterN, ...);
	
	/**
	 * Send the start of a continues stream of data to ILI9341.
	 */
	void sendStartCont();
	
	/**
	 * Send the end of a continues stream of data to ILI9341.
	 */
	void sendEndCont();
	
	/**
	 * Send a command during a continues stream of data.
	 *
	 * \param byte an uint8_t that will be send as command.
	 */
	void sendCommandCont(uint8_t byte);
	
	/**
	 * Send data (1 byte) during a continues stream of data.
	 *
	 * \param byte a uint8_t to send as data.
	 */
	void sendDataCont(uint8_t byte);
	
	/**
	 * Send data (2 bytes) during a continues stream of data.
	 *
	 * \param twoBytes a uint16_t that will be send as data.
	 */
	inline void sendDataCont16_t(uint16_t twoBytes){
		sendDataCont(twoBytes >> 8);
		sendDataCont(twoBytes & 0xFF);
	}
	
	/**
	 * Send a ILI_COLORS during a continues stream of data.
	 *
	 * \param color an ILI_COLORS to be send as data.
	 */
	inline void sendDataCont16_t(ILI_COLORS color){
		sendDataCont16_t((uint16_t)color);
	}
	
	
	//Make generic and accept enum testPattern
	/**
	 * Display various test patterns on the connected display.
	 */
	void sendTestPatternColorBlocks();
	
	/**
	 * Set the width of the connected display.
	 *
	 * \param width an uint to set width of display in pixels.
	 */
	void setWidth(uint width);
	
	/**
	 * Set the height of the connected display.
	 *
	 * \param height an uint to set height of display in pixels.
	 */
	void setHeight(uint height);
	
	/**
	 * Returns max_pixel_count in display
	 * \return uint value of max_pixel_count
	 */
	const uint getMaxPixelCount();
	
	/**
	 * Returns col_start
	 * \return uint value of col_start
	 */
	uint getColStart();
	
	/**
	 * Returns col_end
	 * \return uint value of col_end
	 */
	uint getColEnd();
	
	/**
	 * Returns col_lenght
	 * \return uint value of col_lenght
	 */
	uint getColLenght();
	
	/**
	 * Returns row_start
	 * \return uint value of row_start
	 */
	uint getRowStart();
	
	/**
	 * Returns row_end
	 * \return uint value of row_end
	 */
	uint getRowEnd();
	
	/**
	 * Returns row_lenght
	 * \return uint value of row_lenght
	 */
	uint getRowLenght();
	
	//////////////////////////////////////////////////////////////////////////
	
	/**
	 * Set col_start
	 * \param val uint value to set col_start
	 */
	void setColStart(uint val);
	
	/**
	 * Set col_end
	 * \param val uint value to set col_end
	 */
	void setColEnd(uint val);
	
	/**
	 * Set col_lenght
	 * \param val uint value to set col_lenght
	 */
	void setColLenght(uint val);
	
	/**
	 * Set row_start
	 * \param val uint value to set row_start
	 */
	void setRowStart(uint val);
	
	/**
	 * Set row_end
	 * \param val uint value to set row_end
	 */
	void setRowEnd(uint val);
	
	/**
	 * Set row_lenght
	 * \param val uint value to set row_lenght
	 */
	void setRowLenght(uint val);
	
	/**
	 * Default destructor.
	 */
	~ILI9341Driver();

private:
	
	uint width = ILI9341_WIDTH_X;
	uint height = ILI9341_HEIGHT_Y;
	const uint max_pixel_count = ILI9341_WIDTH_X * ILI9341_HEIGHT_Y;
	
	uint colStart = 0;
	uint colEnd = 319;
	uint colLenght = 319;
	uint rowStart = 0;
	uint rowEnd = 239;
	uint rowLenght = 239;

	ILI9341Driver( const ILI9341Driver &c );
	ILI9341Driver& operator=( const ILI9341Driver &c );
	
	//PINS
	PinDriver & displayCS;
	PinDriver & displayDC;
	PinDriver & displayRESET;
	SpiDriver & ili_spi;
	
	ILI_SLEEP_MODE currentSleepMode = ILI_SLEEP_MODE::SLEEP_IN;
	
	/**
	 * Chip select enable.
	 */
	inline void CS_Enable(){
		displayCS.setOutput(PIO_PIN_STATE::LOW);
	}
	
	/**
	 * Chip select disable.
	 */
	inline void CS_Disable(){
		displayCS.setOutput(PIO_PIN_STATE::HIGH);
	}
	
	/**
	 * Set Data/Command line to data.
	 */
	inline void DC_SetData(){
		displayDC.setOutput(PIO_PIN_STATE::HIGH);
	}
	
	/**
	 * Set Data/Command line to command.
	 */
	inline void DC_SetCommand(){
		displayDC.setOutput(PIO_PIN_STATE::LOW);
	}

}; //ILI9341Driver

#endif //__ILI9341DRIVER_H__
