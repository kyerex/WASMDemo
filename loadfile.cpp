#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>
#include "gsys.h"
#ifdef TGOSLINUX
#include <unistd.h>
#include <alloca.h>
#include <sys/select.h>
#endif

lwrd getfile(const char *fname,char **h)
{
	int fdx;
	struct stat stx;

    *h=NULL;

    fdx=open(fname,O_RDONLY);
	if (fdx == -1) {
		return 0;
	}
    if ( 0 != fstat(fdx,&stx)) {
    	return 0;
    }
    *h=(char *)malloc(stx.st_size+1);
    if (stx.st_size != read(fdx,*h,stx.st_size)) {
    	free((void *)*h);
        *h=NULL;
    	close(fdx);
    	return 0;
    }

    close(fdx);
    *(*h+stx.st_size)='\0';
    return stx.st_size;
}