#include "menu.h"
#include "oled.h"
#include "def.h"
#include "game.h"
#include <inttypes.h>
#include <stdbool.h>
#include <util/delay.h>
#include <string.h>

void menu_init(){
	strcpy(menu.root.name, "Ping Pong Simulator 3000");
	menu.root.run = menu_print;
	menu.root.totalSubMenu = 2;
	menu.currentPage = &menu.root;
	
	strcpy(menu.subMenu1.name, "Play Game");
	menu.subMenu1.run = menu_func0;
	menu.subMenu1.totalSubMenu = 0;
	menu.root.subMenu[0] = &menu.subMenu1;
	
	
	strcpy(menu.subMenu2.name, "HighScores");
	menu.subMenu2.run = menu_func1;
	menu.subMenu2.totalSubMenu = 0;
	menu.subMenu2.id = 1;
	menu.root.subMenu[1] = &menu.subMenu2;
	
	
	menu.cursor = 0;
	
}

//void oled_print(const char *message, uint8_t line); // Assuming this is the OLED print function prototype

void menu_func0() { // Play Game function
    //printf("First menu selected\n");
	oled_pos(0, 0);
    oled_print("     Game in progress", 0);

    if (!DEF_GAME_ACTIVE) {
        DEF_START_GAME; // Start the game
    }
    
    char score_str[16];
    snprintf(score_str, sizeof(score_str), "Score: %u", gScore);
	oled_pos(1, 0);
    oled_print(score_str, 0);

}

void menu_func1() { // Show High Scores function
    //printf("Second menu selected\r\n");
	char highScoreStr[10];
    oled_reset();
	oled_pos(0,0);
    oled_print("HighScores", 0);
	oled_pos(1,0);
	snprintf(highScoreStr, sizeof(highScoreStr), "%u", gHighScores[0]);
	oled_print(highScoreStr, 0);
	oled_pos(2,0);
	snprintf(highScoreStr, sizeof(highScoreStr), "%u", gHighScores[1]);
	oled_print(highScoreStr, 0);
	oled_pos(3,0);
	snprintf(highScoreStr, sizeof(highScoreStr), "%u", gHighScores[2]);
	oled_print(highScoreStr, 0);

    // Display the high scores on the OLED
	/*
    for (int i = 0; i < 3; i++) {
        char score_display[16];
        snprintf(score_display, sizeof(score_display), "%d: %u", i + 1, gHighScores[i]);
        oled_print(score_display, i + 1);
    }
	*/
}

bool menu_move_back(){
	//if (menu.currentPage->parentMenu == NULL){
		//return 0;	// Already in main menu (root) do nothing
	//}
	menu.currentPage = &menu.root;	// Select parent menu of the current page.
	return 1;
}


void menu_selectPage(){
	if(menu.currentPage->totalSubMenu != 0){ // Endrer kun page hvis current page har sub pages
		menu.currentPage = menu.currentPage->subMenu[menu.cursor];
	}
	//menu.currentPage->subMenu[menu.cursor]->run();  // run the function for the selected page
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

void menu_run(){
	menu.currentPage->run();
}

uint8_t menu_is_highscore(){
	if (menu.currentPage->id == 1){
		return 1;
	}
	return 0;
}