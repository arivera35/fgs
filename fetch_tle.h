#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <curl/curl.h>

#define CELESTRACK_URL "http://celestrak.org/NORAD/elements/gp.php?CATNR=%s&FORMAT=TLE" 
#define CELESTRACK_URL_C "http://celestrak.org/NORAD/elements/gp.php?CATNR=25544&FORMAT=TLE"

typedef struct {
    const char* line0;
    const char* line1;
    const char* line2;
} TLEData;

//int set_cat_num(char cat_num [], TLEData *tle);
int set_cat_num(char cat_num [], char tle_str[3][80]);
//int get_tle(char url [], TLEData *tle);
int get_tle(char url [], char tle_str[3][80]);