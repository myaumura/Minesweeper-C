//
//  game.c
//  Minesweeper
//
//  Created by Kirill Gusev on 17.02.2024.
//

#include "game.h"

extern game_settings settings;

int closed_cells;
mine_cell **map_matrix;

void setup_matrix(void) {
    int map_row = settings.parameters.map_row;
    int map_column = settings.parameters.map_column;

    map_matrix = (mine_cell**)malloc(map_row * sizeof(mine_cell*));

    for (int i = 0; i < map_row; i++) {
        map_matrix[i] = (mine_cell*)malloc(map_column * sizeof(mine_cell));
    }
    
    for (int i = 0; i < map_row; i++) {
        for (int j = 0; j < map_column; j++) {
            map_matrix[i][j].mine = false;
            map_matrix[i][j].flag = false;
            map_matrix[i][j].count_near_mines = 0;
            map_matrix[i][j].open = false;
        }
    }
    new_game_matrix();
}

void new_game_matrix(void) {
    int map_row = settings.parameters.map_row;
    int map_column = settings.parameters.map_column;
    
    closed_cells = map_row * map_column;
    srand(time(NULL));
    
    for (int i = 0; i < settings.parameters.mines; i++) {
        int x = rand() % map_row; //TODO: refactor this logic, because mines not changed
        int y = rand() % map_column;
        
        if (map_matrix[x][y].mine) {
            i--;
        } else {
            map_matrix[x][y].mine = true; // установка мины
            
            for (int dx = -1; dx < 2; dx++) {
                for (int dy = -1; dy < 2; dy++) {
                    if (cell_in_map(x + dx, y + dy)) {
                        map_matrix[x + dx][y + dy].count_near_mines += 1;
                    }
                }
            }
        }
    }
}

bool cell_in_map(int x, int y) {
    return (x >= 0) && (y >= 0) && (x < settings.parameters.map_row) && (y < settings.parameters.map_column);
}
