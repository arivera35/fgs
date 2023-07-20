#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <sqlite3.h>
#include "database.h"

// Callback function to write data received from the API
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, char* output) {
    size_t realsize = size * nmemb;
    strcat(output, (char*)contents);
    return realsize;
}

// Function to fetch TLE data from the API and store it in the database
void updateTLEDataFromAPI(sqlite3* db) {
    CURL* curl;
    CURLcode res;
    char* api_response = (char*)malloc(1); // Will hold the complete API response
    api_response[0] = '\0';

    // Initialize libcurl
    curl = curl_easy_init();
    if (curl) {
        // Set the API URL
        const char* api_url = "http://celestrak.org/NORAD/elements/gp.php?GROUP=active&FORMAT=tle";

        // Set up libcurl options
        curl_easy_setopt(curl, CURLOPT_URL, api_url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, api_response);

        // Perform the HTTP GET request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "Error while fetching TLE data from the API: %s\n", curl_easy_strerror(res));
        } else {
            // Process the API response and insert TLE data into the database
            char* line = strtok(api_response, "\n");
            while (line != NULL) {
                // Extract TLE information
                char catalog_number[6];
                char name[26];
                char tle_line_1[70];
                char tle_line_2[70];

                strncpy(catalog_number, line + 2, 5);
                catalog_number[5] = '\0';

                strncpy(name, line + 9, 25);
                name[25] = '\0';

                strncpy(tle_line_1, line, 69);
                tle_line_1[69] = '\0';

                line = strtok(NULL, "\n");
                if (line != NULL) {
                    strncpy(tle_line_2, line, 69);
                    tle_line_2[69] = '\0';
                } else {
                    fprintf(stderr, "Invalid API response format.\n");
                    break;
                }

                printf("ACQUIRED TLE %s\n", name);
                // Your implementation to insert the data into the database (see Step 2)
                // ...
                insertTLEData(db, catalog_number, name, tle_line_1, tle_line_2);
            }
        }

        // Clean up libcurl resources
        curl_easy_cleanup(curl);
    }

    free(api_response);
}

