
#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

// Function to create the TLE table in the database
int createTLETable(sqlite3* db);

// Function to insert TLE data into the database
// int insertTLEData(sqlite3* db, const char* catalog_number, const char* name, const char* tle_line_1, const char* tle_line_2);
int insertTLEData(sqlite3* db, char* catalog_number, char* name, char* tle_line_1, char* tle_line_2);
// Function to get TLE data based on the catalog number
int getTLEByCatalogNumber(sqlite3* db, const char* catalog_number, char* name, char* tle_line_1, char* tle_line_2);

#endif // DATABASE_H
