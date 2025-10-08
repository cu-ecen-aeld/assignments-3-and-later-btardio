#include "serverf.h"
// # include tst

#define SHM_SIZE 331072
#define BUFFER_SIZE 300000

#define SOCKET_PORT 9000

#define FILENAME "/var/tmp/aesdsocketdata"
#define FILENAME_AESD_DEVICE "/dev/aesdchar"
#define INTERVAL_SECONDS 10
#define BUFFER_T 3000

#define APPENDWRITE

#define USE_AESD_CHAR_DEVICE


/********
 * * *
 *
 * Application is implemented using man page implementation.
 *    http://gnu.cs.utah.edu/Manuals/glibc-2.2.3/html_chapter/libc_16.html
 *
 * I did not see a need to introduce a linked list.
 *
 * I have included shared memory, a fork and a semaphore for parallelism.
 * The socket application reads in parallel but after reading it will
 * print to screen unordered unless it blocks on the file operation.
 * The results of this can be seen in stdout
 *
 * abcdefg
 * hijklmnop
 * 1234567890
 * 9876543210
 * One best book is equal to a hundred good friends, but one good friend is equal to a library
 * One best book is equal to a hundred good friends, but one good friend is equal to a library
 * One best book is equal to a hundred good friends, but one good friend is equal to a library
 * validate_multithreaded
 * test_socket_timer
 * test_socket_timer
 * If you want to shine like a sun, first burn like a sun
 * If you want to shine like a sun, first burn like a sun
 * If you want to shine like a sun, first burn like a sun
 * Never stop fighting until you arrive at your destined place - that is, the unique you
 * Never stop fighting until you arrive at your destined place - that is, the unique you
 * Never stop fighting until you arrive at your destined place - that is, the unique you
 *
 * Solving the ordering would require some more on the sender side.
 *
 * Note: comment out #define APPENDWRITE
 * 
 * * *
 ********/


// NOTE: using AF_INET is not bidirectional


extern int shmid;
extern char *shm_addr;

extern pid_t pid;

extern int shmid_bufferposition;
extern int *bufferposition;

extern struct sockaddr_in sockaddrs[FD_SETSIZE];

extern char *file_pointer_new;

extern int shmid_lastBufferPosition;

extern int* lastBufferPosition;

extern sem_t mutex;

struct entry;


size_t find_null(const char *buffer, size_t size) {
    if (buffer == NULL) {
        return -1; // Or handle as an error
    }

    // Find the last non-null character
    // The memrchr function is a GNU extension, but can be implemented manually.
    // In this example, we use a manual loop for portability.
    for (size_t i = size; i > 0; --i) {
        if (buffer[i - 1] != '\0') {
            // The first null is right after the last non-null character
            return i;
        }
    }

    // If the entire buffer is null, the first trailing null is at index 0
    return 0;
}

size_t custom_strlen(const char *str) {
	size_t length = 0;

	
	while (str[length] != '\0') {
		length++;
	}


	return length;
}

void sig_handler(int signo)
{
    if (signo == SIGINT) {
        printf("Caught signal, exiting");
        exit(0);
    }
    if (signo == SIGTERM) {
        printf("Caught signal, exiting");
        exit(0);
    }
}


void log_and_print_a(int priority, const char* fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("Errno: %d\n", errno);
    perror("Error:");
    fflush(stdout);

    va_start(args, fmt);
    vsyslog(priority, fmt, args);
    va_end(args);
}

void log_and_print(const char* fmt) {
    log_and_print_a(LOG_ERR, fmt);
}

int
ioctl_set_msg_int(int file_desc, int* message)
{
	int ret_val;

	ret_val = ioctl(file_desc, IOCTL_SET_MSG, message);

	if (ret_val < 0) {
		printf("ioctl_set_msg failed:%d\n", ret_val);
		return -1;
	}
	return 0;
}


    int
