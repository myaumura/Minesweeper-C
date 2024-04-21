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
#include <GLUT/glut.h>

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
void show_game(mine_cell **, int, int);
void touch_to_open_cell(int, int, int, int);
void opening_cells(void);

// MARK: - Logic

game_settings setup_settings(game_difficult);

// MARK: - Screen functions

void display(void);
void reshape(int, int);
void display_win(void);
void keyboard(unsigned char, int, int);

void menu(int);
void sub_menu(int);
void hint_menu(int);
void create_menu(void);
void show_menu(char);
void create_window(void);

void on_resize(int, int);

float mine_probability(int x, int y, int map_row, int map_column, mine_cell **map_matrix);
void find_least_probable_cell(int *x, int *y, int map_row, int map_column, mine_cell **map_matrix);

void make_move(int, int, mine_cell **);
void openUntilLoss(int map_row, int map_column, mine_cell **map_matrix);
void solution(mine_cell **map_matrix, int map_row, int map_column);

#endif /* setup_ui_h */
