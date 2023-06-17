/* 
* MenuManager.h
*
* Created: 22-5-2023 13:48:15
* Author: Jordan
*/


#ifndef __MENUMANAGER_H__
#define __MENUMANAGER_H__


#include "ILI9341Driver.h"

#include "menuPageSplash.h"

#define MP_SPLASH menuPageSplash


enum class MenuName{
	SPLASH = 0,
	MAIN = 1,	
	//ERROR;
};

//Move to menuPageSplash -> menuPageSplash needs to be converted to c++ first
enum class MenuPageSplashLabels{
	MAINTITLE = 0,
	SUBTITLE = 1,
};

class MenuManager{
//functions
public:
	MenuManager(ILI9341Driver & LCD);
	
	int SetMenu(const menu * name, ILI_COLORS backfill);

	void WriteTextLabel(uint n, font curFont, std::string sBuf, bool contWrite = true);
	
	~MenuManager();
protected:
private:

	ILI9341Driver & myLCD;
	const menu * currentMenu;
	
	bool usedOnce = true;
	uint _cur_WriteTextLabel_fullHeightOffset = 0;

	

	MenuManager( const MenuManager &c );
	MenuManager& operator=( const MenuManager &c );

}; //MenuManager

#endif //__MENUMANAGER_H__
