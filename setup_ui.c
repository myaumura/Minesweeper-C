//
//  setup_ui.c
//  Minesweeper
//
//  Created by Kirill Gusev on 15.02.2024.
//

#include "setup_ui.h"

extern mine_cell **map_matrix;
game_settings settings;
game_difficult difficult;

bool new_game_tapped = false;

// MARK: - Func to show mines count

void show_mines_count(int a) {
    glLineWidth(3);
    glColor3f(1, 1, 0);
    glBegin(GL_LINES);
    
    if ((a != 1) && (a != 4)) {
        glVertex2f(0.3, 0.85);
        glVertex2f(0.7, 0.85);
    }
    if ((a != 0) && (a != 1) && (a != 7)) {
        glVertex2f(0.3, 0.5);
        glVertex2f(0.7, 0.5);
    }
    if ((a != 1) && (a != 4) && (a != 7)) {
        glVertex2f(0.3, 0.15);
        glVertex2f(0.7, 0.15);
    }
    
    if ((a != 5) && (a != 6)) {
        glVertex2f(0.7, 0.5);
        glVertex2f(0.7, 0.85);
    }
    if (a != 2) {
        glVertex2f(0.7, 0.5);
        glVertex2f(0.7, 0.15);
    }
    
    if ((a != 1) && (a != 2) && (a != 3) && (a != 7)) {
        glVertex2f(0.3, 0.5);
        glVertex2f(0.3, 0.85);
    }
    if ((a == 0) || (a == 2) || (a == 6) || (a == 8)) {
        glVertex2f(0.3, 0.5);
        glVertex2f(0.3, 0.15);
    }
    glEnd();
}

// MARK: - Draw a mine

void draw_mine(void) { // TODO: Redraw a mine
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0, 0, 0);
    glVertex2f(0.3, 0.3);
    glVertex2f(0.7, 0.3);
    glVertex2f(0.7, 0.7);
    glVertex2f(0.3, 0.7);
    glEnd();
}

// MARK: - Draw a flag

void draw_flag(void) {
    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0);
    glVertex2f(0.25, 0.75);
    glVertex2f(0.85, 0.5);
    glVertex2f(0.25, 0.25);
    glEnd();
    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex2f(0.25, 0.75);
    glVertex2f(0.25, 0);
    glEnd();
}

// MARK: - Draw a gamefield

void draw_gamefield(void) {
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.8, 0.8, 0.8);
    glVertex2f(0, 1);
    glColor3f(0.7, 0.7, 0.7);
    glVertex2f(1, 1);
    glVertex2f(0, 0);
    glColor3f(0.6, 0.6, 0.6);
    glVertex2f(1, 0);
    glEnd();
}

// MARK: - Open a single cell

void draw_open_gamefield(void) {
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.3, 0.7, 0.3);
    glVertex2f(0, 1);
    glColor3f(0.3, 0.6, 0.3);
    glVertex2f(1, 1);
    glVertex2f(0, 0);
    glColor3f(0.3, 0.5, 0.3);
    glVertex2f(1, 0);
    glEnd();
}

// MARK: - Get mouse click to open a cell

void touch_to_open_cell(int button, int state, int x, int y) {
    int map_row = settings.parameters.map_row;
    int map_column = settings.parameters.map_column;
    // Convert mouse coordinates to cell coordinates (assuming each cell is 10x10 pixels)
    int cell_x = x / (float)glutGet(GLUT_WINDOW_WIDTH) * map_row;
    int cell_y = map_row - y / (float)glutGet(GLUT_WINDOW_HEIGHT) * map_column; // Invert y-coordinate because OpenGL origin is at bottom-left
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        
        if (cell_in_map(cell_x, cell_y)) {
            if (map_matrix[cell_x][cell_y].flag == false) {
                map_matrix[cell_x][cell_y].open = true;
                if (map_matrix[cell_x][cell_y].mine == true) {
                    printf("YOU ARE A LOSER!!!\n");
                }
            }
        }
        // Print out the cell coordinates
        printf("Clicked left button on cell (%d, %d)\n", cell_x, cell_y);
       
    } else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
        if (cell_in_map(cell_x, cell_y)) { // TODO: - need to create clean flag
            if (map_matrix[cell_x][cell_y].open == false) {
                if (map_matrix[cell_x][cell_y].flag == true) {
                    map_matrix[cell_x][cell_y].flag = false;
                } else {
                    map_matrix[cell_x][cell_y].flag = true;
                }
            }
        }
        // Print out the cell coordinates
        printf("Clicked right button on cell (%d, %d)\n", cell_x, cell_y);
    }
    glutPostRedisplay(); // request for refresh screen
}

// MARK: - Open a cell

void opening_cells(void) {
    glutMouseFunc(touch_to_open_cell);
}

