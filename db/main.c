#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sqlite3.h>
#include "api.h"
#include "database.h"


int main(void) {
    
    sqlite3 *db;
    // int res = createTLETable(db);
    insertTLEData(db, "25544", "ISS (ZARYA)", "1 25544U 98067A   23201.19134634  .00010982  00000+0  20095-3 0  9997", "2 25544  51.6392 164.9972 0000314  45.9683 314.1332 15.49879208406919");
    updateTLEDataFromAPI(db);
    return 0;
}