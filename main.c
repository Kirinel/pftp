#include <stdio.h>
#include <stdlib.h>
// #include <transfer.h> here use wrong
// #include <connect.h> here use wrong
// #include <netinet/in.h> we can use it in connect.c not here
// #include <netdb.h> we can use it in connect.c not here
#include <strings.h>
#include <sys/socket.h>
//#include <mach/boolean.h>
#include <pthread.h>
//#include <zconf.h>
#include <netdb.h>

#include "analyze.h"
#include "connect.h"



int main(int argc, char *argv[])
{
    //Parameter
    user_info *user = (user_info*)malloc(sizeof(user_info));
    host_info *host = (host_info*)malloc(sizeof(host_info));
    if (analyzeparameter(argc, argv, user, host))
    {
        error("Analyze parameter error");
    }
    int ctrl_sockfd = ctrlSockConnect(user, host);
    if (ctrl_sockfd < 0)
    {
        error("Try connect to get sockfd error");
    }
    int data_sockfd = dataSockConnect(user, host);

    if (data_sockfd < 0)
    {
        error("Transfer file error");
    }
    return 0;
}

