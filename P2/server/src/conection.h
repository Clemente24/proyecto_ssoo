#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "comunication.h"

//http://manpages.ubuntu.com/manpages/trusty/man7/sys_socket.h.7posix.html
//https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/types.h.html
//https://pubs.opengroup.org/onlinepubs/009695399/basedefs/netinet/in.h.html
//https://pubs.opengroup.org/onlinepubs/7908799/xns/arpainet.h.html
//https://pubs.opengroup.org/onlinepubs/7908799/xns/netdb.h.html

typedef struct players_info{
  int socket_c1;
  int score_c1;
  int lifes_c1;
  int socket_c2;
  int score_c2;
  int lifes_c2;
  int socket_c3;
  int score_c3;
  int lifes_c3;
  int socket_c4;
  int score_c4;
  int lifes_c4;
} PlayersInfo;

PlayersInfo * prepare_sockets_and_get_clients(char * IP, int port);
