#include "server.h"


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include <sys/ioctl.h>	/* ioctl */

/*
void ioctl_set_msg_int(int file_desc, int* message)
{
	int ret_val;

	ret_val = ioctl(file_desc, IOCTL_SET_MSG, message);

	if (ret_val < 0) {
		printf("ioctl_set_msg failed:%d\n", ret_val);
		exit(-1);
	}
}
*/

int main(void){






/*
    int* ctrl_code = malloc(sizeof(int));
    *ctrl_code = 13;

    int file_desc;


	file_desc = open("/dev/aesdchar", 0);
        if (file_desc < 0) {
            printf("Error opening file\n");
            return -1;
        }
	ioctl_set_msg_int(file_desc, ctrl_code);

	close(file_desc);
		printf("!!!\n");


exit(0);

*/








    if (remove("/var/tmp/aesdsocketdata") == 0) {
    } else {
        perror("Error deleting file");
    }

    pid_t p = fork();

    if ( p == 0 ) {
        pmain();
    } else {

    }

}
