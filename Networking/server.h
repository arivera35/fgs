#ifndef server_h
#define server_h

#include <sys/socket.h>
#include <netinet/in.h>

struct server {
    int domain;
    int service;
    int protocol;
    u_long interface;
    int port;
    int backlog;

    struct sockaddr_in address;

    int socket;

    void (*launch)(void);
};

struct server server_constructor(int domain, int service, u_long interface, int port, int backlog, void(*launch)(struct server *server));


#endif
