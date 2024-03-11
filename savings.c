//
//  savings.c
//  Minesweeper
//
//  Created by Kirill Gusev on 11.03.2024.
//

#include "savings.h"

extern mine_cell **map_matrix;
extern game_settings settings;

// MARK: - Save a game

void save_game(void) {
    FILE *save = fopen("save.txt", "w");
    
    if (save == NULL) {
        printf("Error with saving game!\n");
    }
    
    for (int i = 0; i < settings.map_row; i++) {
        for (int j = 0; j < settings.map_column; j++) {
            // [mine, flag, open, count]
            fprintf(save, "[%d, %d, %d, %d], ", map_matrix[i][j].mine,  map_matrix[i][j].flag,  map_matrix[i][j].open,  map_matrix[i][j].count_near_mines);
        }
        fprintf(save, "\n");
    }
    fclose(save);
}

// MARK: - Continue last game

void continue_game(void) {
    FILE *save = fopen("save.txt", "r");
    
    if (save == NULL) {
        printf("Error with recovery game!\n");
    }
    fclose(save);
}
