/* 
* ILI9341Driver.cpp
*
* Created: 10-5-2023 20:11:57
* Author: Jordan
*/


#include "ILI9341Driver.h"

uint16_t operator>>(ILI_COLORS lhs, uint shift){
	return ((uint16_t)lhs >> shift);
}

uint16_t operator&(ILI_COLORS lhs, uint rhs){
	return (((uint16_t)lhs & rhs));
}


// default constructor
ILI9341Driver::ILI9341Driver(	PINDriver & chipselect,
				PINDriver & dc,
				PINDriver & reset,
				SPIDriver & spi):
				displaySS(chipselect),
				displayDC(dc),
				displayRESET(reset),
				mySPI(spi){		
					char buffer[256];
					Helper::Debug::DebugPrint("ILI9341 using Font: \r\nFONT NAME: ");
					Helper::Debug::DebugPrint(font_ubuntumono_10.name);
					Helper::Debug::DebugPrint("\r\nFONT SIZE: ");
					Helper::Debug::DebugPrint(itoa(font_ubuntumono_10.size, buffer, 10));
					Helper::Debug::DebugPrint("\r\n");
					
					
					Helper::Debug::DebugPrint("ILI DRIVER: Hardware reset\r\n");
					HardwareReset();
					
					//Software reset
					Helper::Debug::DebugPrint("ILI DRIVER: Software reset\r\n");
					SoftwareReset();
					
					CS_Disable();
					Helper::Debug::DebugPrint("ILI DRIVER: STATUS END\r\n");

					Helper::Debug::DebugPrint("ILI DRIVER: CMD INIT\r\n");
					
					//Magic?
					SendCommandWithParamter(0xEF, 3, 0x03, 0x80, 0x02);
					
					SendCommandWithParamter(0xCF, 3, 0x00, 0xC1, 0x30);
					
					SendCommandWithParamter(0xED, 4, 0x64, 0x03, 0x12, 0x81);
					
					SendCommandWithParamter(0xE8, 3, 0x85, 0x00, 0x78);
					
					
					
					SendCommandWithParamter(0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02);
					
					SendCommandWithParamter(0xF7, 1, 0x20);
					
					SendCommandWithParamter(0xEA, 2, 0x00, 0x00);

					SendCommandWithParamter(ILI9341CMD_Power_Control_1, 1, 0x23);
					SendCommandWithParamter(ILI9341CMD_Power_Control_2, 1, 0x10);
					
					SendCommandWithParamter(ILI9341CMD_VCOM_Control_1, 2, 0x3e, 0x28);
					SendCommandWithParamter(ILI9341CMD_VCOM_Control_2, 1, 0x86);
					
					//Default = ILI9341_MADCTL_MX | ILI9341_MADCTL_ML PORTRAIT
					//ILI_SendCommandWithParamter(ILI9341CMD_Memory_Access_Control, 1, (ILI9341_MADCTL_MV | ILI9341_MADCTL_ML | ILI9341_MADCTL_BGR));

					SetRotation(ILI_ROTATION_MODE::LANDSCAPE, false, true);
					
					//SoftwareReset(); // Software reset after SetRotation sets CASET & PASET to correct values
					//Can also be set manualy.
					
					SendCommandWithParamter(ILI9341CMD_Vertical_Scrolling_Start_Address, 1, 0x00);
					
					SendCommandWithParamter(ILI9341CMD_COLMOD_Pixel_Format_Set, 1, 0x55);
					SendCommandWithParamter(ILI9341CMD_Frame_Rate_Control_In_Normal_Mode_Full_Colors, 2, 0x00, 0x18);
					
					SendCommandWithParamter(ILI9341CMD_Display_Function_Control, 3, 0x08, 0x82, 0x27);
					
					SendCommandWithParamter(ILI9341CMD_Enable_3G, 1, 0x00);
					
					SendCommandWithParamter(ILI9341CMD_Gamma_Set, 1, 0x01);
					SendCommandWithParamter(ILI9341CMD_Positive_Gamma_Correction, 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00);
					SendCommandWithParamter(ILI9341CMD_Negative_Gamma_Correction, 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F);

					Helper::Debug::DebugPrint("ILI DRIVER: Set sleep mode\r\n");
					SetSleepMode(ILI_SLEEP_MODE::SLEEP_OUT);

					SendCommand(ILI9341CMD_Display_ON);
					
					Helper::Debug::DebugPrint("ILI DRIVER: Display on\r\n");
					
					/*
					SendStartCont();
					SendCommandCont(ILI9341CMD_Memory_Write);
					for (uint i = 0; i < max_pixel_count; ++i){
						SendDataCont16_t(ILI_COLORS::RED);
					}
					SendEndCont();
					
					Helper::Time::delay1_5us(4 * Helper::Time::TIME_UNIT_1_5US::SECOND);
					SetRotation(ILI_ROTATION_MODE::PORTRAIT, false, true);
					
					SendStartCont();
					SendCommandCont(ILI9341CMD_Memory_Write);
					for (uint i = 0; i < max_pixel_count; ++i){
						SendDataCont16_t(ILI_COLORS::BLUE);
					}
					SendEndCont();
					*/
					
					/*
					//Prost read display status
					Helper::Debug::DebugPrint("ILI DRIVER Post: STATUS\r\n");
					ILI_CS_Enable();
										
					ILI_DC_SetCommand();
					mySPI.SPISend(ILI9341CMD_Read_Display_Status);
					ILI_DC_SetData();
					mySPI.SPISend(0x00); //DUMMY & empty
					while(!(SPI0->SPI_SR & SPI_SR_RDRF)){};
					Helper::Debug::DebugPrint((uint8_t)(SPI0->SPI_RDR & 0xFF)); //LLB
					mySPI.SPISend(0x00); //DUMMY & first
					while(!(SPI0->SPI_SR & SPI_SR_RDRF)){};
					Helper::Debug::DebugPrint((uint8_t)(SPI0->SPI_RDR & 0xFF)); //LLB
					mySPI.SPISend(0x00); //DUMMY & first
					while(!(SPI0->SPI_SR & SPI_SR_RDRF)){};
					Helper::Debug::DebugPrint((uint8_t)(SPI0->SPI_RDR & 0xFF)); //LLB
					mySPI.SPISend(0x00); //DUMMY & first
					while(!(SPI0->SPI_SR & SPI_SR_RDRF)){};
					Helper::Debug::DebugPrint((uint8_t)(SPI0->SPI_RDR & 0xFF)); //LLB
					mySPI.SPISend(0x00); //DUMMY & first
					while(!(SPI0->SPI_SR & SPI_SR_RDRF)){};
					Helper::Debug::DebugPrint((uint8_t)(SPI0->SPI_RDR & 0xFF)); //LLB
										
					ILI_CS_Disable();
					Helper::Debug::DebugPrint("ILI DRIVER Post: STATUS END\r\n");
					*/							
}

