#include "gsys.h"
#include "Sock2.h"
#include "ServerLog.h"
#include <stdio.h>
#ifdef TGOSLINUX
#include <signal.h>
#include <sys/socket.h>        /* shutdown() */
#include <netdb.h>			/* getservent() */
#include <netinet/in.h>        /* struct sockaddr_in, INADDR_NONE */
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>        /* inet_addr() */
#include <sys/ioctl.h>        /* ioctl() */
#include <sys/times.h>              /* times() */
#include <sys/termios.h>        /* fstat() */
#include <sched.h>
#include <fcntl.h>        /* fcntl() */
#include <string.h>        /* bcopy() */
#include <errno.h>        /* errno */
#include <stdlib.h>        /* exit() */
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>        /* close(),sleep(),chdir(), select(),getopt() */
#include <sys/types.h>
#include <sys/stat.h>        /* fstat() */
#endif
#define DEBUG

#define fileroot ""

lwrd getfile(const char *fname,char **h);
ServerLog *slog = new ServerLog("");
extern int testcon(char *,Sock2 *);

int main(int argc,char *argv[])
{
    Sock2 s;
    Sock2 c;
    int port;
    int ret;
    fd_set readset;
    char buf[4096];
    char fname[256];
    char *d;
    uint32_t len;

    if (argc < 2) {
	    slog->info((char *)"Must specify port #\n");
	    return 0;
    }
    port=atoi(argv[1]);
    if (port <1024 || port > 49149) {
	slog->info ((char *)"Invalid port #\n");
	return 0;
    }
    s.open(port); // serve main port
    
    if (s.fd == INVALID_SOCKET) {
        sprintf(buf,"Unable to serve Port:%d\n",port);
        slog->error(buf);
        return 0;
    }

    sprintf(buf,"Serving Port:%d",port);
    slog->info(buf);
    #ifdef TGOSLINUX
    signal(SIGCHLD,SIG_IGN);
    #endif
    while (1) {
        FD_ZERO(&readset);
        FD_SET(s.fd,&readset);
        ret=select((int)(s.fd+1),&readset,NULL,NULL,NULL);
        if (ret == -1 && errno == EINTR) {
            continue;
        }
        if (ret == -1 || s.fd == INVALID_SOCKET) {
            slog->error((char *)"Error Ret from select\n");
            return 0;
        }
        c.open(s);
        if (c.fd == INVALID_SOCKET) {
            slog->error((char *)"open failed ??\n");
            return 0;
        }
    break;
    }
    s.close();
    c.DoHandShake();
    if (c.st != WEBSOCK) {
        slog->error((char *)"should be web page ??\n");
        c.close();
        return 0;
    }
    while (1) {
        if(Sock2::READY != c.getwait(&d,&len,-1)) {
            printf("error waiting for file name\n");
            break;
        }
        if (len >= sizeof(fname)) {
            printf("write res greater than %d\n",(int)sizeof(buf));
            break;
        }
        memcpy(fname,d,len);
        free(d);
        fname[len]='\0';
        printf("%s\n",fname);
        if (strcmp(fname,"Load File:end") == 0) break;
        if (memcmp(fname,"Load File:",10) !=0 ) break; // that is all I know how to do !!
        strcpy(buf,fileroot);
        strcat(buf,&fname[10]);
        len=getfile(buf,&d);
        if (len ==0) {
            printf("Could not open:%s\n",buf);
            continue;
        }
        if (Sock2::READY != c.put(d,len,BFLAG)) break;
        free(d);
        if(Sock2::READY != c.getwait(&d,&len,-1)) {
            printf("error waiting for ack\n");
            break;
        }
        if (len >= sizeof(fname)) {
            printf("write res greater than %d\n",(int)sizeof(fname));
            break;
        }
        memcpy(fname,d,len);
        free(d);
        if (strcmp(fname,"got it") !=0 ){
            printf("No acknowledge\n");
            break;
        }
    }
    c.close();
    return 0;
}