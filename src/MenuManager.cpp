/* 
* MenuManager.cpp
*
* Created: 22-5-2023 13:48:15
* Author: Jordan
*/


#include "MenuManager.h"

// default constructor
MenuManager::MenuManager(ILI9341Driver & lcd):
	my_lcd(lcd){
		
	//Fill LCD black
	my_lcd.setRotation(ILI_ROTATION_MODE::LANDSCAPE, false, true);
	my_lcd.sendStartCont();
	my_lcd.sendCommandCont(ILI9341CMD_Memory_Write);
	for (uint i = 0; i < my_lcd.getMaxPixelCount(); ++i){
		my_lcd.sendDataCont16_t(bg_color);
	}
	my_lcd.sendEndCont();
} //MenuManager

void MenuManager::setMenu(const menu * m, ILI_COLORS backfill, bool draw){
	
	//usedOnce = false;
	//_prevLabel = -1;
	
	label_param.clear();
	//cur_full_height_offset = 0;
	
	for(unsigned int i = 0; i < m->textBoxesCount; ++i){
		label_param.push_back(0);
	}
	
	if(current_menu == m && draw == false){
		return;
	}

	current_menu = m;
	
	my_lcd.setRotation((ILI_ROTATION_MODE)current_menu->rotation, false, true);
	
	my_lcd.sendStartCont();
	my_lcd.sendCommandCont(ILI9341CMD_Memory_Write);
	
	unsigned int clearLenght = my_lcd.getMaxPixelCount();
	
	clearLenght = clearLenght - current_menu->bitmapLenght;
	
	for (uint i = 0; i < current_menu->bitmapLenght; ++i){
		my_lcd.sendDataCont16_t(current_menu->menuBitmap[i]);
	}
	
	for (uint i = 0; i < clearLenght; ++i){
		my_lcd.sendDataCont16_t(backfill);
	}
	
	my_lcd.sendEndCont();
}

void MenuManager::clearTextLabel(uint n, ILI_COLORS clearColor){
	my_lcd.sendStartCont();

	//set caset
	my_lcd.sendCommandCont(ILI9341CMD_Column_Address_Set);
	//start is 50 and end is width - 50;
	my_lcd.sendDataCont16_t(current_menu->menuTextBoxes[n]->colStart); //start
	my_lcd.sendDataCont16_t(current_menu->menuTextBoxes[n]->colEnd -1); //end
	
	//should be aliased from LCD , as soon as rotation changes this bugs out
	my_lcd.setColStart(current_menu->menuTextBoxes[n]->colStart); //set this + bounds check
	my_lcd.setColEnd(current_menu->menuTextBoxes[n]->colEnd); //set this + bounds check
	my_lcd.setColLenght(current_menu->menuTextBoxes[n]->colLenghtv);
	
	my_lcd.setRowStart(current_menu->menuTextBoxes[n]->rowStart); //set this + bounds check
	my_lcd.setRowEnd(current_menu->menuTextBoxes[n]->rowEnd); //set this + bounds check
	my_lcd.setRowLenght(current_menu->menuTextBoxes[n]->rowLenght);
	
	//set paset
	my_lcd.sendCommandCont(ILI9341CMD_Page_Address_Set);
	//set paset to start 120
	// end is max
	my_lcd.sendDataCont16_t(current_menu->menuTextBoxes[n]->rowStart); //start
	my_lcd.sendDataCont16_t(current_menu->menuTextBoxes[n]->rowEnd -1); //end
	
	my_lcd.sendEndCont();
	
	my_lcd.sendStartCont();
	my_lcd.sendCommandCont(ILI9341CMD_Memory_Write);
	
	for (uint row = 0; row < current_menu->menuTextBoxes[n]->rowLenght; ++row){
		for(uint column = 0; column < current_menu->menuTextBoxes[n]->colLenghtv; ++column){
			my_lcd.sendDataCont16_t(clearColor);
		}
	}
	
	my_lcd.sendEndCont();
	
	//cur_full_height_offset = 0; //Reset value to start cursor at 0,0 for label
	label_param[n] = 0;
}

