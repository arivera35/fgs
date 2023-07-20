#ifndef TLE_H
#define TLE_H

#include <sqlite3.h>

// Structure to hold the TLE data
typedef struct {
    int catalog_number;
    char lines[3][80];
} TLE;

// Function to update the TLE data in the database by making the API call and storing the TLEs
void updateTLEData(sqlite3* db);

// Function to query the TLE data using the catalog number from the database
TLE getTLEByCatalogNumber(sqlite3* db, int catalog_number);

#endif
