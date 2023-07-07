#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mongoose.h"
#include "libgpredict.h"

// Function to fetch TLE based on the given cat_num and store it in a char array
// void set_cat_num(int cat_num, char *tle) {
//     // Fetch TLE based on cat_num and store it in tle
//     // Replace this with your actual implementation
//     sprintf(tle, "TLE for cat_num %d", cat_num);
// }

static void handle_set_cat_num(struct mg_connection *nc, struct http_message *hm) {
    // Extract the cat_num from the request URL
    char cat_num_str[10];
    sscanf(hm->uri.p, "/set_cat_num/%s", cat_num_str);

    // Convert cat_num to integer
    int cat_num = atoi(cat_num_str);

    // Create a buffer to store the TLE
    char tle[256];

    // Call the set_cat_num function
    set_cat_num(cat_num, tle);

    // Send the TLE as the HTTP response
    mg_printf(nc, "HTTP/1.1 200 OK\r\n"
               "Content-Type: text/plain\r\n"
               "Content-Length: %d\r\n"
               "\r\n"
               "%s",
               strlen(tle), tle);

    nc->flags |= MG_F_SEND_AND_CLOSE;
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    struct http_message *hm = (struct http_message *) ev_data;
    
    switch (ev) {
        case MG_EV_HTTP_REQUEST:
            if (mg_vcmp(&hm->uri, "/set_cat_num") == 0) {
                handle_set_cat_num(nc, hm);
            } else if (mg_vcmp(&hm->uri, "/get_data") == 0) {
                handle_get_data(nc, hm);
            } else if (mg_vcmp(&hm->uri, "/get_pass") == 0) {
                handle_get_pass(nc, hm);
            } else {
                mg_printf(nc, "HTTP/1.1 404 Not Found\r\n"
                            "Content-Length: 9\r\n"
                            "\r\n"
                            "Not Found");
            }
            nc->flags |= MG_F_SEND_AND_CLOSE;
            break;
        default:
            break;
    }
}