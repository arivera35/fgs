#ifndef API_H
#define API_H

#include <sqlite3.h>

// Function to fetch TLE data from the API and store it in the database
void updateTLEDataFromAPI(sqlite3* db);

#endif // API_H
