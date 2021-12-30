#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct slot {
    int req;
    int req_stat;
    int req_dyna;
    int id;
} slot_t;



int main(int argc,char* argv[])
{
    int ite = 1;
    char *shm_name = argv[1];
    long sleep_ms = strtol(argv[2], NULL, 10);
    int thread_n = atoi(argv[3]);
    if ( sleep_ms < 0 || thread_n < 0 ){
	 exit(1);
    }
    if(argc!=4)
    {
        printf("Argument should be: stat_process [shm_name] [sleeptime_ms] [num_threads]\n");
	exit(1);
    }
    

    int pagesize = getpagesize();

    // Create the shared memory region.
    int shmfd = shm_open(shm_name, O_RDWR, S_IRUSR | S_IWUSR);
    if (shmfd < 0) {
        perror("shm_open");
        return 1;
    }
    // Memory map.
    slot_t *shm_ptr = mmap(NULL, pagesize, PROT_READ, MAP_SHARED, shmfd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    

    struct timespec ts;
    ts.tv_sec = sleep_ms / 1000;
    ts.tv_nsec = (sleep_ms % 1000) * 1000000;

    while(1){
	printf("%d\n", ite);
        // sleep awhile
	nanosleep(&ts, NULL);

        for (int i = 0; i < thread_n; i++){
            printf("%d : %d %d %d\n", shm_ptr[i].id, shm_ptr[i].req, shm_ptr[i].req_stat, shm_ptr[i].req_dyna);

	}
        printf("\n");

	ite++;
    }

    return 0;
}
