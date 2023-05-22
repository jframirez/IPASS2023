/* 
* SPIDriver.cpp
*
* Created: 30-4-2023 04:32:39
* Author: Jordan
*/


#include "SPIDriver.h"
#include "sam3xa.h"

// default constructor
SPIDriver::SPIDriver(	PINDriver & misoPin,
						PINDriver & mosiPin,
						PINDriver & spckPin,
						bool spiCS,
						bool csDecoder,
						Spi * spiReg):
						SPI_MISO(misoPin),
						SPI_MOSI(mosiPin),
						SPI_SPCK(spckPin),
						hardwareCS(spiCS),
						chipSelectDecoder(csDecoder),
						SPIreg(spiReg){
	SPIreg->SPI_MR |= SPI_MR_MSTR | SPI_MR_MODFDIS;
	SPIreg->SPI_CSR[0] |= SPI_CSR_NCPHA | SPI_CSR_SCBR(8); //Can be as low as 2
	SPIreg->SPI_CR |= SPI_CR_SPIEN;	
	
	while(!(SPIreg->SPI_SR & SPI_SR_SPIENS)){};				
} //SPI_imp

// default destructor
SPIDriver::~SPIDriver()
{
} //~SPI_imp



//void SPI0_Handler(SPI0_IRQn){
	
//}