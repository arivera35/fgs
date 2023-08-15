#include <stdlib.h>
#include <stdio.h>
#include "libgpredict.h"

int set_catalog_number(const char* catalog_number) {

    //maybe create a thread every time this function is created to create a new sat_t object for each new satellite and then call get_tle_data for that particular satellite
    sat_t sat;
    int response = get_tle_data(catalog_number, sat);
    printf("%d\n", response);
    return response;
}

int get_tle_data(const char* catalog_number, sat_t sat) {
    // Call your function to fetch TLE data
    char sat_tle[3][80];
    tle_t tle;
    char **tle_str = get_sat_tle(catalog_number);
    if (tle_str != NULL) {
        // Copy the TLE lines into the tle_str array
        strcpy(sat_tle[0], tle_str[0]);
        strcpy(sat_tle[1], tle_str[1]);
        strcpy(sat_tle[2], tle_str[2]);

        printf("TLE line 0: %s\n", sat_tle[0]);
        printf("TLE line 1: %s\n", sat_tle[1]);
        printf("TLE line 2: %s\n", sat_tle[2]);

        // Free the allocated memory
        for (int i = 0; i < 3; i++) {
            free(tle_str[i]);
        }
        free(tle_str);
    }
    Get_Next_Tle_Set(sat_tle, &tle);
    if (Get_Next_Tle_Set (sat_tle, &sat.tle) == 1) {
        printf ("\nTLE Data OK\n\n");
        return 0;
    } else {
        printf ("\nTLE NOT OK, exiting\n");
        return 1;
    }

}

double calculate_aos_los(const char* catalog_number, double current_time) {
    // Call your function to calculate AOS/LOS times
}

void predict_position(const char* catalog_number, double current_time, double* lat, double* lon) {
    // Call your function to predict satellite position
}

int main(void){
    printf("hey\n");
    printf("%d\n", set_catalog_number("25544"));
}