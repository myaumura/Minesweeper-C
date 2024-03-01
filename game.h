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


// MARK: - Mine Cell struct

typedef struct {
    bool mine;
    bool flag;
    bool open;
    int count_near_mines;
} mine_cell;

// MARK: - Game difficult

typedef enum {
    EASY,
    MEDIUM,
    HARD,
    HARDCORE // this is for a fucking menace
} game_difficult;

// MARK: - Game settings

typedef struct {
    union {
        struct {
            int width;
            int height;
            int map_row;
            int map_column;
            int mines;
        } parameters;
    };
} game_settings;

void new_game(void);
bool cell_in_map(int x, int y);
void setup_matrix(void);
void new_game_matrix(void);

#endif /* game_h */
