/* 
* SPI_imp.h
*
* Created: 30-4-2023 04:32:39
* Author: Jordan
*/


#ifndef __SPIDRIVER_H__
#define __SPIDRIVER_H__

#include "ILI9341Commands.h"
#include "sam.h"
#include "PinDriver.h"

/**
 * Driver for a SPI bus.
 */
class SpiDriver{

	private:
	
		int baudrate;
		bool hardwareCS = false;
		bool chipSelectDecoder = false;
		Spi * SPIreg;

	public:
		
		/**
		 * Driver to control a SPI bus.
		 *
		 * \param spiReg an integer argument.
		 * \param hardwareCS is using hardware chip select.
		 * \param chipSelectDecoder is using built in select decoder.
		 */
		SpiDriver(	Spi * spiReg,
					bool hardwareCS,
					bool chipSelectDecoder);
		
		/**
		 * Inline function to send a uint8_t over the SPI bus.
		 *
		 * \param byte to send.
		 */
		inline void SpiSend(uint8_t byte){
			SPIreg->SPI_TDR = (0xFF & byte);
			while(!(SPIreg->SPI_SR & SPI_SR_TXEMPTY)){};
		}
	
		//void SPIReceive();
				
				
		~SpiDriver();

	private:
		SpiDriver( const SpiDriver &c );
		SpiDriver& operator=( const SpiDriver &c );
	
	

};

#endif //__SPIDRIVER_H__
