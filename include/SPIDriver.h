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
#include "PINDriver.h"

class SPIDriver
{
//variables
public:
protected:
private:
	int baudrate;
	PINDriver & SPI_MISO;
	PINDriver & SPI_MOSI;
	PINDriver & SPI_SPCK;
	bool hardwareCS = false;
	bool chipSelectDecoder = false;
	Spi * SPIreg;

//functions
public:
	SPIDriver(	PINDriver & misoPin,
				PINDriver & mosiPin,
				PINDriver & spckPin,
				bool hardwareCS,
				bool chipSelectDecoder, 
				Spi * spiReg);
				
	inline void SPISend(uint8_t byte){
		SPIreg->SPI_TDR = (0xFF & byte);
		while(!(SPIreg->SPI_SR & SPI_SR_TXEMPTY)){};
	}
	
	void SPIReceive();
				
				
	~SPIDriver();
protected:
private:
	SPIDriver( const SPIDriver &c );
	SPIDriver& operator=( const SPIDriver &c );
	
	

}; //SPI_imp

#endif //__SPIDRIVER_H__