read_from_client (const int filedes, char* buffer, int nbytes)
{

    unsigned long* sss;

    char fbuffer[BUFFER_SIZE+1];
    int sbytes;

    FILE *file_pointer;

    bzero(fbuffer, BUFFER_SIZE+1);
    int instruction_seek = 0;
    unsigned long ioctl_index;
    int ioctl_int_index;
    int ioctl_offset;
    int items_read;

    if (nbytes == 1) {
        // received a ""
        free(buffer);
        return 0;
    }
    else
    {


	// if instruction is, buffer is AESDCHAR_IOCSEEKTO:<index, offset>

	printf("buffer: %s\n", buffer);
	if(nbytes > 18 && 0 == strncmp("AESDCHAR_IOCSEEKTO:", buffer, 19)) {
		instruction_seek = 1;
		printf("received AESDCHAR_IOSEEKTO\n");
		items_read = sscanf(buffer, "AESDCHAR_IOCSEEKTO:%lu,%d", &ioctl_index, &ioctl_offset);
		//sscanf(buffer, "AESDCHAR_IOCSEEKTO:%d,%
		printf("read ioctl_index: %lu and ioctl_offset: %d\n", ioctl_index, ioctl_offset);


		
		int* ctrl_code = malloc(sizeof(int));
		*ctrl_code = ioctl_index;

		int file_desc;


		file_desc = open("/dev/aesdchar", 0);
		if (file_desc < 0) {
			printf("Error opening file\n");
			return -1;
		}
		ioctl_set_msg_int(file_desc, ctrl_code);

		close(file_desc);







/*

	FILE* file = fopen(FILENAME_AESD_DEVICE, "r");
        if (file == NULL) {
            printf("Error opening file\n");
            return 1;
        }

	
	ioctl_index = 0x0000000003;
;

	sss = malloc(sizeof(unsigned long*));
	//*sss = malloc(sizeof(unsigned long));
	*sss = 0xFFFFFFFFFF;

	printf("sss %lX\n", sss);
	printf("*sss %lX\n", *sss);
//	printf("**sss %lX\n", (unsigned long*)**sss);


	ioctl(file, MY_DEVICE_SET_VALUE, sss);
//	ioctl(file, MY_DEVICE_SET_VALUE, *sss);
//	ioctl(file, MY_DEVICE_SET_VALUE, **sss);
//	free(*sss);
	free(sss);

	fclose(file);
		printf("!!!\n");
		*/
	}



#ifdef APPENDWRITE

#ifdef USE_AESD_CHAR_DEVICE
	file_pointer = fopen(FILENAME_AESD_DEVICE, "a");
#else
        file_pointer = fopen(FILENAME, "a");
#endif

        // seek to position in file corresponding with fd

        if ( file_pointer == NULL ){
            log_and_print("Error opening file.\n");
            return -1;
        }
#endif

        if ((bufferposition = shmat(shmid_bufferposition, NULL, 0)) == (int *) -1) {
            perror("shmat child");
            exit(1);
        }

        // locking mechanism needed here
        if (bufferposition[filedes] == -1){

            sem_wait(&mutex);

            if ((lastBufferPosition = shmat(shmid_lastBufferPosition, NULL, 0)) == (int *) -1) {
                perror("shmat child");
                exit(1);
            }

            bufferposition[filedes] = *lastBufferPosition + BUFFER_T;
            *lastBufferPosition = bufferposition[filedes];

            if (shmdt(lastBufferPosition) == -1) {
                perror("shmdt child");
                exit(1);
            }
            sem_post(&mutex);

        }

        //printf("filedes: %d\n", filedes);
        //printf("bufferposition[filedes]: %d\n", bufferposition[filedes]);
        //printf("nbytes: %d\n", nbytes);
        //fwrite(buffer, sizeof(char), nbytes, stdout);

        if ((shm_addr = shmat(shmid, NULL, 0)) == (char *) -1) {
            perror("shmat child");
            exit(1);
        }

        char* shmbuffptr = &shm_addr[bufferposition[filedes]];

        strncpy(shmbuffptr, buffer, nbytes);

        if (shmdt(shm_addr) == -1) {
            perror("shmdt child");
            exit(1);
        }

        bufferposition[filedes] = bufferposition[filedes] + (nbytes * sizeof(char));

#ifdef APPENDWRITE
	// write to file that is compared
        
	

	
	if (instruction_seek == 0) {
		printf("\n!!!! writing to file. !!!!\n");	
		if (fputs(buffer, file_pointer) == EOF) {
	        	perror("Error writing to file");
	        	fclose(file_pointer);
	        	return -1;
	        }
	}

        if (fclose(file_pointer) == EOF) {
            perror("Error closing the file");
            return -11;
        }

//#ifdef USE_AESD_CHAR_DEVICE
	FILE* file = fopen(FILENAME_AESD_DEVICE, "r");
//#else
//        FILE* file = fopen(FILENAME, "r");
//#endif
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }

	// almost forgot to ask - stack stack stack
	//
	


//#ifdef USE_AESD_CHAR_DEVICE
	
//	openlog("aesdsocketthread", LOG_PID, LOG_USER);

  //  unsigned long ioctl_index;
    //int ioctl_int_index;
   // int ioctl_offset;
   // int items_read;





//	ioctl_index = 0x0000000003;
//;

//	sss = malloc(sizeof(unsigned long));
	//*sss = 0x0000000003;
	//
//	*sss = 0x0000030303;

//	syslog(LOG_ERR, "sss %lX\n", sss);
//	syslog(LOG_ERR, "*sss %lX\n", *sss);

//	fprintf(debug, "!!!!!!!!!!!!!sending address"); //: %lX\n", sss);

//	ioctl(file, MY_DEVICE_SET_VALUE, sss);

//	free(sss);

//	closelog();
//	ioctl(file, MY_DEVICE_SET_VALUE, 1);
	
//	ioctl(file, MY_DEVICE_SET_VALUE, 0x0000000003);
	

//#endif

//	if ( instruction_seek == 1 ) {
//		fseek(file, ioctl_offset, SEEK_SET);
//	} 
	

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
	
	if ( instruction_seek == 1 ) {
		printf("seeking somewhere\n");
		fseek(file, ioctl_offset, SEEK_SET);
	} 


        size_t bytes_read;
      	 
	if ( instruction_seek == 1 ) { 
		bytes_read = fread(fbuffer, 1, file_size, file);
	} else {
		bytes_read = fread(fbuffer, 1, file_size, file);
	}

//        if (bytes_read != (size_t)file_size) {
//            perror("Error reading file");
//            fclose(file);
//            return 1;
//        }

        fbuffer[file_size] = '\0';

        fclose(file);
	printf("filedes: %d\n", filedes);

	int firstnullchar = find_null(fbuffer, file_size); //custom_strlen(fbuffer);

        printf("0firstnullchar: %d\n", firstnullchar);
	printf("bytes_read: %d\n", bytes_read);
	printf("file_size: %d\n", file_size);
	printf("sending: %.*s\n", file_size, fbuffer);
	sbytes = write(filedes, fbuffer, firstnullchar ); // bytes_read); //file_size);
	printf("...\n");
#else

        sbytes = write(filedes, "ACK", 3);

#endif

        if (sbytes < 0){
            perror("write");
            exit(EXIT_FAILURE);
        }

        if (shmdt(bufferposition) == -1) {
            perror("shmdt child");
            exit(1);
        }

        free(buffer);
        return 0;
    }
}




    int
