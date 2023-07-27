#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sqlite3.h>
#include "api.h"
#include "database.h"


int main(void) {
    
    printf("HEY\n");
    const char *api_url = "http://celestrak.org/NORAD/elements/gp.php?GROUP=active&FORMAT=tle";
    char *data = fetch_data_from_api(api_url);
    printf("DATA %ld", sizeof(data));
    if (data) {
        sqlite3 *db = open_database("test.db");
        if (db) {
            create_table(db);
            insert_data_into_database(db, data);

            // Close the database connection
            sqlite3_close(db);
        }

        free(data);
    }

    return 0;

}