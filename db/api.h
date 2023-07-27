#ifndef API_H
#define API_H

#include <sqlite3.h>

// Function to fetch TLE data from the API and store it in the database
// void updateTLEDataFromAPI(sqlite3* db);

char *fetch_data_from_api(const char *api_url);


#endif // API_H
