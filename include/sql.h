#include "sqlite3.h"

// Connection helpers
sqlite3* db_con;
sqlite3_stmt* db_statement;

// Forward declaration of displaying the output;
void DispOutput();

// Global Variable for Filename
static char filename[512];

int runSQL(const char* command) {
	int res, i;
	char res_str[500];

	res = sqlite3_prepare_v2(db_con, command, -1, &db_statement, NULL);

	res = sqlite3_step(db_statement);

	while (res == SQLITE_ROW) {
		
		i = 0;
		while (sqlite3_column_text(db_statement, i) != NULL) {
			printf("%s\n", sqlite3_column_text(db_statement, i));
			i++;
		}
		res = sqlite3_step(db_statement);
	}

	sqlite3_finalize(db_statement);

	return 0;
}
