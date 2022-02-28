#ifndef TINY_H
#define TINY_H

#include <netinet/in.h>
#include <stdbool.h>

extern int listenfd;
extern bool noise;
/* Simplifies calls to bind(), connect(), and accept() */
typedef struct sockaddr SA;

void socket_init();

char *process(int fd, struct sockaddr_in *clientaddr);

#endif