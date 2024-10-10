/*
* @file menu.h
* @brief Menu structures with traversal functions
*/

#ifndef MENU_H
#define MENU_H

#include <inttypes.h>

/**
 * Defines menu structure as a tree with a parent, sub menu and display text
 */
struct Page{
	int8_t* parentMenu;
	int8_t* subMenu[10];
	char name[25]; // Display text
	void run*;
};

struct Menu{
	struct Page* currentPage;
	
	// main menu
	struct Page mainMenu;
	mainMenu.name = "Main Menu";
	currentPage = &mainMenu;

	// sub menu 1
	struct Page subMenu1;
	subMenu1.name = "Sub menu 1";
	subMenu1.run = menu_func0;
	mainMenu.subMenu[0] = &subMenu1;
	
	// sub menu 2 
	struct Page subMenu2;
	subMenu2.name = "Sub menu 2";
	subMenu2.run = menu_func1;
	mainMenu.subMenu[1] = &subMenu2;	
	
	int8_t cursor = 0; // selected page
	
} theMenu;


/**
 * Move current menu to parent menu
 *
 * @return 0 if current menu is root.
 */
bool menu_moveBack();


/**
 * Select a Page based  on cursor position
 */
void menu_selectPage();


/**
 * Move cursor
 */
bool menu_moveCursor(int8_t index);

/**
 * Test page function for page0 and page1
 */
void menu_func0(){
	printf("0")
}

void menu_func1(){
	printf("1")
}


#endif /* MENU_H */