void MenuManager::setBackgroundColor(ILI_COLORS c_){
	bg_color = c_;
}

ILI_COLORS MenuManager::getBackgroundColor(){
	return bg_color;
}

void MenuManager::setInvertedBackgroundColor(ILI_COLORS c_){
	inv_bg_color = c_;
}

ILI_COLORS MenuManager::getInvertedBackgroundColor(){
	return inv_bg_color;
}

void MenuManager::clearAfterWrite(uint n_){
	
	my_lcd.sendStartCont();

	//set caset
	my_lcd.sendCommandCont(ILI9341CMD_Column_Address_Set);
	//start is 50 and end is width - 50;
	my_lcd.sendDataCont16_t(current_menu->menuTextBoxes[n_]->colStart); //start
	my_lcd.sendDataCont16_t(current_menu->menuTextBoxes[n_]->colEnd -1); //end
	
	//should be aliased from LCD , as soon as rotation changes this bugs out
	my_lcd.setColStart(current_menu->menuTextBoxes[n_]->colStart); //set this + bounds check
	my_lcd.setColEnd(current_menu->menuTextBoxes[n_]->colEnd); //set this + bounds check
	my_lcd.setColLenght(current_menu->menuTextBoxes[n_]->colLenghtv);
	
	my_lcd.setRowStart(current_menu->menuTextBoxes[n_]->rowStart); //set this + bounds check
	my_lcd.setRowEnd(current_menu->menuTextBoxes[n_]->rowEnd); //set this + bounds check
	my_lcd.setRowLenght(current_menu->menuTextBoxes[n_]->rowLenght);
	
	//set paset
	my_lcd.sendCommandCont(ILI9341CMD_Page_Address_Set);
	//set paset to start 120
	// end is max
	my_lcd.sendDataCont16_t(current_menu->menuTextBoxes[n_]->rowStart + label_param[n_]); //start
	my_lcd.sendDataCont16_t(current_menu->menuTextBoxes[n_]->rowEnd -1); //end
	
	my_lcd.sendEndCont();
	
	my_lcd.sendStartCont();
	my_lcd.sendCommandCont(ILI9341CMD_Memory_Write);
	
	//Cursor is now positioned at top left of next line
	
	for(uint j = 0; j < (my_lcd.getRowLenght() - label_param[n_]); ++j){
		for(uint j = 0; j < my_lcd.getColLenght(); ++j){
			my_lcd.sendDataCont16_t(bg_color); //BACKGROUND FILL
		}
	}
	
	my_lcd.sendEndCont();
	
	label_param[n_] = 0;
}

