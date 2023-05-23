/*
 * menuPage.h
 *
 * Created: 22-5-2023 15:04:04
 *  Author: Jordan
 */ 


#ifndef MENUPAGE_H_
#define MENUPAGE_H_

#include <stdint.h>
#include <stdio.h>
//#include "ILI9341Driver.h" rewrite to C++, if mixed error will be thrown for C++ specefic include files
//Use .cpp extension instead

struct menuTextBox{
	uint colStart;// = 50; //set this + bounds check
	uint colEnd;// = LCD.width - 50; //set this + bounds check
	uint colLenghtv;
	
	uint rowStart;// = 120; //set this + bounds check
	uint rowEnd;// = LCD.height; //set this + bounds check
	uint rowLenght;// = LCD.rowEnd - LCD.rowStart;
};

struct menu{
	char * name; // Label for this menu
	const uint bitmapLenght;
	const uint16_t * menuBitmap;
	const uint rotation;
	const struct menuTextBox **menuTextBoxes;
	uint textBoxesCount;
};

#endif /* MENUPAGE_H_ */