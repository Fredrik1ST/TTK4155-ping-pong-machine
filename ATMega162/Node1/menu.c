#include <inttypes.h>
#include "oled.h"


bool menu_move_back(){
	if (theMenu.currentPage->parentMenu == NULL){
		return 0;	// Already in main menu (root) do nothing
	}
	theMenu.currentPage = theMenu.currentPage->parentMenu;	// Select parent menu of the current page.
	return 1;
}


void selectPage(){
	theMenu.currentPage->subMenu[theMenu.cursor]->run;  // run the function for the selected page
	
}


bool menu_moveCursor(int8_t index){
	// check if index is out of range
	if (index < 0){
		return 0;	// do nothing
	}
	else if(index > sizeof(theMenu.currentPage->subMenu)){
		return 0;	// do nothing
	}
	theMenu.cursor = index;	// update cursor location
	return 1;
}


void menu_print(){
	oled_home();
	oled_print(theMenu.currentPage->name, 0);
	oled_pos(1, 0);	// newline
	
	// Loop for each sub menu in current menu
	for (int i = 0; i < sizeof(theMenu.currentPage->subMenu); i++){
		// print cursor or space
		if (theMenu.cursor == i){
			oled_print("-> ", 0)
		}
		else{
			oled_print("   ", 0)
		}
		// print selected sub menu name
		oled_print(theMenu.currentPage->subMenu[i]->name, 0);
		
		oled_pos(i+1, 0);	// newline
	}
}
