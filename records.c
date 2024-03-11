//
//  records.c
//  Minesweeper
//
//  Created by Kirill Gusev on 11.03.2024.
//

#include "records.h"

// MARK: - Save the record

void save_record(void) {
    FILE *records = fopen("records.txt", "w");
    
    if (records == NULL) {
        printf("Error with saving a record!\n");
    }
    
    fprintf(records, "You win!\n");
    fclose(records);
}

// MARK: - Read records

void read_records(void) {
    FILE *records = fopen("records.txt", "r");
    
    if (records == NULL) {
        printf("Error with reading records!\n");
    }
    
    fclose(records);
}
