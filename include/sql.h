#include "sqlite3.h"

// Connection helpers
sqlite3* db_con;
sqlite3_stmt* db_statement;

// Global Variable for Filename
static char filename[512];

int runSQL(const char* command) {
	char* error = "error\n";
	return sqlite3_exec(db_con, command, NULL, NULL, &error);
}
