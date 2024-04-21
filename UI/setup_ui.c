//
//  setup_ui.c
//  Minesweeper
//
//  Created by Kirill Gusev on 15.02.2024.
//

#include "setup_ui.h"

extern mine_cell **map_matrix;
extern int closed_cells;
game_settings settings;
game_difficult difficult;

// MARK: - Flags

bool new_game_started = false;
bool game_continue_flag = false;
bool lose_game = false;
bool hint_tapped = false;
bool solution_tapped = false;

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
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0, 0, 0);
    glVertex2f(0.5, 0.5);
    
    int sides = 8;
    for (int i = 0; i <= sides; ++i) {
        float angle = i * (2 * M_PI / sides);
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
    if (!cell_in_map(x, y, map_row, map_column) == true || map_matrix[x][y].open == true) return;
    
    map_matrix[x][y].open = true;
    closed_cells--;
    
    if (map_matrix[x][y].count_near_mines == 0) {
        for (int dx = -1; dx < 2; dx++) {
            for (int dy = -1; dy < 2; dy++) {
                open_fields(x+dx, y+dy, map_row, map_column);
            }
        }
    } else if (map_matrix[x][y].mine == true) {
        lose_game = true;
        for (int i = 0; i < map_row; i++) {
            for (int j = 0; j < map_column; j++) {
                map_matrix[i][j].open = true;
            }
        }
    }
    glutPostRedisplay();
}

// MARK: - Get mouse click to open a cell

void touch_to_open_cell(int button, int state, int x, int y) {
    int map_row = settings.map_row;
    int map_column = settings.map_column;
    int cell_x = x / (float)glutGet(GLUT_WINDOW_WIDTH) * map_row;
    int cell_y = map_row - y / (float)glutGet(GLUT_WINDOW_HEIGHT) * map_column;
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && cell_in_map(cell_x, cell_y, map_row, map_column) && map_matrix[cell_x][cell_y].flag == false) {
        open_fields(cell_x,cell_y, map_row, map_column);
    } else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN && cell_in_map(cell_x, cell_y, map_row, map_column)) {
        if (map_matrix[cell_x][cell_y].open == false && map_matrix[cell_x][cell_y].flag == true) {
            map_matrix[cell_x][cell_y].flag = false;
        } else {
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

void show_game(mine_cell **map_matrix, int map_row, int map_column) {
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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    
    int map_row = settings.map_row;
    int map_column = settings.map_column;
    
    if (new_game_started == true) {
        lose_game = false;
        if (closed_cells == settings.mines) {
        }
        show_game(map_matrix, map_row, map_column);
        glFlush();
        glutSwapBuffers();
    } else if (game_continue_flag == true) {
        lose_game = false;
        glutReshapeFunc(on_resize);
        show_game(map_matrix, map_row, map_column);
        glFlush();
        glutSwapBuffers();
    }
}

void on_resize(int w, int h) {
    settings.width = w;
    settings.height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void display_win(void) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    char *message = "Congratulations!";
    for (int i = 0; i < strlen(message); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
    }
    glFlush();
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
            make_move(settings.map_row, settings.map_column, map_matrix);
            break;
        case 7:
//            solution();
            break;
        case 8:
            //            read_records();
            break;
        case 9:
            exit(0);
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
    glutMainLoop();
}

float mine_probability(int x, int y, int map_row, int map_column, mine_cell **map_matrix) {
    int mines_count = 0;
    int closed_count = 0;
    
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int nx = x + dx;
            int ny = y + dy;
            
            if (nx >= 0 && nx < map_row && ny >= 0 && ny < map_column && (dx != 0 || dy != 0)) {
                if (map_matrix[nx][ny].mine) {
                    mines_count++;
                }
                if (!map_matrix[nx][ny].open) {
                    closed_count++;
                }
            }
        }
    }
    
    if (closed_count > 0) {
        return (float)mines_count / closed_count;
    }
    
    return 0;
}

void find_least_probable_cell(int *x, int *y, int map_row, int map_column, mine_cell **map_matrix) {
    float min_probability = 1;
    int least_probable_cells[100][2];
    int least_probable_count = 0;
    
    for (int i = 0; i < map_row; i++) {
        for (int j = 0; j < map_column; j++) {
            if (!map_matrix[i][j].open) {
                float probability = mine_probability(i, j, map_row, map_column, map_matrix);
                if (probability < min_probability) {
                    min_probability = probability;
                    least_probable_count = 0;
                }
                if (probability == min_probability) {
                    least_probable_cells[least_probable_count][0] = i;
                    least_probable_cells[least_probable_count][1] = j;
                    least_probable_count++;
                }
            }
        }
    }
    
    if (least_probable_count > 0) {
        int index = rand() % least_probable_count;
        *x = least_probable_cells[index][0];
        *y = least_probable_cells[index][1];
    }
}

void make_move(int map_row, int map_column, mine_cell **map_matrix) {
    int x, y;
    find_least_probable_cell(&x, &y, map_row, map_column, map_matrix);
    open_fields(x, y, map_row, map_column);
    
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < map_row && ny >= 0 && ny < map_column) {
                if (!map_matrix[nx][ny].open && !map_matrix[nx][ny].flag) {
                    float probability = mine_probability(nx, ny, map_row, map_column, map_matrix);
                    if (probability > 0.5) {
                        map_matrix[nx][ny].flag = true;
                    }
                }
            }
        }
    }
    glutPostRedisplay();
}
