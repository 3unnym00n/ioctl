#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "query_ioctl.h"

int main()
{
    do 
    {
        query_arg_t  xxx = {0};
        int fd = open("/dev/simple-driver", O_RDWR);
        if (fd < 0)
        {
            break;
        }
        printf("useraddr: %p", &xxx);
        ioctl(fd, QUERY_GET_VARIABLES, &xxx);
        printf("\nxxx.dignity:%d,\nxxx.ego:%d, \nxxx.status:%d\n", xxx.dignity, xxx.ego, xxx.status);
        close(fd);
    } while (0);
   
    return 0;

    

}