void ILI9341Driver::SendCommandWithParamter(uint8_t command, int parameterN, ...){
	//Helper::Debug::DebugPrint("Debug per line\r\n");
	CS_Enable();
	DC_SetCommand();
	mySPI.SPISend(command);
	DC_SetData();
	//Helper::Debug::DebugPrint((uint8_t)(SPI0->SPI_RDR & 0xFF)); //LLB
	
	//Helper::Debug::DebugPrint(command);
	va_list args;
	va_start(args, parameterN);
	//Helper::Debug::DebugPrint("ILI DRIVER TEST\r\n");
	for(int i = 0; i < parameterN; ++i){
		uint8_t byte = (uint8_t)va_arg(args, int);
		//Helper::Debug::DebugPrint(byte);
		//Helper::Debug::DebugPrint("\r\n");
		mySPI.SPISend(byte);
		//Helper::Debug::DebugPrint((uint8_t)(SPI0->SPI_RDR & 0xFF)); //LLB
	}
	va_end(args);
	
	//Helper::Debug::DebugPrint("\r\n");
	CS_Disable();
}

void ILI9341Driver::HardwareReset(){
	CS_Disable();
	DC_SetCommand();
	//RESET Display
	displayRESET.SetOutput(PIO_OUTPUT::LOW);
	Helper::Time::delay1_5us(200 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	displayRESET.SetOutput(PIO_OUTPUT::HIGH);
	Helper::Time::delay1_5us(200 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
}

void ILI9341Driver::SoftwareReset(){
	SendCommand(ILI9341CMD_Software_Reset);
	//If mode sleep out delay 150ms else 5ms minimum
	Helper::Time::delay1_5us(200 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	
	currentSleepMode = ILI_SLEEP_MODE::SLEEP_IN;
	
}

void ILI9341Driver::SetSleepMode(ILI_SLEEP_MODE set){
	if(set == ILI_SLEEP_MODE::SLEEP_IN){
		SendCommand(ILI9341CMD_Enter_Sleep_Mode);
		currentSleepMode = ILI_SLEEP_MODE::SLEEP_IN;
		Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	}
	
	if(set == ILI_SLEEP_MODE::SLEEP_OUT){
		SendCommand(ILI9341CMD_Sleep_Out);
		currentSleepMode = ILI_SLEEP_MODE::SLEEP_OUT;
		Helper::Time::delay1_5us(120 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	}
}

void ILI9341Driver::SendCommand(uint8_t byte){
	CS_Enable();
	DC_SetCommand();
	mySPI.SPISend(byte);
	CS_Disable();	
}

void ILI9341Driver::SetRotation(ILI_ROTATION_MODE mode, bool Cont, bool SetCASETPASET){
	uint8_t MADCTL = 0;
	switch (mode){
		case ILI_ROTATION_MODE::LANDSCAPE:
			//LANDSCAPE
			width = ILI9341_WIDTH_X;
			height = ILI9341_HEIGHT_Y;
			
			colStart = 0;
			colEnd = width - 1;
			colLenght = colEnd - colStart;
			
			rowStart = 0;
			rowEnd = height - 1;
			rowLenght = rowEnd - rowStart;
			
			MADCTL = (ILI9341_MADCTL_MV | ILI9341_MADCTL_ML | ILI9341_MADCTL_BGR);
			break;
		case ILI_ROTATION_MODE::PORTRAIT:
			//PORTRAIT
			width = ILI9341_HEIGHT_Y;
			height = ILI9341_WIDTH_X;
			
			colStart = 0;
			colEnd = width - 1;
			colLenght = colEnd - colStart;
			
			rowStart = 0;
			rowEnd = height - 1;
			rowLenght = rowEnd - rowStart;
			
			MADCTL = (ILI9341_MADCTL_MX | ILI9341_MADCTL_ML | ILI9341_MADCTL_BGR);
			break;
		default:
			MADCTL = (ILI9341_MADCTL_MX | ILI9341_MADCTL_ML | ILI9341_MADCTL_BGR);
	}
	
	//Default = ILI9341_MADCTL_MX | ILI9341_MADCTL_ML PORTRAIT
	if(Cont){
		SendCommand(ILI9341CMD_Memory_Access_Control);
		SendDataCont(MADCTL);
		if(SetCASETPASET){
			SendCommandCont(ILI9341CMD_Column_Address_Set);
			SendDataCont(0x00);
			SendDataCont(0x00);
			SendDataCont(width >> 8);
			SendDataCont(width & 0xFF);
			SendCommandCont(ILI9341CMD_Page_Address_Set);
			SendDataCont(0x00);
			SendDataCont(0x00);
			SendDataCont(height >> 8);
			SendDataCont(height & 0xFF);
		}
	}else{
		SendCommandWithParamter(ILI9341CMD_Memory_Access_Control, 1, MADCTL);
		if(SetCASETPASET){
			SendCommandWithParamter(ILI9341CMD_Column_Address_Set, 4, 0x00, 0x00, width >> 8, width & 0xFF);
			SendCommandWithParamter(ILI9341CMD_Page_Address_Set, 4, 0x00, 0x00, height >> 8, height & 0xFF);
		}
	}
}

void ILI9341Driver::SendStartCont(){
	CS_Enable();
}

void ILI9341Driver::SendEndCont(){
	CS_Disable();
}
void ILI9341Driver::SendCommandCont(uint8_t byte){
	DC_SetCommand();
	mySPI.SPISend(byte);
}
void ILI9341Driver::SendDataCont(uint8_t byte){
	DC_SetData();
	mySPI.SPISend(byte);
}

// default destructor
ILI9341Driver::~ILI9341Driver()
{
} //~ILI9341Driver
