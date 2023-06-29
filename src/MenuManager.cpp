/* 
* MenuManager.cpp
*
* Created: 22-5-2023 13:48:15
* Author: Jordan
*/


#include "MenuManager.h"

// default constructor
MenuManager::MenuManager(ILI9341Driver & LCD):
	myLCD(LCD){
		
	//Fill LCD black
	myLCD.SetRotation(ILI_ROTATION_MODE::LANDSCAPE, false, true);
	myLCD.SendStartCont();
	myLCD.SendCommandCont(ILI9341CMD_Memory_Write);
	for (uint i = 0; i < myLCD.max_pixel_count; ++i){
		myLCD.SendDataCont16_t(ILI_COLORS::BLACK);
	}
	myLCD.SendEndCont();
} //MenuManager

int MenuManager::SetMenu(const menu * m, ILI_COLORS backfill){
	
	usedOnce = false;
	
	currentMenu = m;
	
	myLCD.SetRotation((ILI_ROTATION_MODE)currentMenu->rotation, false, true);
	
	myLCD.SendStartCont();
	myLCD.SendCommandCont(ILI9341CMD_Memory_Write);
	
	unsigned int clearLenght = ILI9341_WIDTH_X * ILI9341_HEIGHT_Y;
	
	clearLenght = clearLenght - currentMenu->bitmapLenght;
	
	for (uint i = 0; i < currentMenu->bitmapLenght; ++i){
		myLCD.SendDataCont16_t(currentMenu->menuBitmap[i]);
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
	
	for (uint i = 0; i < clearLenght; ++i){
		myLCD.SendDataCont16_t(backfill);
	}
	
	myLCD.SendEndCont();
	
	return 1;
}

void MenuManager::ClearTextLabel(uint n, ILI_COLORS clearColor){
	myLCD.SendStartCont();

	//set caset
	myLCD.SendCommandCont(ILI9341CMD_Column_Address_Set);
	//start is 50 and end is width - 50;
	myLCD.SendDataCont16_t(currentMenu->menuTextBoxes[n]->colStart); //start
	myLCD.SendDataCont16_t(currentMenu->menuTextBoxes[n]->colEnd -1); //end
	
	//should be aliased from LCD , as soon as rotation changes this bugs out
	myLCD.colStart = currentMenu->menuTextBoxes[n]->colStart; //set this + bounds check
	myLCD.colEnd = currentMenu->menuTextBoxes[n]->colEnd; //set this + bounds check
	myLCD.colLenght = currentMenu->menuTextBoxes[n]->colLenghtv;
	
	myLCD.rowStart = currentMenu->menuTextBoxes[n]->rowStart; //set this + bounds check
	myLCD.rowEnd = currentMenu->menuTextBoxes[n]->rowEnd; //set this + bounds check
	myLCD.rowLenght = currentMenu->menuTextBoxes[n]->rowLenght;
	
	//set paset
	myLCD.SendCommandCont(ILI9341CMD_Page_Address_Set);
	//set paset to start 120
	// end is max
	myLCD.SendDataCont16_t(currentMenu->menuTextBoxes[n]->rowStart); //start
	myLCD.SendDataCont16_t(currentMenu->menuTextBoxes[n]->rowEnd -1); //end
	
	myLCD.SendEndCont();
	
	myLCD.SendStartCont();
	myLCD.SendCommandCont(ILI9341CMD_Memory_Write);
	
	for (uint row = 0; row < currentMenu->menuTextBoxes[n]->rowLenght; ++row){
		for(uint column = 0; column < currentMenu->menuTextBoxes[n]->colLenghtv; ++column){
			myLCD.SendDataCont16_t(clearColor);
		}
	}
	
	myLCD.SendEndCont();
	
	_cur_WriteTextLabel_fullHeightOffset_lastTrue = 0; //Reset value to start cursor at 0,0 for label
}

void MenuManager::WriteTextLabel(uint n, font curFont, std::string buffer, bool contWrite){
	
		
	if(buffer[0] == '\0'){
		Helper::Debug::DebugPrint("WriteTextLabel EMPTY\r\n");
		return;
	}

	uint curResize = curFont.height - curFont.size; // 16
	
	uint pasetOffset = 0;
	
	if(usedOnce && contWrite && (_prevLabel == static_cast<int>(n))){
		pasetOffset = _cur_WriteTextLabel_fullHeightOffset_lastTrue;
		_cur_WriteTextLabel_fullHeightOffset = _cur_WriteTextLabel_fullHeightOffset_lastTrue;
	}else{
		_cur_WriteTextLabel_fullHeightOffset = 0;
	}
	
	
	
	uint offset_height = 0;
	
	//uint fullHeightOffset = myLCD.rowStart + pasetOffset;
	uint fullHeightOffset = currentMenu->menuTextBoxes[n]->rowStart + pasetOffset;
	uint8_t currentGlyph = buffer[0];
	uint bufferPrintOffset = 0;
	uint glyphCounter = 0;
	
	myLCD.SendStartCont();

	//set caset
	myLCD.SendCommandCont(ILI9341CMD_Column_Address_Set);
	//start is 50 and end is width - 50;
	myLCD.SendDataCont16_t(currentMenu->menuTextBoxes[n]->colStart); //start
	myLCD.SendDataCont16_t(currentMenu->menuTextBoxes[n]->colEnd -1); //end
	
	//should be aliased from LCD , as soon as rotation changes this bugs out
	myLCD.colStart = currentMenu->menuTextBoxes[n]->colStart; //set this + bounds check
	myLCD.colEnd = currentMenu->menuTextBoxes[n]->colEnd; //set this + bounds check
	myLCD.colLenght = currentMenu->menuTextBoxes[n]->colLenghtv;
	
	myLCD.rowStart = currentMenu->menuTextBoxes[n]->rowStart; //set this + bounds check
	myLCD.rowEnd = currentMenu->menuTextBoxes[n]->rowEnd; //set this + bounds check
	myLCD.rowLenght = currentMenu->menuTextBoxes[n]->rowLenght;
	
	//set paset
	myLCD.SendCommandCont(ILI9341CMD_Page_Address_Set);
	//set paset to start 120
	// end is max
	myLCD.SendDataCont16_t(currentMenu->menuTextBoxes[n]->rowStart + pasetOffset); //start
	myLCD.SendDataCont16_t(currentMenu->menuTextBoxes[n]->rowEnd -1); //end
	
	myLCD.SendEndCont();
	

	
	ILI_COLORS backgroundFill = ILI_COLORS::BLACK;
	

	
	myLCD.SendStartCont();
	myLCD.SendCommandCont(ILI9341CMD_Memory_Write);
	
	//Helper::Debug::DebugPrint("A");	
	do{
		//Helper::Debug::DebugPrint("B");	
		offset_height = 0;
		//check if next line fits else reset line height
		if(contWrite){
			if((_cur_WriteTextLabel_fullHeightOffset + curFont.height) >= myLCD.rowLenght){
				//Helper::Debug::DebugPrint("\r\nTEST OFFSET\r\n");
				_cur_WriteTextLabel_fullHeightOffset = 0;
				fullHeightOffset = 0;
				myLCD.SendEndCont();
				
				myLCD.SendStartCont();

				//set caset
				myLCD.SendCommandCont(ILI9341CMD_Column_Address_Set);
				//start is 50 and end is width - 50;
				myLCD.SendDataCont16_t(currentMenu->menuTextBoxes[n]->colStart); //start
				myLCD.SendDataCont16_t(currentMenu->menuTextBoxes[n]->colEnd -1); //end
				
				//should be aliased from LCD , as soon as rotation changes this bugs out
				myLCD.colStart = currentMenu->menuTextBoxes[n]->colStart; //set this + bounds check
				myLCD.colEnd = currentMenu->menuTextBoxes[n]->colEnd; //set this + bounds check
				myLCD.colLenght = currentMenu->menuTextBoxes[n]->colLenghtv;
				
				myLCD.rowStart = currentMenu->menuTextBoxes[n]->rowStart; //set this + bounds check
				myLCD.rowEnd = currentMenu->menuTextBoxes[n]->rowEnd; //set this + bounds check
				myLCD.rowLenght = currentMenu->menuTextBoxes[n]->rowLenght;
				
				//set paset
				myLCD.SendCommandCont(ILI9341CMD_Page_Address_Set);
				//set paset to start 120
				// end is max
				myLCD.SendDataCont16_t(currentMenu->menuTextBoxes[n]->rowStart + 0); //start
				myLCD.SendDataCont16_t(currentMenu->menuTextBoxes[n]->rowEnd -1); //end
				
				myLCD.SendEndCont();
				
				myLCD.SendStartCont();
				myLCD.SendCommandCont(ILI9341CMD_Memory_Write);
			}		
		}
		
		for (int i = 0; i < curFont.height; ++i){
			//Helper::Debug::DebugPrint("C");	
			
			glyphCounter = 0;
			currentGlyph = buffer[glyphCounter + bufferPrintOffset]; //initial buffer pos for line.
			
			if((currentGlyph == '\r')){
				bufferPrintOffset += 1;
				currentGlyph = buffer[glyphCounter + bufferPrintOffset];
			}

			uint line_lenght = 0;
			//for all glyphs
			while(line_lenght <= myLCD.colLenght){
				//Helper::Debug::DebugPrint("D");	
								
				if((currentGlyph == '\0') | (currentGlyph == '\n')){ //special chars, these chars do not get printed, \r could be supported but would need to reset all offsets
					//fill line
					//Helper::Debug::DebugPrint("E");
					for(uint k = 0; k < (myLCD.colLenght - line_lenght); ++k){
						myLCD.SendDataCont16_t(backgroundFill); //BACKGROUND FILL
					}
					if(currentGlyph == '\0'){
						break;
					}
					glyphCounter += 1;
					break;
				}else{
					//Helper::Debug::DebugPrint("F");
					//Helper::Debug::DebugPrint(currentGlyph);
					currentGlyph = currentGlyph - ' ';
					line_lenght += curFont.glyphs[currentGlyph]->advance;
				}
				//offset_width += font_UbuntuMonoB_10.glyphs[currentGlyph]->cols;
				if(line_lenght <= (myLCD.colLenght - 1 )){ //should be <= but for testing
					//Helper::Debug::DebugPrint("G");
					int col_offset = curFont.glyphs[currentGlyph]->cols;
					
					//print left blanking .left
					for(int leftC = 0; leftC < curFont.glyphs[currentGlyph]->left; ++leftC){
						//Helper::Debug::DebugPrint("H");
						//line_lenght += 1;
						myLCD.SendDataCont16_t(backgroundFill);
					}
					//Helper::Debug::DebugPrint("I");
					for (int j = 0; j < col_offset; ++j){
						int lineCheckBackFill = (curFont.height - offset_height) - (curFont.glyphs[currentGlyph]->top + curResize);
						int lineCheckPrint = offset_height - (curFont.height - (curFont.glyphs[currentGlyph]->top + curResize));
						if(lineCheckBackFill > 0){
							myLCD.SendDataCont16_t(backgroundFill); //BACKGROUND FILL
						}else if (lineCheckPrint < curFont.glyphs[currentGlyph]->rows){
							uint8_t grayscale = curFont.glyphs[currentGlyph]->bitmap[j + (lineCheckPrint * curFont.glyphs[currentGlyph]->cols)];
							// divide grayscale glyph by R >> 3, G >> 2, B >> 3
							uint16_t RGB = ( (0xF800 & ((grayscale >> 3) << 11)) | (0x07E0 & ((grayscale >> 2) << 5)) | (0x001F & (grayscale >> 3)) );
							myLCD.SendDataCont16_t(RGB);
							//0xF800 - R mask, 0x07E0 - G mask, 0x001F - B mask
						}else{
							myLCD.SendDataCont16_t(backgroundFill); //BACKGROUND FILL
						}
					}
					//Helper::Debug::DebugPrint("J");
					//print right blanking (.advance - (.left + .col))
					for(int leftC = 0; leftC < (curFont.glyphs[currentGlyph]->advance - (curFont.glyphs[currentGlyph]->left + curFont.glyphs[currentGlyph]->cols)); ++leftC){
						myLCD.SendDataCont16_t(backgroundFill);
					}
					//Helper::Debug::DebugPrint("K");
				}else{
					//Helper::Debug::DebugPrint("X");
					//send empty to fill line width or fill empty space in assigned field
					for(uint k = 0; k < (myLCD.colLenght - (line_lenght - (curFont.glyphs[currentGlyph]->advance)) ); ++k){
						myLCD.SendDataCont16_t(backgroundFill); //BACKGROUND FILL
					}
					//Helper::Debug::DebugPrint("Y");
					break;
				}
				
				//Helper::Debug::DebugPrint("L");
				glyphCounter += 1;
				currentGlyph = buffer[glyphCounter + bufferPrintOffset];
			}
			offset_height += 1;
			fullHeightOffset += 1;
			_cur_WriteTextLabel_fullHeightOffset += 1;
			
			if(fullHeightOffset >= myLCD.rowEnd){
				break; //Break immediate on end of buffer
			}
		}
		//Helper::Debug::DebugPrint("N");
		bufferPrintOffset += glyphCounter;
		
		if(fullHeightOffset >= myLCD.rowEnd){
			_cur_WriteTextLabel_fullHeightOffset = 0;
			break; //If buffer is on last row break
		}
	} while (currentGlyph != '\0'); //while (bufferPrintOffset < lineLenghtGlyphs); // 57 < 78 can this be currentGlyph != '\0'

	
	myLCD.SendEndCont();
	
	usedOnce = true;
	if(contWrite){
		_cur_WriteTextLabel_fullHeightOffset_lastTrue = _cur_WriteTextLabel_fullHeightOffset;
		_prevLabel = n;
	}
	//Helper::Debug::DebugPrint(std::to_string(_cur_WriteTextLabel_fullHeightOffset) + "\r\n");
}

// default destructor
MenuManager::~MenuManager()
{
} //~MenuManager