void MenuManager::writeTextLabel(uint n, font curFont, std::string buffer, bool continueWrite, bool invertedColor, bool wrap){
	
		
	if(buffer[0] == '\0'){
		return;
	}

	uint curResize = curFont.height - curFont.size; // 16
	
	uint pasetOffset = 0;
	
	if(continueWrite){
		pasetOffset = label_param[n];
		cur_full_height_offset = label_param[n];
	}else{
		label_param[n] = 0;
		cur_full_height_offset = 0;
	}
		
	uint offset_height = 0;
	
	//uint fullHeightOffset = my_lcd.rowStart + pasetOffset;
	//uint fullHeightOffset = current_menu->menuTextBoxes[n]->rowStart + pasetOffset;
	uint8_t currentGlyph = buffer[0];
	uint bufferPrintOffset = 0;
	uint glyphCounter = 0;
	
	my_lcd.sendStartCont();

	//set caset
	my_lcd.sendCommandCont(ILI9341CMD_Column_Address_Set);
	//start is 50 and end is width - 50;
	my_lcd.sendDataCont16_t(current_menu->menuTextBoxes[n]->colStart); //start
	my_lcd.sendDataCont16_t(current_menu->menuTextBoxes[n]->colEnd -1); //end
	
	//should be aliased from LCD , as soon as rotation changes this bugs out
	my_lcd.setColStart(current_menu->menuTextBoxes[n]->colStart); //set this + bounds check
	my_lcd.setColEnd(current_menu->menuTextBoxes[n]->colEnd); //set this + bounds check
	my_lcd.setColLenght(current_menu->menuTextBoxes[n]->colLenghtv);
	
	my_lcd.setRowStart(current_menu->menuTextBoxes[n]->rowStart); //set this + bounds check
	my_lcd.setRowEnd(current_menu->menuTextBoxes[n]->rowEnd); //set this + bounds check
	my_lcd.setRowLenght(current_menu->menuTextBoxes[n]->rowLenght);
	
	//set paset
	my_lcd.sendCommandCont(ILI9341CMD_Page_Address_Set);
	//set paset to start 120
	// end is max
	my_lcd.sendDataCont16_t(current_menu->menuTextBoxes[n]->rowStart + pasetOffset); //start
	my_lcd.sendDataCont16_t(current_menu->menuTextBoxes[n]->rowEnd -1); //end
	
	my_lcd.sendEndCont();
	

	
	ILI_COLORS backgroundFill = bg_color;
	
	if(invertedColor){
		backgroundFill = inv_bg_color;
	}

	
	my_lcd.sendStartCont();
	my_lcd.sendCommandCont(ILI9341CMD_Memory_Write);
	
	//Helper::Debug::DebugPrint("A");	
	do{
		//Helper::Debug::DebugPrint("B");	
		offset_height = 0;
		//check if next line fits else reset line height
		if(wrap){
			if((cur_full_height_offset + curFont.height) >= my_lcd.getRowLenght()){
				//Helper::Debug::DebugPrint("\r\nTEST OFFSET\r\n");
				cur_full_height_offset = 0;
				//fullHeightOffset = 0;
				
				my_lcd.sendEndCont();
				
				my_lcd.sendStartCont();

				//set caset
				my_lcd.sendCommandCont(ILI9341CMD_Column_Address_Set);
				//start is 50 and end is width - 50;
				my_lcd.sendDataCont16_t(current_menu->menuTextBoxes[n]->colStart); //start
				my_lcd.sendDataCont16_t(current_menu->menuTextBoxes[n]->colEnd -1); //end
				
				//should be aliased from LCD , as soon as rotation changes this bugs out
				my_lcd.setColStart(current_menu->menuTextBoxes[n]->colStart); //set this + bounds check
				my_lcd.setColEnd(current_menu->menuTextBoxes[n]->colEnd); //set this + bounds check
				my_lcd.setColLenght(current_menu->menuTextBoxes[n]->colLenghtv);
				
				my_lcd.setRowStart(current_menu->menuTextBoxes[n]->rowStart); //set this + bounds check
				my_lcd.setRowEnd(current_menu->menuTextBoxes[n]->rowEnd); //set this + bounds check
				my_lcd.setRowLenght(current_menu->menuTextBoxes[n]->rowLenght);
				
				//set paset
				my_lcd.sendCommandCont(ILI9341CMD_Page_Address_Set);
				//set paset to start 120
				// end is max
				my_lcd.sendDataCont16_t(current_menu->menuTextBoxes[n]->rowStart + 0); //start
				my_lcd.sendDataCont16_t(current_menu->menuTextBoxes[n]->rowEnd -1); //end
				
				my_lcd.sendEndCont();
				
				my_lcd.sendStartCont();
				my_lcd.sendCommandCont(ILI9341CMD_Memory_Write);
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
			while(line_lenght <= my_lcd.getColLenght()){
				//Helper::Debug::DebugPrint("D");	
								
				if((currentGlyph == '\0') | (currentGlyph == '\n')){ //special chars, these chars do not get printed, \r could be supported but would need to reset all offsets
					//fill line
					//Helper::Debug::DebugPrint("E");
					for(uint k = 0; k < (my_lcd.getColLenght() - line_lenght); ++k){
						my_lcd.sendDataCont16_t(backgroundFill); //BACKGROUND FILL
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
				if(line_lenght <= (my_lcd.getColLenght() - 1 )){ //should be <= but for testing
					//Helper::Debug::DebugPrint("G");
					int col_offset = curFont.glyphs[currentGlyph]->cols;
					
					//print left blanking .left
					for(int leftC = 0; leftC < curFont.glyphs[currentGlyph]->left; ++leftC){
						//Helper::Debug::DebugPrint("H");
						//line_lenght += 1;
						my_lcd.sendDataCont16_t(backgroundFill);
					}
					//Helper::Debug::DebugPrint("I");
					for (int j = 0; j < col_offset; ++j){
						int lineCheckBackFill = (curFont.height - offset_height) - (curFont.glyphs[currentGlyph]->top + curResize);
						int lineCheckPrint = offset_height - (curFont.height - (curFont.glyphs[currentGlyph]->top + curResize));
						if(lineCheckBackFill > 0){
							my_lcd.sendDataCont16_t(backgroundFill); //BACKGROUND FILL
						}else if (lineCheckPrint < curFont.glyphs[currentGlyph]->rows){
							uint8_t grayscale = curFont.glyphs[currentGlyph]->bitmap[j + (lineCheckPrint * curFont.glyphs[currentGlyph]->cols)];
							// divide grayscale glyph by R >> 3, G >> 2, B >> 3
							
							if(invertedColor){
								grayscale = 0xFF - grayscale;
							}
							
							if(grayscale == 0xFF && invertedColor){
								my_lcd.sendDataCont16_t(inv_bg_color);
							}else{
								uint16_t RGB = ( (0xF800 & ((grayscale >> 3) << 11)) | (0x07E0 & ((grayscale >> 2) << 5)) | (0x001F & (grayscale >> 3)) );
								my_lcd.sendDataCont16_t(RGB);
								//0xF800 - R mask, 0x07E0 - G mask, 0x001F - B mask
							}
								
							

						}else{
							my_lcd.sendDataCont16_t(backgroundFill); //BACKGROUND FILL
						}
					}
					//Helper::Debug::DebugPrint("J");
					//print right blanking (.advance - (.left + .col))
					for(int leftC = 0; leftC < (curFont.glyphs[currentGlyph]->advance - (curFont.glyphs[currentGlyph]->left + curFont.glyphs[currentGlyph]->cols)); ++leftC){
						my_lcd.sendDataCont16_t(backgroundFill);
					}
					//Helper::Debug::DebugPrint("K");
				}else{
					//Helper::Debug::DebugPrint("X");
					//send empty to fill line width or fill empty space in assigned field
					for(uint k = 0; k < (my_lcd.getColLenght() - (line_lenght - (curFont.glyphs[currentGlyph]->advance)) ); ++k){
						my_lcd.sendDataCont16_t(backgroundFill); //BACKGROUND FILL
					}
					//Helper::Debug::DebugPrint("Y");
					break;
				}
				
				//Helper::Debug::DebugPrint("L");
				glyphCounter += 1;
				currentGlyph = buffer[glyphCounter + bufferPrintOffset];
			}
			offset_height += 1;
			//fullHeightOffset += 1;
			cur_full_height_offset += 1;
			
			if(cur_full_height_offset >= my_lcd.getRowEnd()){
				break; //Break immediate on end of buffer
			}
		}
		//Helper::Debug::DebugPrint("N");
		bufferPrintOffset += glyphCounter;
		
		if(cur_full_height_offset >= my_lcd.getRowEnd()){
			//cur_full_height_offset = 0;
			break; //If buffer is on last row break
		}
	} while (currentGlyph != '\0'); //while (bufferPrintOffset < lineLenghtGlyphs); // 57 < 78 can this be currentGlyph != '\0'

	
	my_lcd.sendEndCont();
	
	label_param[n] = cur_full_height_offset;

}

// default destructor
MenuManager::~MenuManager()
{
} //~MenuManager
