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
        return;
    }
    
    fprintf(save, "%d %d %d %d %d\n", settings.map_row, settings.map_column, settings.width, settings.height, settings.mines);
     
    for (int i = 0; i < settings.map_column; i++) {
        for (int j = 0; j < settings.map_row; j++) {
            int combined_value = map_matrix[i][j].mine * 1000 + map_matrix[i][j].flag * 100 + map_matrix[i][j].open * 10 + map_matrix[i][j].count_near_mines;
            fprintf(save, "%04d ", combined_value);
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
        return;
    }
    
    fscanf(save, "%d %d %d %d %d", &settings.map_row, &settings.map_column, &settings.width, &settings.height, &settings.mines);
    
    int map_row = settings.map_row;
    int map_column = settings.map_column;
    
    map_matrix = (mine_cell**)realloc(map_matrix,map_row * sizeof(mine_cell*));

    for (int i = 0; i < map_row; i++) {
        map_matrix[i] = (mine_cell*)realloc(map_matrix, map_column * sizeof(mine_cell));
    }
    
    for (int i = 0; i < map_row; i++) {
        for (int j = 0; j < map_column; j++) {
            map_matrix[i][j].mine = false;
            map_matrix[i][j].flag = false;
            map_matrix[i][j].open = false;
            map_matrix[i][j].count_near_mines = 0;
        }
    }
    
    for (int i = 0; i < map_row; i++) {
        for (int j = 0; j < map_column; j++) {
            int combined_value = 0;
            fscanf(save, "%d", &combined_value);
            
            int mine = combined_value / 1000;
            combined_value %= 1000;
            int flag = combined_value / 100;
            combined_value %= 100;
            int open = combined_value / 10;
            int count_near_mines = combined_value % 10;
            
            if (mine) { map_matrix[i][j].mine = true; }
            if (flag) { map_matrix[i][j].flag = true; }
            if (open) { map_matrix[i][j].open = true; }
            map_matrix[i][j].count_near_mines = count_near_mines;;
        }
    }
    fclose(save);
}
