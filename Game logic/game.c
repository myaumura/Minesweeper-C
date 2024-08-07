﻿//
//  game.c
//  Minesweeper
//
//  Created by Kirill Gusev on 17.02.2024.
//

#include "game.h"

int closed_cells;
mine_cell** map_matrix;

// MARK: - Init matrix

void setup_matrix(game_settings* settings) {
    int map_row = settings->map_row;
    int map_column = settings->map_column;
    int mines = settings->mines;

    map_matrix = (mine_cell**)malloc(map_row * sizeof(mine_cell*));

    for (int i = 0; i < map_row; i++) {
        map_matrix[i] = (mine_cell*)malloc(map_column * sizeof(mine_cell));
    }

    for (int i = 0; i < map_row; i++) {
        for (int j = 0; j < map_column; j++) {
            map_matrix[i][j].mine = false;
            map_matrix[i][j].flag = false;
            map_matrix[i][j].open = false;
            map_matrix[i][j].count_near_mines = 0;
        }
    }
    new_game_matrix(map_row, map_column, mines);
}

// MARK: - Setup a new game

void new_game_matrix(int map_row, int map_column, int mines) {

    closed_cells = map_row * map_column;
    srand((unsigned int)time(NULL));

    for (int i = 0; i < mines; i++) {
        int x = rand() % map_row;
        int y = rand() % map_column;

        if (map_matrix[x][y].mine) {
            i--;
        }
        else {
            map_matrix[x][y].mine = true;

            for (int dx = -1; dx < 2; dx++) {
                for (int dy = -1; dy < 2; dy++) {
                    if (cell_in_map(x + dx, y + dy, map_row, map_column)) {
                        map_matrix[x + dx][y + dy].count_near_mines += 1;
                    }
                }
            }
        }
    }
}

// MARK: - Check cell in matrix

bool cell_in_map(int x, int y, int map_row, int map_column) {
    return (x >= 0) && (y >= 0) && (x < map_row) && (y < map_column);
}

bool cell_is_mine(int x, int y) {
    return map_matrix[x][y].mine;
}
