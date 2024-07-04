//
//  setup_ui.c
//  Minesweeper
//
//  Created by Kirill Gusev on 15.02.2024.
//

#include "setup_ui.h"

extern mine_cell** map_matrix;
extern int closed_cells;
game_settings settings;
game_difficult difficult;
bool win_flag = 0;
char message[100];

// MARK: - Flags

bool new_game_started = false;
bool game_continue_flag = false;
bool lose_game = false;

/*long long int*/time_t start_time;
/*long long int*/time_t end_time;
/*long long int*/time_t elapsed_time;
long long int attempt_number = 1;

// MARK: - Func to show mines count

void draw_message_window(char* message) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(-0.8, 0.0);
    int i = 0;
    while (message[i] != '\0') {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, message[i]);
        i++;
    }
            
    glFlush();
    glutSwapBuffers();
}

void display_message_callback() {
    draw_message_window(message);
}

void handle_game_end(bool win) {
    

    if (win) {
        end_time = /*clock()*/time(NULL);
        elapsed_time = end_time - start_time;
        sprintf(message, "You win! time: %lld sec. ", elapsed_time);
        save_record();
    }
    else {
        sprintf(message, "You lose!");
    }

    int window_width = 400;
    int window_height = 400;
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("Result");
    glutDisplayFunc(display_message_callback);
    glutMainLoop();

    attempt_number++;
}


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
    glBegin(GL_QUADS);
    glColor3f(0.9, 0.15, 0.15);
    glVertex2f(0.0, 0.0);
    glColor3f(0.5, 0.15, 0.15);
    glVertex2f(1.0, 0.0);
    glColor3f(0.9, 0.15, 0.15);
    glVertex2f(1.0, 1.0);
    glColor3f(1.0, 0.15, 0.15);
    glVertex2f(0.0, 1.0);
    glEnd();
    // Рисуем многоугольную мину
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0, 0, 0); // черный цвет
    glVertex2f(0.5, 0.5); // центр мины

    int sides = 8; // количество сторон многоугольника
    for (int i = 0; i <= sides; ++i) {
        float angle = i * (2 * 3.14 / sides);
        float x = 0.5 + 0.2 * cos(angle);
        float y = 0.5 + 0.2 * sin(angle);
        glVertex2f(x, y);
    }
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

// MARK: - Open a several fields

void open_fields(int x, int y, int map_row, int map_column) {
    if (!cell_in_map(x, y, map_row, map_column) || map_matrix[x][y].open || map_matrix[x][y].flag) return;

    map_matrix[x][y].open = true;
    closed_cells--;

    if (map_matrix[x][y].mine) {
        lose_game = true;
        for (int i = 0; i < map_row; i++) {
            for (int j = 0; j < map_column; j++) {
                map_matrix[i][j].open = true;
            }
        }
    }
    else if (map_matrix[x][y].count_near_mines == 0) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx != 0 || dy != 0) {
                    open_fields(x + dx, y + dy, map_row, map_column);
                }
            }
        }
    }
    glutPostRedisplay();
}


// MARK: - Get mouse click to open a cell

void touch_to_open_cell(int button, int state, int x, int y) {
    int map_row = settings.map_row;
    int map_column = settings.map_column;
    // Convert mouse coordinates to cell coordinates
    int cell_x = x / (float)glutGet(GLUT_WINDOW_WIDTH) * map_row;
    int cell_y = map_row - y / (float)glutGet(GLUT_WINDOW_HEIGHT) * map_column; // Invert y-coordinate because OpenGL origin is at bottom-left

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && cell_in_map(cell_x, cell_y, map_row, map_column) && map_matrix[cell_x][cell_y].flag == false) {
        open_fields(cell_x, cell_y, map_row, map_column);
        if (map_matrix[cell_x][cell_y].mine == true) {
            handle_game_end(false);
        }
    }
    
    else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN && cell_in_map(cell_x, cell_y, map_row, map_column)) {
        if (map_matrix[cell_x][cell_y].open == false && map_matrix[cell_x][cell_y].flag == true) {
            map_matrix[cell_x][cell_y].flag = false;
        }
        else {
            map_matrix[cell_x][cell_y].flag = true;
        }
    }
    glutPostRedisplay();
}

// MARK: - Open a cell

void opening_cells(void) {
    glutMouseFunc(touch_to_open_cell);
}

// MARK: - Show a game

void show_game(mine_cell** map_matrix, int map_row, int map_column) {
    glLoadIdentity();
    glScalef(2.0 / map_row, 2.0 / map_column, 1);
    glTranslatef(-map_row * 0.5, -map_column * 0.5, 0);

    for (int i = 0; i < map_column; i++) {
        for (int j = 0; j < map_row; j++) {
            glPushMatrix();
            glTranslated(i, j, 0);
            if (map_matrix[i][j].open == true) {
                draw_open_gamefield();
                if (map_matrix[i][j].mine == true) {
                    draw_mine();
                }
                else if (map_matrix[i][j].count_near_mines > 0) {
                    show_mines_count(map_matrix[i][j].count_near_mines);
                }
            }
            else {
                draw_gamefield();
                if (map_matrix[i][j].flag == true) {
                    draw_flag();
                }
                else {
                    draw_gamefield();
                }
            }
            glPopMatrix();
        }
    }
}

