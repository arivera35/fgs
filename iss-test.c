// #include <stdlib.h>
// #include <stdio.h>
// #include "libgpredict.h"
// //#include "mongoose.h"

// char  tle_str[3][80];
// sat_t sat;
// qth_t qth;
// tle_t tle;

// static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
//   if (ev == MG_EV_HTTP_MSG) {
//     struct mg_http_message *hm = (struct mg_http_message *) ev_data;
//     if (mg_http_match_uri(hm, "/api/hello")) {              // On /api/hello requests,
//       mg_http_reply(c, 200, "", "{%m:%d}\n",
//                     MG_ESC("status"), 1);                   // Send dynamic JSON response
//     } else {                                                // For all other URIs,
//       struct mg_http_serve_opts opts = {.root_dir = "."};   // Serve files
//       mg_http_serve_dir(c, hm, &opts);                      // From root_dir
//     }
//   }
// }

// int main () {

//     struct mg_mgr mgr;
//     mg_mgr_init(&mgr);
//     mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, NULL);
//     for(;;) mg_mgr_poll(&mgr, 1000);
//     return 0;
// }


//-----------------------------------------------------------------------------------------------
// #include <stdlib.h>
// #include <stdio.h>
// #include "libgpredict.h"

// char  tle_str[3][80];
// sat_t sat;
// qth_t qth;
// tle_t tle;

// int main () {

//     char catnr [5];
//     printf("Enter NORAD catalog number: ");
//     scanf("%s", catnr);
//     // set_cat_num(catnr, tle_str);
//     // printf("TLE line 0: %s\n", tle_str[0]);
//     // printf("TLE line 1: %s\n", tle_str[1]);
// 	  // printf("TLE line 2: %s\n", tle_str[2]);
  
//     /* ISS */
//     strcpy( tle_str[0], "ISS (ZARYA)");
//     strcpy(tle_str[1], "1 25544U 98067A   23188.51662157  .00010281  00000+0  19057-3 0  9991");
//     strcpy(tle_str[2], "2 25544  51.6406 227.7630 0000247  61.6810  99.2425 15.49589703404949");
    
//     printf("TLE line 0: %s\n", tle_str[0]);
//     printf("TLE line 1: %s\n", tle_str[1]);
// 	  printf("TLE line 2: %s\n", tle_str[2]);


//     /* NOAA 1 */
//     // strcpy( tle_str[0], "NOAA 1");
//     // strcpy(tle_str[1], "1 04793U 70106A   23188.08804067 -.00000027  00000+0  10276-3 0  9990");
//     // strcpy(tle_str[2], "2 04793 101.5015 246.3286 0031034 293.0513  95.9834 12.54016614406281");
  

//     Get_Next_Tle_Set(tle_str, &tle);
    
//     if (Get_Next_Tle_Set (tle_str, &sat.tle) == 1) {
//         printf ("\nTLE Data OK\n\n");
//     } else {
//         printf ("\nTLE NOT OK, exiting\n");
//         return 1;
//     }

//     /* Initialize Ground Station Location*/
//     qth.lat = 31.7676556;
//     qth.lon = -106.4376003;
//     qth.alt = 255;

//     // /* Initialize Satellite */
//     select_ephemeris (&sat);
//     gtk_sat_data_init_sat(&sat, &qth);

//     double curr_time;
//     struct tm cdate;
//     char buffer[26];

//     pass_t *pass = get_next_pass(&sat, &qth, 2.0);
//     Date_Time(pass->aos, &cdate);
//     strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &cdate);
//     printf("%s\n", buffer);
//     // GSList *list = get_next_passes(&sat, &qth, 5.0 , 18);
//     // GSList *current = list;
//     // struct tm cdate;
//     // char buffer[80];

//     // while (current != NULL) {
//     //     pass_t *pass = (pass_t*)current->data;

//     //     Date_Time(pass->aos, &cdate);
        
//     //     strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &cdate);

//     //     // Get time zone offset in seconds
//     //     time_t currentTime = time(NULL);
//     //     struct tm *localTime = localtime(&currentTime);
//     //     int timeZoneOffset = localTime->tm_gmtoff;

//     //     // Convert time zone offset to hours and minutes
//     //     int hours_aos = timeZoneOffset / 3600;
//     //     int minutes_aos = (timeZoneOffset % 3600) / 60;

//     //     // Append time zone offset to the formatted date and time string
//     //     sprintf(buffer + strlen(buffer), " %c%02d:%02d", (timeZoneOffset >= 0) ? '+' : '-', abs(hours_aos), abs(minutes_aos));

//     //     printf("AOS: %s\n", buffer);

//     //     Date_Time(pass->los, &cdate);
//     //     strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &cdate);

//     //     // Get time zone offset in seconds
//     //     currentTime = time(NULL);
//     //     localTime = localtime(&currentTime);
//     //     timeZoneOffset = localTime->tm_gmtoff;

//     //     // Convert time zone offset to hours and minutes
//     //     int hours_los = timeZoneOffset / 3600;
//     //     int minutes_los = (timeZoneOffset % 3600) / 60;

//     //     // Append time zone offset to the formatted date and time string
//     //     sprintf(buffer + strlen(buffer), " %c%02d:%02d", (timeZoneOffset >= 0) ? '+' : '-', abs(hours_los), abs(minutes_los));

//     //     printf("LOS: %s\n", buffer);

//     //     // float duration = abs(minutes_los) - abs(minutes_aos);
//     //     // printf("Pass duration: %f\n", duration);
//     //     // if (duration > 180) {
//     //     //     printf("Pass is less than 3 minutes\n");
//     //     // }
//     //     // else {
//     //     printf("AOS: %f\n", pass->aos);
//     //     printf("TCA: %f\n", pass->tca);
//     //     printf("LOS: %f\n", pass->los);
//     //     // }
//     //     current = current->next;
//     // }

//     while(1){
//         curr_time = get_current_daynum();
//         // /* Calculate Position for specified date */
//         predict_calc(&sat, &qth, curr_time);

//         // /* Display results */
//         printf("\nLat:       %10.12f\n", sat.ssplat);
//         printf("Lon:       %10.12f\n", sat.ssplon);
//         printf("Azimuth:    %f\n", sat.az);
//         printf("Elevation:  %f\n", sat.el);
//         printf("Range Rate:  %f\n", sat.range_rate);
//         printf("Footprint:  %f\n", sat.footprint);
//         printf("Alt:        %4.10f\n", sat.alt);
//         printf("Velocity (km/s):  %10.10f\n", sat.velo);

//         usleep(2000000);

//     }

// }
//---------------------------------------------------------------------------------------
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
    char tle[3][80];

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


int main () {

    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, "http://0.0.0.0:8000", ev_handler, NULL);
    for(;;) mg_mgr_poll(&mgr, 1000);
    return 0;
}