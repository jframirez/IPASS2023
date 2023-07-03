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

/**
 * Struct that hold all info for a text box in a menu.
 */
struct menuTextBox{
	uint colStart;		/**< start of column */
	uint colEnd;		/**< end of column */
	uint colLenghtv;	/**< length of column */
	
	uint rowStart;		/**< start of row */
	uint rowEnd;		/**< end of row */
	uint rowLenght;		/**< length of row */
};

/**
 * Struct that hold all info for a menu screen.
 */
struct menu{
	char * name;					/**< C-style string with menu name */
	const uint bitmapLenght;		/**< lenght of menu bitmap */
	const uint16_t * menuBitmap;	/**< pointer to menu bitmap */
	const uint rotation;			/**< enum value ILI_ROTATION */
	const struct menuTextBox **menuTextBoxes; /**< pointer to array of pointers to menuTextBox */
	uint textBoxesCount;			/**< number of text boxes */
};

#endif /* MENUPAGE_H_ */