#include "menu.h"
#include "oled.h"
#include <inttypes.h>
#include <stdbool.h>
#include <util/delay.h>

void menu_init(){
	strcpy(menu.root.name, "Main Menu");
	menu.root.totalSubMenu = 2;
	menu.currentPage = &menu.root;
	
	strcpy(menu.subMenu1.name, "Sub menu 1");
	menu.subMenu1.run = menu_func0;
	menu.subMenu1.totalSubMenu = 0;
	menu.root.subMenu[0] = &menu.subMenu1;
	
	
	strcpy(menu.subMenu2.name, "Sub menu 2");
	menu.subMenu2.run = menu_func1;
	menu.subMenu2.totalSubMenu = 0;
	menu.root.subMenu[1] = &menu.subMenu2;
	
	menu.cursor = 0;
	
}

void menu_func0(){ // TODO: Change this func to be a "Start Game" Menu entry - Start counting score & display it
	printf("First menu selected\n");
	oled_clear_buffer();
	oled_printToBuffer("First menu selected", 0);
	oled_push_buffer();
	_delay_ms(500);
}

void menu_func1(){ // TODO: Change this func to be a "HighScore(s)" Menu entry - Show the highest score, or a list of high scores if i cba. Need to implement that too.
	printf("Second menu selected\n");
	oled_clear_buffer();
	oled_printToBuffer("Second menu selected", 0);
	oled_push_buffer();
	_delay_ms(500);
}

bool menu_move_back(){
	if (menu.currentPage->parentMenu == NULL){
		return 0;	// Already in main menu (root) do nothing
	}
	menu.currentPage = menu.currentPage->parentMenu;	// Select parent menu of the current page.
	return 1;
}


void menu_selectPage(){
	menu.currentPage->subMenu[menu.cursor]->run();  // run the function for the selected page
}


bool menu_setCursor(int8_t index){
	// check if index is out of range
	if (index < 0){
		return 0;	// do nothing
	}
	else if(index >= menu.currentPage->totalSubMenu){
		return 0;	// do nothing
	}
	menu.cursor = index;	// update cursor location
	return 1;
}


bool menu_moveCursor(int8_t direction){
	int8_t index = menu.cursor + direction;
	menu_setCursor(index);
}

/*
void menu_print(){
	oled_home();
	oled_print(menu.currentPage->name, 0);
	oled_pos(1, 0);	// newline
	
	// Loop for each sub menu in current menu
	for (int i = 0; i <= menu.currentPage->totalSubMenu; i++){
		// print cursor or space
		if (menu.cursor == i){
			oled_print("-> ", 0);
		}
		else{
			oled_print("   ", 0);
		}
		// print selected sub menu name
		oled_print(menu.currentPage->subMenu[i]->name, 0);
		
		oled_pos(i+2, 0);	// newline
	}
}
*/

void menu_print() {
    oled_clear_buffer();
    oled_printToBuffer(menu.currentPage->name, 0, 0, 0);
    
    for (int i = 0; i <= menu.currentPage->totalSubMenu; i++) {
        if (menu.cursor == i) {
            oled_printToBuffer("-> ", 0, i + 1, 0); 
        } else {
            oled_printToBuffer("   ", 0, i + 1, 0); 
        }
        oled_printToBuffer(menu.currentPage->subMenu[i]->name, 0, i + 1, 3); 
    }
    oled_push_buffer();
}
