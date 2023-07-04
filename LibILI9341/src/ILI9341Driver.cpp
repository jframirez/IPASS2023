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
ILI9341Driver::ILI9341Driver(	uint width,
				uint height,
				PinDriver & chipselect,
				PinDriver & dc,
				PinDriver & reset,
				SpiDriver & spi):
				display_cs(chipselect),
				display_dc(dc),
				display_reset(reset),
				ili_spi(spi)
				{		
					
					setWidth(width);
					setHeight(height);
							
					Helper::Debug::DebugPrint("ILI DRIVER INIT: \r\n");
					Helper::Debug::DebugPrint("ILI DRIVER: Hardware reset\r\n");
					hardwareReset();
					
					//Software reset
					Helper::Debug::DebugPrint("ILI DRIVER: Software reset\r\n");
					softwareReset();
					
					ChipSelectdisable();

					Helper::Debug::DebugPrint("ILI DRIVER: CMD INIT\r\n");
					
					//Magic?, this command is necessary but is undocumented, but exist in all existing library's 
					sendCommandWithParamter(0xEF, 3, 0x03, 0x80, 0x02);
					
					sendCommandWithParamter(ILI9341CMD_Power_on_sequence_control, 4, 0x64, 0x03, 0x12, 0x81);
					
					sendCommandWithParamter(ILI9341CMD_Pump_ratio_control, 1, 0x20);
					
					sendCommandWithParamter(ILI9341CMD_Driver_timing_control_A, 3, 0x85, 0x00, 0x78);
					sendCommandWithParamter(ILI9341CMD_Power_control_A, 5, 0x39, 0x2C, 0x00, 0x34, 0x02);
					
					
					sendCommandWithParamter(ILI9341CMD_Driver_timing_control_B, 2, 0x00, 0x00);
					sendCommandWithParamter(ILI9341CMD_Power_control_B, 3, 0x00, 0xC1, 0x30);

					sendCommandWithParamter(ILI9341CMD_Power_Control_1, 1, 0x23);
					sendCommandWithParamter(ILI9341CMD_Power_Control_2, 1, 0x10);
					
					sendCommandWithParamter(ILI9341CMD_VCOM_Control_1, 2, 0x3e, 0x28);
					sendCommandWithParamter(ILI9341CMD_VCOM_Control_2, 1, 0x86);
					
					setRotation(ILI_ROTATION_MODE::LANDSCAPE, false, true);
					
					sendCommandWithParamter(ILI9341CMD_Vertical_Scrolling_Start_Address, 1, 0x00);
					
					sendCommandWithParamter(ILI9341CMD_COLMOD_Pixel_Format_Set, 1, 0x55);
					sendCommandWithParamter(ILI9341CMD_Frame_Rate_Control_In_Normal_Mode_Full_Colors, 2, 0x00, 0x18);
					
					sendCommandWithParamter(ILI9341CMD_Display_Function_Control, 3, 0x08, 0x82, 0x27);
					
					sendCommandWithParamter(ILI9341CMD_Enable_3G, 1, 0x00);
					
					sendCommandWithParamter(ILI9341CMD_Gamma_Set, 1, 0x01);
					sendCommandWithParamter(ILI9341CMD_Positive_Gamma_Correction, 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00);
					sendCommandWithParamter(ILI9341CMD_Negative_Gamma_Correction, 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F);

					Helper::Debug::DebugPrint("ILI DRIVER: Set sleep mode\r\n");
					setSleepMode(ILI_SLEEP_MODE::SLEEP_OUT);

					sendCommand(ILI9341CMD_Display_ON);
					
					Helper::Debug::DebugPrint("ILI DRIVER: Display on\r\n");					
}

void ILI9341Driver::sendCommandWithParamter(uint8_t command, int parameterN, ...){
	ChipSelectEnable();
	DCSetCommand();
	ili_spi.SpiSend(command);
	DCSetData();

	va_list args;
	va_start(args, parameterN);

	for(int i = 0; i < parameterN; ++i){
		uint8_t byte = (uint8_t)va_arg(args, int);
		ili_spi.SpiSend(byte);
	}
	
	va_end(args);
	
	ChipSelectdisable();
}

