#include "fetch_tle.h"

/*int set_cat_num(char cat_num [], TLEData *tle){
    char buffer [1000] = CELESTRACK_URL;
    snprintf(buffer, sizeof(CELESTRACK_URL)+4, CELESTRACK_URL, cat_num);
    return get_tle(buffer, tle);
}*/

int set_cat_num(char cat_num [], char (*tle_str)[80]){
    char buffer [1000] = CELESTRACK_URL;
    snprintf(buffer, sizeof(CELESTRACK_URL)+4, CELESTRACK_URL, cat_num);
    return get_tle(buffer, tle_str);
}

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    char (*tle_str)[80] = (char (*)[80])userp;
    char *tle_string = malloc(total_size + 1);
    memcpy(tle_string, contents, total_size);
    tle_string[total_size] = '\0';

    // Extract line1 and line2 from the TLE string
    char* line0 = strtok(tle_string, "\n");
    char* line1 = strtok(NULL, "\n");
    char* line2 = strtok(NULL, "\n");

    // Store the TLE lines in the TLEData structure
    strncpy(tle_str[0], line0, 80);
    strncpy(tle_str[1], line1, 80);
    strncpy(tle_str[2], line2, 80);

    free(tle_string);

    return total_size;
/*    size_t total_size = size * nmemb;
    TLEData *tle_data = (TLEData *)userp;
    char *tle_string = malloc(total_size + 1);
    memcpy(tle_string, contents, total_size);
    tle_string[total_size] = '\0';

    // Extract line1 and line2 from the TLE string
    char* line0 = strtok(tle_string, "\n");
    char* line1 = strtok(NULL, "\n");
    char* line2 = strtok(NULL, "\n");

    // Store the TLE lines in the TLEData structure
    tle_data->line0 = strdup(line0);
    tle_data->line1 = strdup(line1);
    tle_data->line2 = strdup(line2);

    free(tle_string);

    return total_size;
*/
}

int get_tle(char url [], char (*tle_str)[80]){
    // initializing curl pointer
    CURL *curl = curl_easy_init();
    if (!curl){
        printf("Error initializing curl\n");
        return -1;
    }
    // set options
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, tle_str);
    // perform action
    CURLcode result = curl_easy_perform(curl);
    if (result > 0 | result > 0){
        printf("Error downloading data\n");
        return -1;
    }
    curl_easy_cleanup(curl);
    return 1;
} 
