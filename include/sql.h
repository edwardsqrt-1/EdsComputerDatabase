#include "sqlite3.h"
#include "dbtable.h"

#define MAX_ROWS 1024
#define MAX_COLS 100

// Connection helpers
sqlite3* db_con;
sqlite3_stmt* db_statement;

// Forward declaration of displaying the output;
void DispOutput();

// Global Variable for Filename
static char filename[512];

int runSQL(const char* command) {
	int res, c, r;

	if (!strcmp(filename, "")) return -5;

	res = sqlite3_prepare_v2(db_con, command, -1, &db_statement, NULL);

	if (res != SQLITE_OK) return -1;

	res = sqlite3_step(db_statement);

	struct DBTable result_table;

	result_table.data = (char***) malloc(sizeof(char**) * MAX_ROWS);
	for (r = 0; r < MAX_ROWS; r++) result_table.data[r] = (char**) malloc(sizeof(char*) * MAX_COLS);
	r = 0;

	while (res == SQLITE_ROW) {
		c = 0;
		while (sqlite3_column_text(db_statement, c) != NULL) {
			result_table.data[r][c] = (char*) malloc(strlen(sqlite3_column_text(db_statement, c)) + 1);
			strcpy(result_table.data[r][c], sqlite3_column_text(db_statement, c));
			c++;
		}
		res = sqlite3_step(db_statement);
		r++;
	}

	result_table.rows = r;
	result_table.columns = c;

	PrintTable(&result_table);

	sqlite3_finalize(db_statement);

	for (r = 0; r < result_table.rows; r++) for (c = 0; c < result_table.columns; c++) free(result_table.data[r][c]);	
	for (r = 0; r < MAX_ROWS; r++) free(result_table.data[r]);
	free(result_table.data);

	return 0;
}