void ILI9341Driver::hardwareReset(){
	ChipSelectdisable();
	DCSetCommand();
	//RESET Display
	display_reset.setOutput(PIO_PIN_STATE::LOW);
	Helper::Time::delay1_5us(200 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	display_reset.setOutput(PIO_PIN_STATE::HIGH);
	Helper::Time::delay1_5us(200 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
}

void ILI9341Driver::softwareReset(){
	sendCommand(ILI9341CMD_Software_Reset);
	//If mode sleep out delay 150ms else 5ms minimum
	Helper::Time::delay1_5us(200 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	
	current_sleep_mode = ILI_SLEEP_MODE::SLEEP_IN;
	
}

void ILI9341Driver::setSleepMode(ILI_SLEEP_MODE set){
	if(set == ILI_SLEEP_MODE::SLEEP_IN){
		sendCommand(ILI9341CMD_Enter_Sleep_Mode);
		current_sleep_mode = ILI_SLEEP_MODE::SLEEP_IN;
		Helper::Time::delay1_5us(5 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	}
	
	if(set == ILI_SLEEP_MODE::SLEEP_OUT){
		sendCommand(ILI9341CMD_Sleep_Out);
		current_sleep_mode = ILI_SLEEP_MODE::SLEEP_OUT;
		Helper::Time::delay1_5us(120 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	}
}

void ILI9341Driver::sendCommand(uint8_t byte){
	ChipSelectEnable();
	DCSetCommand();
	ili_spi.SpiSend(byte);
	ChipSelectdisable();	
}

void ILI9341Driver::setWidth(uint w){
	//Max is ILI9341_WIDTH_X
	if(w > ILI9341_WIDTH_X){
		w = ILI9341_WIDTH_X;
	}
	
	width = w;
	
	col_start = 0;
	col_end = width - 1;
	col_lenght = col_end - col_start;
}

void ILI9341Driver::setHeight(uint h){
	//Max is ILI9341_WIDTH_X
	if(h > ILI9341_WIDTH_X){
		h = ILI9341_WIDTH_X;
	}
	
	height = h;
	
	row_start = 0;
	row_end = height - 1;
	row_lenght = row_end - row_start;
}

void ILI9341Driver::setRotation(ILI_ROTATION_MODE mode, bool cont, bool setCASETPASET){
	uint8_t MADCTL = 0;
		
	switch (mode){
		case ILI_ROTATION_MODE::LANDSCAPE_INVERTED:
			setWidth(ILI9341_WIDTH_X);
			setHeight(ILI9341_HEIGHT_Y);
			
			MADCTL = (ILI9341_MADCTL_MV | ILI9341_MADCTL_ML | ILI9341_MADCTL_BGR);
			break;
		case ILI_ROTATION_MODE::LANDSCAPE:
			setWidth(ILI9341_WIDTH_X);
			setHeight(ILI9341_HEIGHT_Y);
		
			MADCTL = (ILI9341_MADCTL_MV | ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
			break;	
		case ILI_ROTATION_MODE::PORTRAIT:
			setWidth(ILI9341_HEIGHT_Y);
			setHeight(ILI9341_WIDTH_X);
			
			MADCTL = (ILI9341_MADCTL_MX | ILI9341_MADCTL_ML | ILI9341_MADCTL_BGR);
			break;
		case ILI_ROTATION_MODE::PORTRAIT_INVERTED:
			setWidth(ILI9341_HEIGHT_Y);
			setHeight(ILI9341_WIDTH_X);
			
			MADCTL = (ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
			break;			
		default: //Default is landscape
			MADCTL = (ILI9341_MADCTL_MX | ILI9341_MADCTL_ML | ILI9341_MADCTL_BGR);
	}
	
	if(cont){
		sendCommand(ILI9341CMD_Memory_Access_Control);
		sendDataCont(MADCTL);
		if(setCASETPASET){
			sendCommandCont(ILI9341CMD_Column_Address_Set);
			sendDataCont(0x00);
			sendDataCont(0x00);
			sendDataCont(width >> 8);
			sendDataCont(width & 0xFF);
			sendCommandCont(ILI9341CMD_Page_Address_Set);
			sendDataCont(0x00);
			sendDataCont(0x00);
			sendDataCont(height >> 8);
			sendDataCont(height & 0xFF);
		}
	}else{
		sendCommandWithParamter(ILI9341CMD_Memory_Access_Control, 1, MADCTL);
		if(setCASETPASET){
			sendCommandWithParamter(ILI9341CMD_Column_Address_Set, 4, 0x00, 0x00, width >> 8, width & 0xFF);
			sendCommandWithParamter(ILI9341CMD_Page_Address_Set, 4, 0x00, 0x00, height >> 8, height & 0xFF);
		}
	}
}

void ILI9341Driver::sendStartCont(){
	ChipSelectEnable();
}

void ILI9341Driver::sendEndCont(){
	ChipSelectdisable();
}
void ILI9341Driver::sendCommandCont(uint8_t byte){
	DCSetCommand();
	ili_spi.SpiSend(byte);
}
void ILI9341Driver::sendDataCont(uint8_t byte){
	DCSetData();
	ili_spi.SpiSend(byte);
}

const uint ILI9341Driver::getMaxPixelCount(){
	return max_pixel_count;
}

uint ILI9341Driver::getColStart(){
	return col_start;
}

uint ILI9341Driver::getColEnd(){
	return col_end;
}

uint ILI9341Driver::getColLenght(){
	return col_lenght;
}

uint ILI9341Driver::getRowStart(){
	return row_start;
}

uint ILI9341Driver::getRowEnd(){
	return row_end;
}

uint ILI9341Driver::getRowLenght(){
	return row_lenght;
}

//////////////////////////////////////////////////////////////////////////

void ILI9341Driver::setColStart(uint val){
	col_start = val;
}

void ILI9341Driver::setColEnd(uint val){
	col_end = val;
}

void ILI9341Driver::setColLenght(uint val){
	col_lenght = val;
}

void ILI9341Driver::setRowStart(uint val){
	row_start = val;
}

void ILI9341Driver::setRowEnd(uint val){
	row_end = val;
}

void ILI9341Driver::setRowLenght(uint val){
	row_lenght = val;
}

void ILI9341Driver::sendTestPatternColorBlocks(){
	
	//ILI_COLORS currentColor = ILI_COLORS::GREEN;
	std::vector<ILI_COLORS> color_list = {	ILI_COLORS::GREEN,
											ILI_COLORS::RED,
											ILI_COLORS::BLACK,
											ILI_COLORS::WHITE,
											ILI_COLORS::BLUE,
											ILI_COLORS::ORANGE,
											ILI_COLORS::PURPLE,
											ILI_COLORS::YELLOW};
											
	auto getVal = [&](int n)	{
		return color_list[n%color_list.size()];
	};
	
	setRotation(ILI_ROTATION_MODE::LANDSCAPE, false, true);
	
	sendStartCont();
	sendCommandCont(ILI9341CMD_Memory_Write);
	
	int blocksize = 20;
	
	//I,J offset -1;
	for (int i = 1; i <= 240/blocksize; ++i){
		for (int z = 1; z <= blocksize; ++z){
			for (int j = 1; j <= 320; j+=blocksize){
				for (int k = 0; k < blocksize; ++k){
					sendDataCont16_t(getVal((j) + (i)));
				}
			}
		}	
	}
	
	sendEndCont();
	
	Helper::Time::delay1_5us(2000 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	
	setRotation(ILI_ROTATION_MODE::LANDSCAPE_INVERTED, false, true);
	
	sendStartCont();
	sendCommandCont(ILI9341CMD_Memory_Write);
	
	//I,J offset -1;
	for (int i = 1; i <= 240/blocksize; ++i){
		for (int z = 1; z <= blocksize; ++z){
			for (int j = 1; j <= 320; j+=blocksize){
				for (int k = 0; k < blocksize; ++k){
					sendDataCont16_t(getVal((j) + (i)));
				}
			}
		}
	}
	
	sendEndCont();
	
	Helper::Time::delay1_5us(2000 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	
	setRotation(ILI_ROTATION_MODE::PORTRAIT, false, true);
	
	sendStartCont();
	sendCommandCont(ILI9341CMD_Memory_Write);
	
	//I,J offset -1;
	
	for (int z = 1; z <= 320/blocksize; ++z){
		for (int k = 1; k <= blocksize; ++k){
			for (int i = 1; i <= 240; ++i){
				sendDataCont16_t(getVal(z));
			}
		}
	}
	
	sendEndCont();
	
	Helper::Time::delay1_5us(2000 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	
	setRotation(ILI_ROTATION_MODE::PORTRAIT_INVERTED, false, true);
	
	sendStartCont();
	sendCommandCont(ILI9341CMD_Memory_Write);
	
	//I,J offset -1;
	
	for (int z = 1; z <= 320/blocksize; ++z){
		for (int k = 1; k <= blocksize; ++k){
			for (int i = 1; i <= 240; ++i){
				sendDataCont16_t(getVal(z));
			}
		}
	}
	
	sendEndCont();
	
	Helper::Time::delay1_5us(2000 * Helper::Time::TIME_UNIT_1_5US::MILLISECOND);
	
	return;
}

// default destructor
ILI9341Driver::~ILI9341Driver(){
}
