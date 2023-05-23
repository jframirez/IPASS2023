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
	
	int SetMenu(const menu * name);

	void WriteTextLabel(uint n, font curFont, const char * pString, ... );
	
	~MenuManager();
protected:
private:

	ILI9341Driver & myLCD;
	const menu * currentMenu;
	

	MenuManager( const MenuManager &c );
	MenuManager& operator=( const MenuManager &c );

}; //MenuManager

#endif //__MENUMANAGER_H__
