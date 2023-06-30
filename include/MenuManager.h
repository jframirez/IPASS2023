/* 
* MenuManager.h
*
* Created: 22-5-2023 13:48:15
* Author: Jordan
*/


#ifndef __MENUMANAGER_H__
#define __MENUMANAGER_H__


#include "ILI9341Driver.h"

#include "fontem.h"

#include "menuPageSplash.h"

#define MP_SPLASH menuPageSplash


enum class MenuName{
	SPLASH = 0,
	MAIN = 1,	
	//ERROR;
};

 /**
  * Manages the menu currently displayed on a ILI9341Driver.
  */
class MenuManager{
//functions
public:

	/**
	 * Manages all options relative to the current menu that is set.
	 *
	 * \param lcd reference to ILI9341Driver.
	 */
	MenuManager(ILI9341Driver & lcd);
	
	/**
	 * Set the reference to the menu.
	 *
	 * This will also display the menu on  the lcd.
	 *
	 * \param name pointer to a const menu.
	 * \param backfill background color of current menu.
	 */
	void SetMenu(const menu * name, ILI_COLORS backfill);
	
	//int SetImage(const image * img, int xStart, int yStart){
	// per line
	// write 16 bit if xStart + lineCounter > displayWidth 
	// byteCounter + = imgXwidth
	// write image[byteCounter]
	//}
	
	/**
	 * Write text to a menu label
	 *
	 * Write a std::string to a area defined as textLabel in the menu.
	 * The label can be selected with n. If the next line needs to be fitted
	 * in the same textLabel area, contWrite needs to be set to true. This will
	 * apply a integral newline after the last line.
	 * 
	 * \param n number of textLabel in menu
	 * \param curFont use font to write text
	 * \param sBuf holds the string to write to the label
	 * \param contWrite continue writing after last WriteTextLabel
	 */
	void WriteTextLabel(uint n, font curFont, std::string sBuf, bool contWrite = false);
	
	/**
	 * Clear a label filling it with a ILI_COLORS.
	 *
	 * Write the specified colore in the text label and resets the text label offset.
	 *
	 * \param n number of textLabel in menu.
	 * \param clearColor a ILI_COLORS to clear textLabel with.
	 */
	void ClearTextLabel(uint n, ILI_COLORS clearColor = ILI_COLORS::BLACK);
	
	~MenuManager();
protected:
private:

	ILI9341Driver & myLCD;
	const menu * currentMenu;
	
	bool usedOnce = false;
	int _prevLabel = -1;
	uint _cur_WriteTextLabel_fullHeightOffset = 0;
	uint _cur_WriteTextLabel_fullHeightOffset_lastTrue = 0;

	MenuManager( const MenuManager &c );
	MenuManager& operator=( const MenuManager &c );

}; //MenuManager

#endif //__MENUMANAGER_H__