// MARK: - Show a game

void show_game(void) {
    int map_row = settings.parameters.map_row;
    int map_column = settings.parameters.map_column;
    glLoadIdentity();
    glScalef(2.0 / map_row, 2.0 / map_column, 1); // scale of cell
    glTranslatef(-map_row * 0.5, -map_column * 0.5, 0); // set to bottom left anchor
    
    for (int i = 0; i < map_column; i++) {
        for (int j = 0; j < map_row; j++) {
            glPushMatrix();
            glTranslated(i, j, 0);
            if (map_matrix[i][j].open == true) {
                draw_open_gamefield();
                if (map_matrix[i][j].mine == true) {
                    draw_mine();
                } else if (map_matrix[i][j].count_near_mines > 0) {
                    show_mines_count(map_matrix[i][j].count_near_mines);
                }
            } else {
                draw_gamefield();
                if (map_matrix[i][j].flag == true) {
                    draw_flag();
                } else {
                    draw_gamefield();
                }
            }
            glPopMatrix();
        }
    }
}

// MARK: - Scene lifecycle

void display(void) {
    glClearColor(0.07f, 0.13f, 0.17f, 0.0f); // set background color
    glClear(GL_COLOR_BUFFER_BIT); //clear that color
    glFlush();
    
    if (new_game_tapped == true) {
        show_game();
        glFlush();
        glutSwapBuffers();
    }
    
}
    
// MARK: - Resize window

void reshape(int width, int height) {

}

// MARK: - Menu characterictics

void menu(int value) {
    switch (value) {
        case 1:
            printf("Continue button tapped\n");
            glColor3f(0.0, 1.0, 0.0);
            break;
        case 2:
            printf("Records button tapped\n");
            glColor3f(0.0, 0.0, 1.0);
            break;
        case 3:
            printf("Exit button tapped\n");
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void sub_menu(int value) {
    switch (value) {
        case 0:
            difficult = EASY;
            printf("EASY\n");
            break;
        case 1:
            difficult = MEDIUM;
            printf("MEDIUM\n");
            break;
        case 2:
            difficult = HARD;
            printf("HARD\n");
            break;
        case 3:
            difficult = HARDCORE;
            printf("HARDCORE\n");
            break;
    }
    game_settings settings = setup_settings(difficult);
    
    // Изменение размеров окна
    glutReshapeWindow(settings.parameters.width, settings.parameters.height);
    
    setup_matrix();
    
    new_game_tapped = true;
    printf("New Game button tapped\n");

    glutPostRedisplay();
}


// MARK: - Create menu

void create_menu(void) {
    
    // setting for sub menu
    int sub = glutCreateMenu(sub_menu);
    glutAddMenuEntry("Easy", 0);
    glutAddMenuEntry("Medium", 1);
    glutAddMenuEntry("Hard", 2);
    glutAddMenuEntry("Hardcore", 3);
    glutCreateMenu(sub_menu);
    
    // main menu
    glutCreateMenu(menu);
    glutAddSubMenu("New Game", sub);
    glutAddMenuEntry("Continue Game", 5);
    glutAddMenuEntry("Records", 6);
    glutAddMenuEntry("Exit", 7);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// MARK: - Setup settings

game_settings setup_settings(game_difficult difficult) {
    game_settings settings;
    
    switch (difficult) { // TODO: Need to create how to resize map size
        case EASY:
            settings.parameters.width = 400;
            settings.parameters.height = 400;
            settings.parameters.mines = 10;
            settings.parameters.map_row = 8;
            settings.parameters.map_column = 8;
            break;
        case MEDIUM:
            settings.parameters.width = 700;
            settings.parameters.height = 700;
            settings.parameters.mines = 40;
            settings.parameters.map_row = 16;
            settings.parameters.map_column = 16;
            break;
        case HARD:
            settings.parameters.width = 1000;
            settings.parameters.height = 800;
            settings.parameters.mines = 99;
            settings.parameters.map_row = 20;
            settings.parameters.map_column = 20;
            break;
        case HARDCORE:
            settings.parameters.width = 1000;
            settings.parameters.height = 1000;
            settings.parameters.mines = 1200;
            settings.parameters.map_row = 40;
            settings.parameters.map_column = 40;
            break;
    }
    return settings;
}

// MARK: - Create a OpenGL window

void create_window(void) {
    
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    settings = setup_settings(difficult);
    
    glutInitWindowSize(settings.parameters.width, settings.parameters.height);
    glutInitWindowPosition(settings.parameters.width / 2, settings.parameters.height / 2);
    glutCreateWindow("MINESWEEPER GAME");
   
    create_menu();
    setup_matrix();
    opening_cells();
    glutDisplayFunc(display);
    glutMainLoop();
}
