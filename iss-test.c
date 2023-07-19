// #include <stdlib.h>
// #include <stdio.h>
// #include "libgpredict.h"
// //#include "mongoose.h"

// char  tle_str[3][80];
// sat_t sat;
// qth_t qth;
// tle_t tle;

// static void handle_set_cat_num(struct mg_connection *nc, struct http_message *hm) {
//     // Extract the cat_num from the request URL
//     char cat_num_str[10];
//     sscanf(hm->uri.p, "/set_cat_num/%s", cat_num_str);

//     // Convert cat_num to integer
//     int cat_num = atoi(cat_num_str);

//     // Create a buffer to store the TLE
//     char tle[3][80];

//     // Call the set_cat_num function
//     set_cat_num(cat_num, tle);

//     // Send the TLE as the HTTP response
//     mg_printf(nc, "HTTP/1.1 200 OK\r\n"
//                "Content-Type: text/plain\r\n"
//                "Content-Length: %d\r\n"
//                "\r\n"
//                "%s",
//                strlen(tle), tle);

//     nc->flags |= MG_F_SEND_AND_CLOSE;
// }

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
#include <stdlib.h>
#include <stdio.h>
#include "libgpredict.h"

int main () {

    char sat_tle[3][80];
    char catnr [5];
    sat_t sat;
    qth_t qth;
    tle_t tle;
    
    printf("Enter NORAD catalog number: ");
    scanf("%s", catnr);
    char **tle_str = get_sat_tle(catnr);
    if (tle_str != NULL) {
        // Copy the TLE lines into the tle_str array
        strcpy(sat_tle[0], tle_str[0]);
        strcpy(sat_tle[1], tle_str[1]);
        strcpy(sat_tle[2], tle_str[2]);

        printf("TLE line 0: %s\n", sat_tle[0]);
        printf("TLE line 1: %s\n", sat_tle[1]);
        printf("TLE line 2: %s\n", sat_tle[2]);

        // Free the allocated memory
        // for (int i = 0; i < 3; i++) {
        //     free(tle_str[i]);
        // }
        // free(tle_str);
    }
usleep(200000);



    // get_sat_tle(catnr, &tle_str);
    // printf("TLE line 0: %s\n", tle_str[0]);
    // printf("TLE line 1: %s", tle_str[1]);
	// printf("TLE line 2: %s\n", tle_str[2]);

  
    /* ISS */
    // strcpy( tle_str[0], "ISS (ZARYA)");
    // strcpy(tle_str[1], "1 25544U 98067A   23188.51662157  .00010281  00000+0  19057-3 0  9991");
    // strcpy(tle_str[2], "2 25544  51.6406 227.7630 0000247  61.6810  99.2425 15.49589703404949");
    
    // printf("TLE line 0: %s\n", tle_str[0]);
    // printf("TLE line 1: %s\n", tle_str[1]);
	  // printf("TLE line 2: %s\n", tle_str[2]);


    /* NOAA 1 */
    // strcpy( tle_str[0], "NOAA 1");
    // strcpy(tle_str[1], "1 04793U 70106A   23188.08804067 -.00000027  00000+0  10276-3 0  9990");
    // strcpy(tle_str[2], "2 04793 101.5015 246.3286 0031034 293.0513  95.9834 12.54016614406281");
  
    printf("ABOUT TO SET TLE\n");
    Get_Next_Tle_Set(sat_tle, &tle);
    usleep(200000);
    if (Get_Next_Tle_Set (sat_tle, &sat.tle) == 1) {
        printf ("\nTLE Data OK\n\n");
    } else {
        printf ("\nTLE NOT OK, exiting\n");
        return 1;
    }

    /* Initialize Ground Station Location*/
    qth.lat = 31.7676556;
    qth.lon = -106.4376003;
    qth.alt = 255;

    printf("ABOUT TO SELECT EPHEMERIS\n");

    // /* Initialize Satellite */
    select_ephemeris (&sat);
    gtk_sat_data_init_sat(&sat, &qth);
usleep(200000);
    double curr_time;
    struct tm cdate;
    char buffer[26];
printf("ABOUT TO GET NEXT PASS\n");
    pass_t *pass = get_next_pass(&sat, &qth, 2.0);
    usleep(300000);
    printf("GOT NEXT PASS\n");
    printf("AOS:%f \n", pass->aos);
    printf("Orbit:%d \n", pass->orbit);
    Date_Time(pass->aos, &cdate);
    // usleep(300000);
    // printf("GOT DATE TIME\n");
    // strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &cdate);
    //     printf("FORMATTED NEXT PASS\n");
    //     usleep(200000);
    // printf("%s\n", buffer);

    while(1){
        curr_time = get_current_daynum();
        // /* Calculate Position for specified date */
        predict_calc(&sat, &qth, curr_time);

        // /* Display results */
        printf("\nLat:       %10.12f\n", sat.ssplat);
        printf("Lon:       %10.12f\n", sat.ssplon);
        printf("Azimuth:    %f\n", sat.az);
        printf("Elevation:  %f\n", sat.el);
        printf("Range Rate:  %f\n", sat.range_rate);
        printf("Footprint:  %f\n", sat.footprint);
        printf("Alt:        %4.10f\n", sat.alt);
        printf("Velocity (km/s):  %10.10f\n", sat.velo);

        usleep(2000000);

    }

}
