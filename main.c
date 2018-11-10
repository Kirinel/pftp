#include <stdio.h>
#include <stdlib.h>
// #include <transfer.h> here use wrong
// #include <connect.h> here use wrong
// #include <netinet/in.h> we can use it in connect.c not here
// #include <netdb.h> we can use it in connect.c not here
#include <strings.h>

#include "analyze.h"
#include "connect.h"



int main(int argc, char *argv[])
{
    //Parameter
    user_info user;
    host_info host;
    if (analyzeparameter(argc, argv, &user, &host))
    {
        error("Analyze parameter error");
    }
    //Connection:
    int sockfd = tryconnect(user, host);
    if (sockfd < 0)
    {
        error("Try connect to get sockfd error");
    }

    //Transfer
    //Here should some function like
    // int transfer_result =transferfile(int sockfd, someStruct file_information)
    // then return result
    // Hint:
    //      1. fileInfo struct should contain : filename, the disk storage location
    //      2. when you use pftp -s serverdomain/gun -f /ProgramIndex, ProgramIndex is the filename in struct
    //      3. after execute2 , file should transfer to location in your computer, is the second info in struct
    int transfer_result;
    //change
    if (transfer_result < 0)
    {
        error("Transfer file error");
    }
    return 0;
}