make_socket (uint16_t port)
{
    int sock;
    struct sockaddr_in name;

    sock = socket (PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror ("socket");
        exit (EXIT_FAILURE);
    }

    int rcvBufferSize = 0;
    socklen_t optlen = sizeof(rcvBufferSize);

    if (getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcvBufferSize, &optlen) == -1) {
        perror("getsockopt failed");
        close(sock);
        return -1;
    }

    printf("Current receive buffer size: %d\n", rcvBufferSize);

    name.sin_family = AF_INET;
    name.sin_port = htons (port);
    name.sin_addr.s_addr = htonl (INADDR_ANY);
    if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
    {
        perror ("bind");
        exit (EXIT_FAILURE);
    }

    return sock;
}



void *safe_malloc(size_t n)
{
    void *p = malloc(n);
    if (p == NULL) {
        log_and_print("Fatal: failed to allocate bytes.\n");
        abort();
    }
    return p;
}



void append_time(void) {

    FILE *fp;
    time_t raw_time;
    struct tm *gmt_time;
    char timestamp_buf[256];

    time(&raw_time);

    // 2. Convert raw time to GMT broken-down time.
    // RFC 2822 uses GMT (UTC).
    gmt_time = gmtime(&raw_time);
    if (gmt_time == NULL) {
        perror("gmtime");
        exit(EXIT_FAILURE);
    }

    // 3. Format the GMT time as an RFC 2822 compliant string.
    // Example format: "Mon, 08 Sep 2025 04:30:00 +0000"
    strftime(timestamp_buf, 256, "%a, %d %b %Y %H:%M:%S +0000", gmt_time);

    // 4. Open the file in append mode.
    // Creates the file if it doesn't exist.
    fp = fopen(FILENAME, "a");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // 5. Append the formatted timestamp to the file.
    fprintf(fp, "timestamp:%s\n", timestamp_buf);

    // 6. Close the file to ensure the data is written.
    fclose(fp);

    // 7. Print to console for confirmation.
    //printf("Appended timestamp: %s\n", timestamp_buf);

}