// MARK: - Scene lifecycle

void display(void) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    int map_row = settings.map_row;
    int map_column = settings.map_column;

    if (new_game_started == true) {
        //start_time = clock();
        lose_game = false;
        if (closed_cells == settings.mines) {
            handle_game_end(true);
        }
        show_game(map_matrix, map_row, map_column);
        glFlush();
        glutSwapBuffers();
    }
    else if (game_continue_flag == true) {
        lose_game = false;
        //        glutReshapeFunc(on_resize);
        
        show_game(map_matrix, map_row, map_column);
        glFlush();
        glutSwapBuffers();
    }
}

// Функция рендеринга текста
void render_text(const char* text) {

    int len = strlen(text);

    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }

}
void display_about() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glColor3f(0, 0, 0);

    glRasterPos2f(-0.8, 0.7);
    render_text("The goal of the game is to uncover all empty cells without triggering any mines.");

    glRasterPos2f(-0.8, 0.5);
    render_text("To open a cell, click the left mouse button.");

    glRasterPos2f(-0.8, 0.3);
    render_text("To place a flag on a cell, click the middle mouse button.");

    glFlush();
    glutSwapBuffers();

}
void about_game() {

    // Создание окна
    int width = 800;
    int height = 300;

    glutInitWindowSize(width, height);
    glutCreateWindow("About");

    glClearColor(1, 1, 1, 1);

    // Вызов дисплея
    glutDisplayFunc(display_about);

    glutMainLoop();

}





// MARK: - Menu characterictics

void menu(int value) {
    switch (value) {
    case 4:
        continue_game();
        glutInitWindowSize(settings.width, settings.height);
        game_continue_flag = true;
        break;
    case 5: // Save game
        save_game();
        break;
    case 6:
        hintSingleMove(settings.map_row, settings.map_column);
        break; //Hint
    case 7:
        startFullSolve();
        break;
    case 8:
        read_records();
        break;
    case 9:
        exit(0);
        break;
    case 10:
        about_game();
        break;
    }
    
    glutPostRedisplay();
}

void sub_menu(int value) {
    switch (value) {
    case 0:
        difficult = EASY;
        break;
    case 1:
        difficult = MEDIUM;
        break;
    case 2:
        difficult = HARD;
        break;
    case 3:
        difficult = HARDCORE;
        break;
    }

    settings = setup_settings(difficult);
    setup_matrix(&settings);
    glutReshapeWindow(settings.width, settings.height);
    new_game_started = true;
    glutPostRedisplay();
}

// MARK: - Create menu

