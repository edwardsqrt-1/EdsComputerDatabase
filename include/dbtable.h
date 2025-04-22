#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Set the output (will need a lot of memory, that's fun)
// Can't make this a local variable since otherwise it will fall out of scope
// And I don't desire to use malloc for this one
char output[32768];

// Database Table Structure
struct DBTable {
    unsigned int rows;
    unsigned int columns;
    char*** data;
};

// Constructor for a table
struct DBTable InitTable(unsigned int r, unsigned int c) {

    // Declare new table
    struct DBTable new_table;

    // Set the rows and columns
    new_table.rows = r;
    new_table.columns = c;

    // Allocate array of string arrays as rows
    new_table.data = (char***) malloc(sizeof(char**) * r);

    // Allocate string pointers to each column
    for (int row = 0; row < r; row++) new_table.data[row] = (char**) malloc(sizeof(char*) * c);

    // Return the set up table
    return new_table;

}

// Function to print a table
void PrintTable(const struct DBTable *table) {

    // Check for a table with no columns or rows
    if (table->columns == 0 || table->rows == 0) {
        sprintf(output, "<no data given>\n");
        return;
    }

    // Delimiters
    char* t_start = "| ";
    char* t_delim = " | ";
    char* t_cease = " |\n";
    char* iter = 0;

    // Get maximum widths of each piece of data in the table (exceptions made for comments, which have own widths)
    int dataWidth = 0, commentWidth = 10, tableLength;
    for (int r = 0; r < table->rows; r++) {
        for (int c = 0; c < table->columns - 1; c++){
            if (strlen(table->data[r][c]) > dataWidth) dataWidth = strlen(table->data[r][c]);
        } 
        if (strlen(table->data[r][table->columns - 1]) > commentWidth) commentWidth = strlen(table->data[r][table->columns - 1]);
    }

    // Calculate Table Length
    tableLength = (table->columns - 1) * (dataWidth + 3) + commentWidth + 4;

    // Print top delimiter
    for (int i = 0; i < tableLength; i++) {
        if (i == 0 || i == tableLength - 1) strcat(output, "+");
        else strcat(output, "-");
    }
    strcat(output, "\n");

    // Print each row
    for (int r = 0; r < table->rows; r++) {

        // Use the start code
        strcat(output, t_start);

        // Append each column and the delimiter
        for (int c = 0; c < table->columns; c++) {

            // Add column data, filler space if applicable, and delimiter
            strcat(output, table->data[r][c]);
            if (c != table->columns - 1) {
                for (int e = strlen(table->data[r][c]); e < dataWidth; e++) strcat(output, " ");
                strcat(output, t_delim);
            } else for (int e = strlen(table->data[r][c]); e < commentWidth; e++) strcat(output, " ");

        }

        // Use the end code
        strcat(output, t_cease);

    }

    // Print another delimiter to end the table
    for (int i = 0; i < tableLength; i++) {
        if (i == 0 || i == tableLength - 1) strcat(output, "+");
        else strcat(output, "-");
    }
    strcat(output, "\n");
    
}


// Function to delete a table
void FreeTable(struct DBTable *table) {

    for (int row = 0; row < table->rows; row++) free(table->data[row]);
    free(table->data);
 
}

