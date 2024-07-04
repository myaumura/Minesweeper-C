//
//  records.c
//  Minesweeper
//
//  Created by Kirill Gusev on 11.03.2024.
//

#include "records.h"

extern time_t elapsed_time;

unsigned int records_content[MAX_RECORDS];
unsigned int num_records = 0;



// MARK: - Save the record

void save_record(void) {
    FILE* records = fopen("records.txt", "a");     

    if (records == NULL) {
        printf("Error with saving a record!\n");
    }

    fprintf(records, "%lld\n", elapsed_time);              
    fclose(records);
}

// MARK: - Read records

void draw_records_window(void) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(-0.8, 0.0);
    float y = 0.8f;
    for (int i = 0; i < num_records; i++) {
        char buffer[100];
        sprintf(buffer, "Player: %u seconds\n", records_content[i]);
        glRasterPos2f(-0.8, y);
        for (const char* c = buffer; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
        y -= 0.1f; // перенос на одну строку
    }

    glFlush();
    glutSwapBuffers();
}

void display_records_callback(void) {
    draw_records_window();
}

void read_records(void) {
    FILE* records = fopen("records.txt", "r");

    if (records == NULL) {
        printf("Error with reading records!\n");
    }
    int i = 0;
    while (num_records < MAX_RECORDS && fscanf(records, "%u", &records_content[num_records]) == 1) {
        num_records++;
    }

    fclose(records);
    int window_width = 400;
    int window_height = 600;
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("Records");
    glutDisplayFunc(display_records_callback);
    glutMainLoop();
}
