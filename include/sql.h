#include "sqlite3.h"
#include "dbtable.h"

// Define maximum rows and columns of the database
#define MAX_ROWS 1024
#define MAX_COLS 20

// Connection helpers
sqlite3* db_con;
sqlite3_stmt* db_statement;

// Forward declaration of displaying the output;
void DispOutput();

// Global Variable for Filename
static char filename[512];

// Function to take a query and send output to the output array in dbtable.h
int runSQL(const char* query) {
	
	// Create result and iterators
	int res, c, r;

	// Exit immediately if SQL command is empty
	if (!strcmp(filename, "")) return -5;

	// Otherwise prepare the statement
	res = sqlite3_prepare_v2(db_con, query, -1, &db_statement, NULL);

	// If the prepare fails, exit, otherwise begin stepping through
	if (res != SQLITE_OK) return -1;
	res = sqlite3_step(db_statement);

	// Create a table of maximum and minimum possible sizes
	struct DBTable result_table;
	result_table.data = (char***) malloc(sizeof(char**) * MAX_ROWS);
	for (r = 0; r < MAX_ROWS; r++) result_table.data[r] = (char**) malloc(sizeof(char*) * MAX_COLS);
	r = 0;

	// While a new row can be found, allocate and set up each row
	while (res == SQLITE_ROW) {
		
		// Allocate row
		c = 0;
		while (sqlite3_column_text(db_statement, c) != NULL) {
			result_table.data[r][c] = (char*) malloc(strlen(sqlite3_column_text(db_statement, c)) + 1);
			strcpy(result_table.data[r][c], sqlite3_column_text(db_statement, c));
			c++;
		}
		
		// Step again and iterate
		res = sqlite3_step(db_statement);
		r++;

	}

	// Set the rows and columns
	result_table.rows = r;
	result_table.columns = c;

	// Print table
	PrintTable(&result_table);

	// Delete the statement variable
	sqlite3_finalize(db_statement);

	// Free table and exit successfully
	for (r = 0; r < result_table.rows; r++) for (c = 0; c < result_table.columns; c++) free(result_table.data[r][c]);	
	for (r = 0; r < MAX_ROWS; r++) free(result_table.data[r]);
	free(result_table.data);
	return 0;
}
