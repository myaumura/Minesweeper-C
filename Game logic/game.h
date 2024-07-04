//
//  game.h
//  Minesweeper
//
//  Created by Kirill Gusev on 17.02.2024.
//

#ifndef game_h
#define game_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
//#include <unistd.h>

// MARK: - Mine Cell struct

typedef struct {
    bool mine;
    bool flag;
    bool open;
    int count_near_mines;
} mine_cell;

// MARK: - Game settings

typedef struct {
    int width;
    int height;
    int map_row;
    int map_column;
    int mines;
} game_settings;

// MARK: - Game difficult

typedef enum {
    EASY,
    MEDIUM,
    HARD,
    HARDCORE 
} game_difficult;



// MARK: - Game logic

void new_game(void);
void setup_matrix(game_settings*);
void new_game_matrix(int, int, int);
bool cell_in_map(int, int, int, int);
bool cell_is_mine(int, int);

#endif /* game_h */
