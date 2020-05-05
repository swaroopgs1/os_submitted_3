#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include<sys/ioctl.h>

#define RD_VALUE _IOR('a','b', char*)

static char buffer[50000];

int main() {
   
    int fp = open("/dev/k_probe", O_RDONLY);
    if (fp < 0){
	perror("Failed to open the driver");
	return errno;
    }
    printf("Process Info\n");
    ioctl(fp, RD_VALUE, buffer);
    printf("%s", buffer);

    close(fp);

    /*FILE *f = fopen("pg_address_file.txt", "w");
    if (f == NULL)
    {
       printf("Error opening file!\n");
       exit(1);
    }

    fprintf(f, "%s", buffer);
    fclose(f);*/
    return 0;
}
