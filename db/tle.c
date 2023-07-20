#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sqlite3.h>
#include <curl/curl.h>
#include "cJSON.h"
#include "tle.h"

// Function to create the TLE table in the database (already defined)
// ...
void createTableIfNotExists(sqlite3* db) {
    char* errMsg;
    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS tle_data ("
                                 "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "    catalog_number INTEGER NOT NULL,"
                                 "    line1 TEXT NOT NULL,"
                                 "    line2 TEXT NOT NULL"
                                 ");";

    int result = sqlite3_exec(db, createTableSQL, NULL, NULL, &errMsg);
    if (result != SQLITE_OK) {
        fprintf(stderr, "Error creating table: %s\n", errMsg);
        sqlite3_free(errMsg);
        exit(EXIT_FAILURE);
    }
}

// Function to update the TLE data in the database by making the API call and storing the TLEs
void updateTLEData(sqlite3* db) {
    CURL* curl;
    CURLcode res;
    char buffer[1024 * 1024] = {0}; // Assuming the API response is within 1MB

    // The API URL to fetch TLE data from Celestrak
    const char* url = "http://celestrak.org/NORAD/elements/gp.php?GROUP=active&FORMAT=json-pretty"; // Replace with the actual API URL

    // Initialize cURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set the cURL options
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

        // Perform the API call
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "cURL Error: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            exit(EXIT_FAILURE);
        }

        // Cleanup cURL
        curl_easy_cleanup(curl);
        curl_global_cleanup();


    // Extract and store the TLE data in the database
    cJSON* tleArray = cJSON_GetObjectItem(json, "data");
    if (tleArray != NULL && cJSON_IsArray(tleArray)) {
        int tleCount = cJSON_GetArraySize(tleArray);

        const char* insertSQL = "INSERT INTO tle_data (catalog_number, line1, line2, line3) VALUES (?, ?, ?, ?);";
        sqlite3_stmt* stmt;

        for (int i = 0; i < tleCount; i++) {
            cJSON* tleObj = cJSON_GetArrayItem(tleArray, i);
            if (tleObj != NULL && cJSON_IsObject(tleObj)) {
                cJSON* catalogNumber = cJSON_GetObjectItem(tleObj, "NORAD_CAT_ID");
                cJSON* line1 = cJSON_GetObjectItem(tleObj, "TLE_LINE1");
                cJSON* line2 = cJSON_GetObjectItem(tleObj, "TLE_LINE2");
                cJSON* line3 = cJSON_GetObjectItem(tleObj, "TLE_LINE3");

                if (cJSON_IsNumber(catalogNumber) && cJSON_IsString(line1) && cJSON_IsString(line2) && cJSON_IsString(line3)) {
                    int catalog_number = catalogNumber->valueint;
                    const char* line1Str = line1->valuestring;
                    const char* line2Str = line2->valuestring;
                    const char* line3Str = line3->valuestring;

                    // Prepare the SQL statement
                    result = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, NULL);
                    if (result != SQLITE_OK) {
                        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
                        cJSON_Delete(json);
                        exit(EXIT_FAILURE);
                    }

                    // Bind the parameters to the SQL statement
                    sqlite3_bind_int(stmt, 1, catalog_number);
                    sqlite3_bind_text(stmt, 2, line1Str, -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt, 3, line2Str, -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt, 4, line3Str, -1, SQLITE_STATIC);

                    // Execute the SQL statement
                    result = sqlite3_step(stmt);
                    if (result != SQLITE_DONE) {
                        fprintf(stderr, "Error executing statement: %s\n", sqlite3_errmsg(db));
                        cJSON_Delete(json);
                        exit(EXIT_FAILURE);
                    }

                    // Finalize the statement
                    sqlite3_finalize(stmt);
                }
            }
        }
    }

    // Delete the cJSON object after parsing
    cJSON_Delete(json);
    } 
    else {
        fprintf(stderr, "cURL initialization error.\n");
        exit(EXIT_FAILURE);
    }

}



// Function to query the TLE data using the catalog number from the database
TLE getTLEByCatalogNumber(sqlite3* db, int catalog_number) {
    TLE tle;
    char query[100];
    sprintf(query, "SELECT catalog_number, line1, line2, line3 FROM tle_data WHERE catalog_number=%d;", catalog_number);

    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        exit(EXIT_FAILURE);
    }

    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        tle.catalog_number = sqlite3_column_int(stmt, 0);
        strncpy(tle.lines[0], (const char*)sqlite3_column_text(stmt, 1), sizeof(tle.lines[0]));
        strncpy(tle.lines[1], (const char*)sqlite3_column_text(stmt, 2), sizeof(tle.lines[1]));
        strncpy(tle.lines[2], (const char*)sqlite3_column_text(stmt, 3), sizeof(tle.lines[2]));
    } else {
        // TLE not found in the database
        tle.catalog_number = -1;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    return tle;
}


// Rest of the code remains unchanged
// ...