void initialize() {

    // Create shared memory segments
    // IPC_PRIVATE ensures a unique key, IPC_CREAT creates if it doesn't exist
    // 0666 sets read/write permissions for owner, group, and others
    if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shmid_lastBufferPosition = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) < 0) {
        perror("shmget_lastBufferPosition");
        exit(1);
    }


    if ((shmid_bufferposition = shmget(IPC_PRIVATE, FD_SETSIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget_bufferposition");
        exit(1);
    }

    if ((lastBufferPosition = shmat(shmid_lastBufferPosition, NULL, 0)) == (int *) -1) {
        perror("shmat child");
        exit(1);
    }


    if ((bufferposition = shmat(shmid_bufferposition, NULL, 0)) == (int *) -1) {
        perror("shmat child");
        exit(1);
    }


    if (sem_init(&mutex, 0, 1) != 0) {
        perror("sem_init failed");
        exit(EXIT_FAILURE);
    }

    *lastBufferPosition = -BUFFER_T;

    memset(bufferposition, -1, FD_SETSIZE);



}

int pmain(void) {




    unsigned long* sss;
    unsigned long ioctl_index;
    int ioctl_int_index;
    int ioctl_offset;
    int items_read;

    initialize();

    if (shmdt(bufferposition) == -1) {
        perror("shmdt child");
        exit(1);
    }

    if (shmdt(lastBufferPosition) == -1) {
        perror("shmdt child");
        exit(1);
    }

    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        log_and_print("Unable to create signal handler.\n");
    }


    if (signal(SIGTERM, sig_handler) == SIG_ERR) {
        log_and_print("Unable to create signal handler.\n");
    }

    size_t s_size;

    struct in_addr my_s_addr;

    fd_set active_fd_set, read_fd_set;

    inet_pton(AF_INET, "127.0.0.1", &my_s_addr);

    int s_fd = make_socket(SOCKET_PORT);

    if (s_fd < 0) {
        log_and_print("Unable to create socket.\n");
        return -1;
    }

    int l_rval = listen(s_fd, 3);

    if ( l_rval < 0 ) {
        log_and_print("Unable to listen on port.\n");
    }

    struct sockaddr_in addr_connector;

    s_size = sizeof (addr_connector);

    /* Initialize the set of active sockets. */
    FD_ZERO (&active_fd_set);
    FD_SET (s_fd, &active_fd_set);

    int n_reads = 0;

    int status = 0;

    time_t last_execution_time = time(NULL); // Initialize with current time
    last_execution_time += 10;
    const double interval_seconds = 10.0; // Desired interval in seconds

    while (1)
    {

        time_t current_time = time(NULL);
        double elapsed_time = difftime(current_time, last_execution_time);

        if (elapsed_time >= interval_seconds) {
            append_time();
            last_execution_time = current_time; // Update last execution time
        }

        /* Block until input arrives on one or more active sockets. */
        read_fd_set = active_fd_set;
        if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
        {
            perror ("select");
            exit (EXIT_FAILURE);
        }

        int i;
        /* Service all the sockets with input pending. */
        for (i = 0; i < FD_SETSIZE; ++i)
        {
            if (FD_ISSET (i, &read_fd_set))
            {
                if (i == s_fd)
                {
                    /* Connection request on original socket. */
                    int new;
                    s_size = sizeof (addr_connector);

                    new = accept(s_fd, (struct sockaddr*) &addr_connector, (unsigned int *) &s_size); //(struct sockaddr *) &addr_connector, NULL);
                    if ( new < 0 ) {
                        log_and_print("Unable to accept.\n");

                    } else {
                        sockaddrs[new] = addr_connector;
                    }
                    FD_SET (new, &active_fd_set);
                }
                else
                {

                    n_reads = n_reads + 1;  
                    /* Data arriving on an already-connected socket. */
                    char* read_buffer = malloc(sizeof(char)*BUFFER_SIZE+1);
                    bzero(read_buffer, BUFFER_SIZE+1);
                    int nbytes = read (i, read_buffer, BUFFER_SIZE);
                    pid_t pid;

                    if (nbytes < 0)
                    {
                        // Read error.
                        perror ("read");
                        //exit (EXIT_FAILURE);
                        continue;
                    }
                    else if (nbytes == 0) {
			free(read_buffer);
                        close (i); 
                        FD_CLR (i, &active_fd_set);
                        //exit(EXIT_SUCCESS);
                        continue;

                    } else { 
                        pid = fork();

                        if ( pid < 0 ) { 
                            fprintf(stderr, "fork failed\n"); free(read_buffer); exit(EXIT_FAILURE);
                        } else if (pid == 0) {
                            pid_t pidd;
                            pidd = fork();
                            if ( pidd < 0 ) { fprintf(stderr, "fork failed\n"); free(read_buffer); exit(EXIT_FAILURE); }
                            else if (pidd == 0) {
                                read_from_client (i, read_buffer, nbytes);
                                exit(EXIT_SUCCESS);
                            } else {
				free(read_buffer);
                            }
                            exit(EXIT_SUCCESS);
                            //break;

                        } else {
			    free(read_buffer);
                            //exit(EXIT_SUCCESS);
                        }

                    }
                }
            }
        }

        wait(&status);

        if ((shm_addr = shmat(shmid, NULL, 0)) == (char *) -1) {
            perror("shmat child");
            exit(1);
        }

        //printf("\n~~~ A: ");
        fwrite(shm_addr, sizeof(char), BUFFER_T, stdout);
        //printf("\n~~~ B: ");
        fwrite(&shm_addr[BUFFER_T], sizeof(char), BUFFER_T, stdout);
        //printf("\n~~~ C: ");
        fwrite(&shm_addr[BUFFER_T + BUFFER_T], sizeof(char), BUFFER_T, stdout);
        //printf("\n~~~ D: ");
        fwrite(&shm_addr[BUFFER_T + BUFFER_T + BUFFER_T], sizeof(char), BUFFER_T, stdout);
        printf("\n\n\n");
        if (shmdt(shm_addr) == -1) {
            perror("shmdt child");
            exit(1);
        }
	

    }
}
