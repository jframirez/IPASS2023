/* 
* ILI9341Driver.cpp
*
* Created: 10-5-2023 20:11:57
* Author: Jordan
*/


#include "../include/ILI9341Driver.h"

uint16_t operator>>(ILI_COLORS lhs, uint shift){
	return ((uint16_t)lhs >> shift);
}

uint16_t operator&(ILI_COLORS lhs, uint rhs){
	return (((uint16_t)lhs & rhs));
}


// default constructor
ILI9341Driver::ILI9341Driver(	uint width,
				uint height,
				PINDriver & chipselect,
				PINDriver & dc,
				PINDriver & reset,
				SPIDriver & spi):
				displaySS(chipselect),
				displayDC(dc),
				displayRESET(reset),
				mySPI(spi)
				{		
					
					SetWidth(width);
					SetHeight(height);
							
					Helper::Debug::DebugPrint("ILI DRIVER INIT: \r\n");
					Helper::Debug::DebugPrint("ILI DRIVER: Hardware reset\r\n");
					HardwareReset();
					
					//Software reset
					Helper::Debug::DebugPrint("ILI DRIVER: Software reset\r\n");
					SoftwareReset();
					
					CS_Disable();

					Helper::Debug::DebugPrint("ILI DRIVER: CMD INIT\r\n");
					
					//Magic?, this command is necessary but is undocumented, but exist in all existing library's 
					SendCommandWithParamter(0xEF, 3, 0x03, 0x80, 0x02);
					
					SendCommandWithParamter(ILI9341CMD_Power_on_sequence_control, 4, 0x64, 0x03, 0x12, 0x81);
					
					SendCommandWithParamter(ILI9341CMD_Pump_ratio_control, 1, 0x20);
					
					SendCommandWithParamter(ILI9341CMD_Driver_timing_control_A, 3, 0x85, 0x00, 0x78);
					SendCommandWithParamter(ILI9341CMD_Power_control_A, 5, 0x39, 0x2C, 0x00, 0x34, 0x02);
					
					
					SendCommandWithParamter(ILI9341CMD_Driver_timing_control_B, 2, 0x00, 0x00);
					SendCommandWithParamter(ILI9341CMD_Power_control_B, 3, 0x00, 0xC1, 0x30);

					SendCommandWithParamter(ILI9341CMD_Power_Control_1, 1, 0x23);
					SendCommandWithParamter(ILI9341CMD_Power_Control_2, 1, 0x10);
					
					SendCommandWithParamter(ILI9341CMD_VCOM_Control_1, 2, 0x3e, 0x28);
					SendCommandWithParamter(ILI9341CMD_VCOM_Control_2, 1, 0x86);
					
					SetRotation(ILI_ROTATION_MODE::LANDSCAPE, false, true);
					
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
}

void ILI9341Driver::SendCommandWithParamter(uint8_t command, int parameterN, ...){
	CS_Enable();
	DC_SetCommand();
	mySPI.SPISend(command);
	DC_SetData();

	va_list args;
	va_start(args, parameterN);

	for(int i = 0; i < parameterN; ++i){
		uint8_t byte = (uint8_t)va_arg(args, int);
		mySPI.SPISend(byte);
	}
	
	va_end(args);
	
	CS_Disable();
}

