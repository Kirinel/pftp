#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netdb.h>
#include <unistd.h>

#include "analyze.h"
#include "connect.h"

pthread_mutex_t mutex;

int main(int argc, char *argv[])
{
    int parseflag = 0;
    int para_argc = 0;
    user_info *client = (user_info*)malloc(sizeof(user_info));
    user_info *server = (user_info*)malloc(sizeof(user_info));
    char* finalfile;
    analyzeparameter(argc, argv, client, server, &parseflag);
    if(parseflag) return 0;
    if(client->parafile) {
        FILE *parafile = fopen(client->parafile, "r");
        if(!parafile) {
            error("no parafile!");
            exit(3);
        }
        char para_argv[32][128];
        struct parg *pargs[128];
        pthread_mutex_init(&mutex,NULL);
        while (fscanf(parafile, "%s", para_argv[para_argc]) != EOF) {
            pargs[para_argc] = (struct parg*)malloc(sizeof(struct parg));
            pargs[para_argc]->client = (struct user_info*)malloc(sizeof(struct user_info));
            pargs[para_argc]->server = (struct user_info*)malloc(sizeof(struct user_info));
            pargs[para_argc]->server->port = 21;
            pargs[para_argc]->mutex = &mutex;
            if(client->logfile) {
                pargs[para_argc]->client->logfile = (char*)malloc(128 * sizeof(char));
                strcpy(pargs[para_argc]->client->logfile, client->logfile);
            }
            char *p = &para_argv[para_argc][6];
            char *q = strchr(p, ':');
            int i = 0;
            while (p != q) {
                pargs[para_argc]->client->name[i] = *p;
                p++;
                i++;
            }
            p++;
            q = strchr(p, '@');
            i = 0;
            while (p != q) {
                pargs[para_argc]->client->password[i] = *p;
                p++;
                i++;
            }
            p++;
            q = strchr(p, '/');
            i = 0;
            while (p != q) {
                pargs[para_argc]->server->name[i] = *p;
                p++;
                i++;
            }
            p++;
            strcpy(pargs[para_argc]->client->filename, p);
            finalfile = pargs[para_argc]->client->filename;
            para_argc++;
        }
        pthread_t tids[128];
        for(int i = 0; i < para_argc; i++) {
            pargs[i]->argc = para_argc;
            pargs[i]->num = i;
//            printf("before thread %d\n", i);
            pthread_create(&tids[i], NULL, ftpconnect, (void *)pargs[i]);
        }
        for(int i = 0; i < para_argc; i++) {
            pthread_join(tids[i], NULL);
//            printf("after thread %d\n", i);
        }
        FILE* final = fopen(finalfile, "a+");
        if(!final) {
            error("fail to open the final file");
            exit(7);
        }
        FILE* temp;
        char tempfile[128];
        char c;
        for(int i = 0; i < para_argc; i++) {
            bzero(tempfile, sizeof(tempfile));
            sprintf(tempfile, "temp_%d_%s", i, finalfile);
            temp = fopen(tempfile, "r");
            if(!temp) {
                fprintf(stderr, "fail to open the temp file: %s", tempfile);
                exit(7);
            }
            do {
                c = fgetc(temp);
                if(feof(temp)) {
                    break;
                }
                fputc(c, final);
            } while (1);
            fclose(temp);
//            printf("finish writing tempfile: %s\n",tempfile);
            remove(tempfile);
        }
    }
    else {
        struct parg *arg1 = (struct parg*)malloc(sizeof(struct parg));
        arg1->client = client;
        arg1->server = server;
        arg1->server->port = 21;
        arg1->argc = 1;
        arg1->num = 0;
        arg1->mutex = &mutex;
        ftpconnect(arg1);

    }


    return 0;
}