void create_menu(void) {

    int sub = glutCreateMenu(sub_menu);
    glutAddMenuEntry("Easy", 0);
    glutAddMenuEntry("Medium", 1);
    glutAddMenuEntry("Hard", 2);
    glutAddMenuEntry("Hardcore", 3);
    glutCreateMenu(sub_menu);

    glutCreateMenu(menu);
    glutAddSubMenu("New Game", sub);
    glutAddMenuEntry("Continue Game", 4);
    glutAddMenuEntry("Save Game", 5);
    glutAddMenuEntry("Hint", 6);
    glutAddMenuEntry("Solution", 7);
    glutAddMenuEntry("Records", 8);
    glutAddMenuEntry("Exit", 9);
    glutAddMenuEntry("About Game", 10);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// MARK: - Setup settings

game_settings setup_settings(game_difficult difficult) {
    game_settings settings;

    switch (difficult) {
    case EASY:
        settings.width = 400;
        settings.height = 400;
        settings.mines = 10;
        settings.map_row = 8;
        settings.map_column = 8;
        break;
    case MEDIUM:
        settings.width = 800;
        settings.height = 800;
        settings.mines = 40;
        settings.map_row = 16;
        settings.map_column = 16;
        break;
    case HARD:
        settings.width = 800;
        settings.height = 800;
        settings.mines = 99;
        settings.map_row = 20;
        settings.map_column = 20;
        break;
    case HARDCORE:
        settings.width = 1000;
        settings.height = 800;
        settings.mines = 288;
        settings.map_row = 24;
        settings.map_column = 24;
        break;
    }
    return settings;
}

// MARK: - Create a OpenGL window

void create_window(void) {
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    settings = setup_settings(difficult);

    glutInitWindowSize(settings.width, settings.height);
    glutCreateWindow("MINESWEEPER GAME");

    create_menu();
    setup_matrix(&settings);
    opening_cells();
    glutDisplayFunc(display);
    start_time = /*clock()*/time(NULL);
    glutMainLoop();
}

void hintSingleMove(int row, int column) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            if (map_matrix[i][j].open && map_matrix[i][j].count_near_mines > 0) {
                int flaggedCount = 0;
                int hiddenCount = 0;
                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        int ni = i + x;
                        int nj = j + y;
                        if (ni >= 0 && ni < row && nj >= 0 && nj < column) {
                            if (map_matrix[ni][nj].flag) {
                                flaggedCount++;
                            }
                            if (!map_matrix[ni][nj].open && !map_matrix[ni][nj].flag) {
                                hiddenCount++;
                            }
                        }
                    }
                }

                // Strategy 1: Mark all mines if hiddenCount equals remaining mines
                if (hiddenCount > 0 && hiddenCount == map_matrix[i][j].count_near_mines - flaggedCount) {
                    for (int x = -1; x <= 1; x++) {
                        for (int y = -1; y <= 1; y++) {
                            int ni = i + x;
                            int nj = j + y;
                            if (ni >= 0 && ni < row && nj >= 0 && nj < column && !map_matrix[ni][nj].open && !map_matrix[ni][nj].flag) {
                                map_matrix[ni][nj].flag = 1;
                                glutPostRedisplay();
                                return;
                            }
                        }
                    }
                }

                // Strategy 2: Open all safe cells if flaggedCount equals total mines
                if (flaggedCount == map_matrix[i][j].count_near_mines) {
                    for (int x = -1; x <= 1; x++) {
                        for (int y = -1; y <= 1; y++) {
                            int ni = i + x;
                            int nj = j + y;
                            if (ni >= 0 && ni < row && nj >= 0 && nj < column && !map_matrix[ni][nj].open && !map_matrix[ni][nj].flag) {
                                open_fields(ni, nj, row, column);
                                return;
                            }
                        }
                    }
                }
            }
        }
    }

    // If no obvious move is found, try a heuristic approach to make a safe move
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            if (!map_matrix[i][j].open && !map_matrix[i][j].flag) {
                // Assume this cell is safe and open it
                open_fields(i, j, row, column);
                return;
            }
        }
    }
}
void hintFullSolve(int row, int column) {
    bool moveMade = false;

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            if (map_matrix[i][j].open && map_matrix[i][j].count_near_mines > 0) {
                int flaggedCount = 0;
                int hiddenCount = 0;

                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        int ni = i + x;
                        int nj = j + y;
                        if (ni >= 0 && ni < row && nj >= 0 && nj < column) {
                            if (map_matrix[ni][nj].flag) {
                                flaggedCount++;
                            }
                            if (!map_matrix[ni][nj].open && !map_matrix[ni][nj].flag) {
                                hiddenCount++;
                            }
                        }
                    }
                }

                // Mark all mines if hiddenCount equals remaining mines
                if (hiddenCount > 0 && hiddenCount == map_matrix[i][j].count_near_mines - flaggedCount) {
                    for (int x = -1; x <= 1; x++) {
                        for (int y = -1; y <= 1; y++) {
                            int ni = i + x;
                            int nj = j + y;
                            if (ni >= 0 && ni < row && nj >= 0 && nj < column && !map_matrix[ni][nj].open && !map_matrix[ni][nj].flag) {
                                map_matrix[ni][nj].flag = 1;
                                moveMade = true;
                                glutPostRedisplay();
                                return;
                            }
                        }
                    }
                }

                // Open all safe cells if flaggedCount equals total mines
                if (flaggedCount == map_matrix[i][j].count_near_mines) {
                    for (int x = -1; x <= 1; x++) {
                        for (int y = -1; y <= 1; y++) {
                            int ni = i + x;
                            int nj = j + y;
                            if (ni >= 0 && ni < row && nj >= 0 && nj < column && !map_matrix[ni][nj].open && !map_matrix[ni][nj].flag) {
                                open_fields(ni, nj, row, column);
                                moveMade = true;
                                if (lose_game) {
                                    return;
                                }
                                glutPostRedisplay();
                                return;
                            }
                        }
                    }
                }
            }
        }
    }

    // If no obvious move is found, try a heuristic approach to make a safe move
    if (!moveMade) {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < column; j++) {
                if (!map_matrix[i][j].open && !map_matrix[i][j].flag) {
                    open_fields(i, j, row, column);
                    moveMade = true;
                    if (lose_game) {
                        return;
                    }
                    glutPostRedisplay();
                    return;
                }
            }
        }
    }

    // Check if the game is won
    if (closed_cells == settings.mines) {
        handle_game_end(true);
    }
}

void timerCallback(int value) {
    solveStep(settings.map_row, settings.map_column);

    if (!lose_game && closed_cells != settings.mines) {
        glutTimerFunc(1000, timerCallback, 0);  // Set the timer for 1 second
    }
}

void solveStep(int row, int column) {
    hintFullSolve(row, column);
}

// Call this function to start the full solve process
void startFullSolve(void) {
    glutTimerFunc(1000, timerCallback, 0);  // Start the timer with 1 second interval
}
