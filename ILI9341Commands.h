/*
 * IncFile1.h
 *
 * Created: 11-5-2023 03:52:31
 *  Author: Jordan
 */ 


#ifndef ILI9341COMMANDS_H_
#define ILI9341COMMANDS_H_

#define ILI9341CMD_NOP (0x00) // NOP
#define ILI9341CMD_Software_Reset (0x01) // Software Reset
#define ILI9341CMD_Read_display_identification_information (0x04) // Read display identification information
#define ILI9341CMD_Read_Display_Status (0x09) // Read Display Status
#define ILI9341CMD_Enter_Sleep_Mode (0x10) // Enter Sleep Mode
#define ILI9341CMD_Sleep_Out (0x11) // Sleep Out
#define ILI9341CMD_Partial_Mode_ON (0x12) // Partial Mode ON
#define ILI9341CMD_Normal_Display_Mode_ON (0x13) // Normal Display Mode ON
#define ILI9341CMD_Display_Inversion_OFF (0x20) // Display Inversion OFF
#define ILI9341CMD_Display_Inversion_ON (0x21) // Display Inversion ON
#define ILI9341CMD_Gamma_Set (0x26) // Gamma Set
#define ILI9341CMD_Display_OFF (0x28) // Display OFF
#define ILI9341CMD_Display_ON (0x29) // Display ON
#define ILI9341CMD_Partial_Area (0x30) // Partial Area
#define ILI9341CMD_Vertical_Scrolling_Definition (0x33) // Vertical Scrolling Definition
#define ILI9341CMD_Tearing_Effect_Line_OFF (0x34) // Tearing Effect Line OFF
#define ILI9341CMD_Tearing_Effect_Line_ON (0x35) // Tearing Effect Line ON
#define ILI9341CMD_Memory_Access_Control (0x36) // Memory Access Control
#define ILI9341CMD_Vertical_Scrolling_Start_Address (0x37) // Vertical Scrolling Start Address
#define ILI9341CMD_Idle_Mode_OFF (0x38) // Idle Mode OFF
#define ILI9341CMD_Idle_Mode_ON (0x39) // Idle Mode ON
#define ILI9341CMD_Set_Tear_Scanline (0x44) // Set_Tear_Scanline
#define ILI9341CMD_Get_Scanline (0x45) // Get_Scanline
#define ILI9341CMD_Write_Display_Brightness (0x51) // Write Display Brightness
#define ILI9341CMD_Read_Display_Brightness (0x52) // Read Display Brightness
#define ILI9341CMD_Write_CTRL_Display (0x53) // Write CTRL Display
#define ILI9341CMD_Read_CTRL_Display (0x54) // Read CTRL Display
#define ILI9341CMD_Write_Content_Adaptive_Brightness_Control (0x55) // Write Content Adaptive Brightness Control
#define ILI9341CMD_Read_Content_Adaptive_Brightness_Control (0x56) // Read Content Adaptive Brightness Control
#define ILI9341CMD_Read_Display_Power_Mode (0x0A) // Read Display Power Mode
#define ILI9341CMD_Read_Display_MADCTL (0x0B) // Read Display MADCTL
#define ILI9341CMD_Read_Display_Pixel_Format (0x0C) // Read Display Pixel Format
#define ILI9341CMD_Read_Display_Image_Format (0x0D) // Read Display Image Format
#define ILI9341CMD_Read_Display_Signal_Mode (0x0E) // Read Display Signal Mode
#define ILI9341CMD_Read_Display_Self_Diagnostic_Result (0x0F) // Read Display Self-Diagnostic Result
#define ILI9341CMD_Column_Address_Set (0x2A) // Column Address Set
#define ILI9341CMD_Page_Address_Set (0x2B) // Page Address Set
#define ILI9341CMD_Memory_Write (0x2C) // Memory Write
#define ILI9341CMD_Color_Set (0x2D) // Color Set
#define ILI9341CMD_Memory_Read (0x2E) // Memory Read
#define ILI9341CMD_COLMOD_Pixel_Format_Set (0x3A) // COLMOD: Pixel Format Set
#define ILI9341CMD_Write_Memory_Continue (0x3C) // Write_Memory_Continue
#define ILI9341CMD_Read_Memory_Continue (0x3E) // Read_Memory_Continue
#define ILI9341CMD_Write_CABC_Minimum_Brightness (0x5E) // Write CABC Minimum Brightness
#define ILI9341CMD_Read_CABC_Minimum_Brightness (0x5F) // Read CABC Minimum Brightness
#define ILI9341CMD_RGB_Interface_Signal_Control (0xB0) // RGB Interface Signal Control
#define ILI9341CMD_Frame_Rate_Control_In_Normal_Mode_Full_Colors (0xB1) // Frame Rate Control (In Normal Mode/Full Colors)
#define ILI9341CMD_Frame_Rate_Control_In_Idle_Mode_8_colors (0xB2) // Frame Rate Control (In Idle Mode/8 colors)
#define ILI9341CMD_Frame_Rate_control_In_Partial_Mode_Full_Colors (0xB3) // Frame Rate control (In Partial Mode/Full Colors)
#define ILI9341CMD_Display_Inversion_Control (0xB4) // Display Inversion Control
#define ILI9341CMD_Blanking_Porch_Control (0xB5) // Blanking Porch Control
#define ILI9341CMD_Display_Function_Control (0xB6) // Display Function Control
#define ILI9341CMD_Entry_Mode_Set (0xB7) // Entry Mode Set
#define ILI9341CMD_Backlight_Control_1 (0xB8) // Backlight Control 1
#define ILI9341CMD_Backlight_Control_2 (0xB9) // Backlight Control 2
#define ILI9341CMD_Backlight_Control_3 (0xBA) // Backlight Control 3
#define ILI9341CMD_Backlight_Control_4 (0xBB) // Backlight Control 4
#define ILI9341CMD_Backlight_Control_5 (0xBC) // Backlight Control 5
#define ILI9341CMD_Backlight_Control_7 (0xBE) // Backlight Control 7
#define ILI9341CMD_Backlight_Control_8 (0xBF) // Backlight Control 8
#define ILI9341CMD_Power_Control_1 (0xC0) // Power Control 1
#define ILI9341CMD_Power_Control_2 (0xC1) // Power Control 2
#define ILI9341CMD_VCOM_Control_1 (0xC5) // VCOM Control 1
#define ILI9341CMD_VCOM_Control_2 (0xC7) // VCOM Control 2
#define ILI9341CMD_Power_control_A (0xCB) //Power control A
#define ILI9341CMD_Power_control_B (0xCF) //Power control B
#define ILI9341CMD_NV_Memory_Write (0xD0) // NV Memory Write
#define ILI9341CMD_NV_Memory_Protection_Key (0xD1) // NV Memory Protection Key
#define ILI9341CMD_NV_Memory_Status_Read (0xD2) // NV Memory Status Read
#define ILI9341CMD_Read_ID4 (0xD3) // Read ID4
#define ILI9341CMD_Read_ID1 (0xDA) // Read ID1
#define ILI9341CMD_Read_ID2 (0xDB) // Read ID2
#define ILI9341CMD_Read_ID3 (0xDC) // Read ID3
#define ILI9341CMD_Positive_Gamma_Correction (0xE0) // Positive Gamma Correction
#define ILI9341CMD_Negative_Gamma_Correction (0xE1) // Negative Gamma Correction
#define ILI9341CMD_Digital_Gamma_Control_1 (0xE2) // Digital Gamma Control 1
#define ILI9341CMD_Digital_Gamma_Control_2 (0xE3) // Digital Gamma Control 2
#define ILI9341CMD_Driver_timing_control_A (0xE8) //Driver timing control A
#define ILI9341CMD_Driver_timing_control_A_External (0xE9) //Driver timing control A
#define ILI9341CMD_Driver_timing_control_B (0xEA) //Driver timing control B
#define ILI9341CMD_Power_on_sequence_control (0xED) //Power on sequence control
#define ILI9341CMD_Enable_3G (0xF2) //Enable 3G
#define ILI9341CMD_Interface_Control (0xF6) // Interface Control
#define ILI9341CMD_Pump_ratio_control (0xF7) //Pump ratio control

#define ILI9341_MADCTL_MY (1 << 7)
#define ILI9341_MADCTL_MX (1 << 6)
#define ILI9341_MADCTL_MV (1 << 5)
#define ILI9341_MADCTL_ML (1 << 4)
#define ILI9341_MADCTL_BGR (1 << 3)
#define ILI9341_MADCTL_MH (1 << 2)

#define ILI9341_WIDTH_X 320
#define ILI9341_HEIGHT_Y 240


#endif /* ILI9341COMMANDS_H_ */