void ILI9341Driver::HardwareReset(){
	CS_Disable();
	DC_SetCommand();
	//RESET Display
	displayRESET.SetOutput(PIO_PIN_STATE::LOW);
	Helper::Time::delay1_5us(200 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	displayRESET.SetOutput(PIO_PIN_STATE::HIGH);
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

void ILI9341Driver::SetWidth(uint w){
	//Max is ILI9341_WIDTH_X
	if(w > ILI9341_WIDTH_X){
		w = ILI9341_WIDTH_X;
	}
	
	width = w;
	
	colStart = 0;
	colEnd = width - 1;
	colLenght = colEnd - colStart;
}

void ILI9341Driver::SetHeight(uint h){
	//Max is ILI9341_WIDTH_X
	if(h > ILI9341_WIDTH_X){
		h = ILI9341_WIDTH_X;
	}
	
	height = h;
	
	rowStart = 0;
	rowEnd = height - 1;
	rowLenght = rowEnd - rowStart;
}

void ILI9341Driver::SetRotation(ILI_ROTATION_MODE mode, bool cont, bool setCASETPASET){
	uint8_t MADCTL = 0;
		
	switch (mode){
		case ILI_ROTATION_MODE::LANDSCAPE_INVERTED:
			SetWidth(ILI9341_WIDTH_X);
			SetHeight(ILI9341_HEIGHT_Y);
			
			MADCTL = (ILI9341_MADCTL_MV | ILI9341_MADCTL_ML | ILI9341_MADCTL_BGR);
			break;
		case ILI_ROTATION_MODE::LANDSCAPE:
			SetWidth(ILI9341_WIDTH_X);
			SetHeight(ILI9341_HEIGHT_Y);
		
			MADCTL = (ILI9341_MADCTL_MV | ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
			break;	
		case ILI_ROTATION_MODE::PORTRAIT:
			SetWidth(ILI9341_HEIGHT_Y);
			SetHeight(ILI9341_WIDTH_X);
			
			MADCTL = (ILI9341_MADCTL_MX | ILI9341_MADCTL_ML | ILI9341_MADCTL_BGR);
			break;
		case ILI_ROTATION_MODE::PORTRAIT_INVERTED:
			SetWidth(ILI9341_HEIGHT_Y);
			SetHeight(ILI9341_WIDTH_X);
			
			MADCTL = (ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
			break;			
		default: //Default is landscape
			MADCTL = (ILI9341_MADCTL_MX | ILI9341_MADCTL_ML | ILI9341_MADCTL_BGR);
	}
	
	if(cont){
		SendCommand(ILI9341CMD_Memory_Access_Control);
		SendDataCont(MADCTL);
		if(setCASETPASET){
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
		if(setCASETPASET){
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

void ILI9341Driver::SendTestPatternColorBlocks(){
	
	//ILI_COLORS currentColor = ILI_COLORS::GREEN;
	std::vector<ILI_COLORS> colorList = {	ILI_COLORS::GREEN,
											ILI_COLORS::RED,
											ILI_COLORS::BLACK,
											ILI_COLORS::WHITE,
											ILI_COLORS::BLUE,
											ILI_COLORS::ORANGE,
											ILI_COLORS::PURPLE,
											ILI_COLORS::YELLOW};
											
	auto getVal = [&](int n)	{
		return colorList[n%colorList.size()];
	};
	
	SetRotation(ILI_ROTATION_MODE::LANDSCAPE, false, true);
	
	SendStartCont();
	SendCommandCont(ILI9341CMD_Memory_Write);
	
	int blocksize = 20;
	
	//I,J offset -1;
	for (int i = 1; i <= 240/blocksize; ++i){
		for (int z = 1; z <= blocksize; ++z){
			for (int j = 1; j <= 320; j+=blocksize){
				for (int k = 0; k < blocksize; ++k){
					SendDataCont16_t(getVal((j) + (i)));
				}
			}
		}	
	}
	
	SendEndCont();
	
	Helper::Time::delay1_5us(2000 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	
	SetRotation(ILI_ROTATION_MODE::LANDSCAPE_INVERTED, false, true);
	
	SendStartCont();
	SendCommandCont(ILI9341CMD_Memory_Write);
	
	//I,J offset -1;
	for (int i = 1; i <= 240/blocksize; ++i){
		for (int z = 1; z <= blocksize; ++z){
			for (int j = 1; j <= 320; j+=blocksize){
				for (int k = 0; k < blocksize; ++k){
					SendDataCont16_t(getVal((j) + (i)));
				}
			}
		}
	}
	
	SendEndCont();
	
	Helper::Time::delay1_5us(2000 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	
	SetRotation(ILI_ROTATION_MODE::PORTRAIT, false, true);
	
	SendStartCont();
	SendCommandCont(ILI9341CMD_Memory_Write);
	
	//I,J offset -1;
	
	for (int z = 1; z <= 320/blocksize; ++z){
		for (int k = 1; k <= blocksize; ++k){
			for (int i = 1; i <= 240; ++i){
				SendDataCont16_t(getVal(z));
			}
		}
	}
	
	SendEndCont();
	
	Helper::Time::delay1_5us(2000 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	
	SetRotation(ILI_ROTATION_MODE::PORTRAIT_INVERTED, false, true);
	
	SendStartCont();
	SendCommandCont(ILI9341CMD_Memory_Write);
	
	//I,J offset -1;
	
	for (int z = 1; z <= 320/blocksize; ++z){
		for (int k = 1; k <= blocksize; ++k){
			for (int i = 1; i <= 240; ++i){
				SendDataCont16_t(getVal(z));
			}
		}
	}
	
	SendEndCont();
	
	Helper::Time::delay1_5us(2000 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	
	return;
}

// default destructor
ILI9341Driver::~ILI9341Driver(){
}
