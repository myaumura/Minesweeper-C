//
//  records.h
//  Minesweeper
//
//  Created by Kirill Gusev on 11.03.2024.
//
#define _CRT_SECURE_NO_WARNINGS
#ifndef records_h
#define records_h
#define MAX_RECORDS 10000

#include <stdio.h>
#include "setup_ui.h"
void save_record(void);
void read_records(void);
void display_records_callback(void);
void draw_records_window(void);


#endif /* records_h */
