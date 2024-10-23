/*
* @file menu.h
* @brief Menu structures with traversal functions
*/

#ifndef MENU_H
#define MENU_H

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include <stdio.h>

/**
 * Defines menu structure as a tree with a parent, sub menu and display text
 */
struct Page{
	struct Page* parentMenu;
	struct Page* subMenu[10];
	int8_t totalSubMenu;
	char name[25]; // Display text
	void (*run)(void);  // run when menu is selected

};


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
 * Move cursor to a specific index
 */
bool menu_setCursor(int8_t index);


/**
 * Move cursor
 * @param typically -1 or 1 for up and down
 */
bool menu_moveCursor(int8_t direction);


/**
 * Test page function for page0 and page1
 */
void menu_func0();

void menu_func1();


/**  
 *	Stores all menus 
 */
struct struct_Menu{
	struct Page* currentPage;
	
	struct Page root;
	struct Page subMenu1;
	struct Page subMenu2;

	int8_t cursor; // selected page
	
	
} menu;


void menu_init();

void menu_print();



#endif /* MENU_H */