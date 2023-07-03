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
	 * \param draw force a re-draw of the menu in empty state.
	 */
	void setMenu(const menu * name, ILI_COLORS backfill, bool draw = false);
	
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
	 * \param continueWrite continue writing from last print offset
	 * \param invertedColor invert print color
	 * \param wrap text that does not fit in label to the start overwriting any text there.
	 */
	void writeTextLabel(uint n, font curFont, std::string sBuf, bool continueWrite, bool invertedColor = false, bool wrap = true);
	
	/**
	 * Clear label at current line.
	 *
	 * Clear the line after any written text.
	 *
	 * \param n_ number of textLabel in menu.
	 */
	void clearAfterWrite(uint n_);
	
	/**
	 * Clear a label filling it with a ILI_COLORS.
	 *
	 * Write the specified colore in the text label and resets the text label offset.
	 *
	 * \param n_ number of textLabel in menu.
	 * \param clear_color a ILI_COLORS to clear textLabel with.
	 */
	void clearTextLabel(uint n_, ILI_COLORS clear_color = ILI_COLORS::BLACK);
	
	/**
	 * Set background color.
	 *
	 * \param c_ ILI_COLORS for background
	 */
	void setBackgroundColor(ILI_COLORS c_);
	
	/**
	 * Set inverted background color.
	 *
	 * \return c_ ILI_COLORS background
	 */
	ILI_COLORS getBackgroundColor();
	
	/**
	 * Set inverted background color.
	 *
	 * \param c_ ILI_COLORS for background
	 */
	void setInvertedBackgroundColor(ILI_COLORS c_);
	
	/**
	 * Set inverted background color.
	 *
	 * \return c_ ILI_COLORS background
	 */
	ILI_COLORS getInvertedBackgroundColor();
	
	~MenuManager();

private:

	ILI9341Driver & my_lcd;
	const menu * current_menu;

	uint cur_full_height_offset = 0;
	
	ILI_COLORS bg_color = ILI_COLORS::BLACK;
	ILI_COLORS inv_bg_color = ILI_COLORS::WHITE;
	
	std::vector<unsigned int> label_param;

	MenuManager( const MenuManager &c );
	MenuManager& operator=( const MenuManager &c );

}; //MenuManager

#endif //__MENUMANAGER_H__
