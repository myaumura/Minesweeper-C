//
//  setup_ui.h
//  Minesweeper
//
//  Created by Kirill Gusev on 15.02.2024.
//

#ifndef setup_ui_h
#define setup_ui_h

// Libraries for interface
#define GL_SILENCE_DEPRECATION
#define _CRT_SECURE_NO_WARNINGS

#include <GLUT/glut.h>
#include <time.h>
// Local files
#include "game.h"
#include "records.h"
#include "savings.h"

// MARK: - Interface functions

void show_mines_count(int);
void draw_mine(void);
void draw_flag(void);
void draw_gamefield(void);
void open_fields(int, int, int, int);
void show_game(mine_cell**, int, int);
void touch_to_open_cell(int, int, int, int);
void opening_cells(void);

// MARK: - Logic

game_settings setup_settings(game_difficult);

// MARK: - Screen functions

void display(void);
void reshape(int, int);

void menu(int);
void sub_menu(int);
void hint_menu(int);
void create_menu(void);
void show_menu(char);
void create_window(void);
void hintSingleMove(int row, int column);
void hintSingleMove(int row, int column);
void hintFullSolve(int row, int column);
void solveStep(int row, int column);
void timerCallback(int value);
void startFullSolve(void);
#endif /* setup_ui_h */
