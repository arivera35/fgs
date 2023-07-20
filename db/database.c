#include "database.h"


// Function to create the TLE table in the database
int createTLETable(sqlite3* db) {

    char *err_msg = 0;
    
    int rc = sqlite3_open("test.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    const char* create_table_sql = "CREATE TABLE IF NOT EXISTS TLE ("
                                   "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "CATALOG_NUMBER INTEGER,"
                                   "NAME TEXT,"
                                   "TLE_LINE_1 TEXT,"
                                   "TLE_LINE_2 TEXT);";

    rc = sqlite3_exec(db, create_table_sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    printf("Table created successfully\n");
    return rc;
}

// Function to insert TLE data into the database
int insertTLEData(sqlite3* db, char* catalog_number, char* name, char* tle_line_1, char* tle_line_2) {
    
    char *err_msg = 0;
    sqlite3_stmt* stmt;

    // printf("IN INSERTTLEDATA\n");
    int rc = sqlite3_open("test.db", &db);

    const char* insert_sql = "INSERT INTO TLE (CATALOG_NUMBER, NAME, TLE_LINE_1, TLE_LINE_2) VALUES (?, ?, ?, ?);";

    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_int(stmt, 1, atoi(catalog_number));
    sqlite3_bind_text(stmt, 2, name, strlen(name), NULL);
    sqlite3_bind_text(stmt, 3, tle_line_1, strlen(tle_line_1), NULL);
    sqlite3_bind_text(stmt, 4, tle_line_2, strlen(tle_line_2), NULL);

    int step = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    // printf("FINISHED INSERTING DATA\n");
    sqlite3_close(db);
    return 0;
}

// Function to get TLE data based on the catalog number
int getTLEByCatalogNumber(sqlite3* db, const char* catalog_number, char* name, char* tle_line_1, char* tle_line_2) {
    sqlite3_stmt* stmt;
    const char* select_sql = "SELECT NAME, TLE_LINE_1, TLE_LINE_2 FROM TLE WHERE CATALOG_NUMBER = ?;";

    int result = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return result;
    }

    sqlite3_bind_int(stmt, 1, atoi(catalog_number));

    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        const unsigned char* retrieved_name = sqlite3_column_text(stmt, 0);
        const unsigned char* retrieved_tle_line_1 = sqlite3_column_text(stmt, 1);
        const unsigned char* retrieved_tle_line_2 = sqlite3_column_text(stmt, 2);

        strcpy(name, (const char*)retrieved_name);
        strcpy(tle_line_1, (const char*)retrieved_tle_line_1);
        strcpy(tle_line_2, (const char*)retrieved_tle_line_2);
    } else if (result != SQLITE_DONE) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    return result;
}


