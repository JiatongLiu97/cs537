#include "helper.h"
#include "request.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdlib.h>

// 
// server.c: A very, very simple web server
//
// To run:
//  server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//
//

int *buffer_shr;
int buffer_length;
int numfull;
int numempty;
int pagesize;
char shm_n[300];
int thread_inx = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;

typedef struct slot {
    int req;
    int req_stat;
    int req_dyna;
    int id;
} slot_t;

slot_t *shm_ptr;

void sig_handler(int signum){

  //Return type of the handler function should be void
  printf("\nInside handler function\n");
  int ret = munmap(shm_ptr, pagesize);
  if (ret != 0) {
      perror("munmap");
      exit(1);
  }
  // Delete the shared memory region.
  ret = shm_unlink(shm_n);
  if (ret != 0) {
      perror("shm_unlink");
      exit(1);
  }

  exit(0);
}

// CS537: Parse the new arguments too
void getargs(int *port,int *threads, int *buffers, char *shm_name, int argc, char *argv[])
{
  if (argc != 5) {
    fprintf(stderr, "Usage: %s <port> <threads> <buffers> <shm_name>\n", argv[0]);
    exit(1);
  }
  *port = atoi(argv[1]);
  *threads = atoi(argv[2]);
  *buffers = atoi(argv[3]);
  strcpy(shm_name, argv[4]);

  if (*port == -1 || *port == 22 || *threads < 0 || *buffers < 0){
      exit(1);
  }
}

void writeFd(int connfd)
{
    for ( int i = 0; i < buffer_length; i++ ) {
	    if (buffer_shr[i] == 0){
		    buffer_shr[i] = connfd;
		    numempty--;
		    return;
	    }
   }
   return;
}

int readFd(void)
{
    for ( int i = 0; i < buffer_length; i++ ) {
            if (buffer_shr[i] != 0){
	            int connfd;
                    connfd = buffer_shr[i];
		    buffer_shr[i] = 0;
                    numfull--;
                    return connfd;;
            }
   }
   return -1;
}

static void *consume(void *arg)
{  
    pthread_mutex_lock(&mutex);
    int index = thread_inx;
    int p_id = pthread_self();
    int req_num = 0;
    int req_sta = 0;
    int req_dyn = 0;    
    thread_inx++;
    pthread_mutex_unlock(&mutex);
    while(1){
        pthread_mutex_lock(&mutex);
        while (numfull == 0)
            pthread_cond_wait(&full, &mutex);
        int confd = readFd();
        numempty++;
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
	if (confd == -1)
	   exit(1);
        int is_stat = requestHandle(confd);
        Close(confd);
	if(is_stat == 1)
            req_sta++;
        else
            req_dyn++;
	req_num++;
        
	shm_ptr[index].id = p_id;
	shm_ptr[index].req = req_num;
	shm_ptr[index].req_stat = req_sta;
	shm_ptr[index].req_dyna = req_dyn;
    }
}

int main(int argc, char *argv[])
{
  signal(SIGINT,sig_handler); // Register signal handler

  int listenfd, connfd, port, clientlen, threads, buffers;
  char shm_name[20];
  struct sockaddr_in clientaddr;

  getargs(&port, &threads, &buffers, shm_name, argc, argv);

  int arr[buffers];
  /* initialize elements of array arr to 0 */
  for (int i = 0; i < buffers; i++ ) {
     arr[i] = 0; 
  }
  buffer_shr = arr;
  buffer_length = buffers;
  numfull = 0;
  numempty = buffers;



  //
  // CS537 (Part B): Create & initialize the shared memory region...
  //
  pagesize = getpagesize();
  strcpy(shm_n, shm_name);
  // Create the shared memory region.
  int shmfd = shm_open(shm_n, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if (shmfd < 0) {
      perror("shm_open");
      exit(1);
  }
  // Extend its size.
  int ret = ftruncate(shmfd, pagesize);
  if (ret != 0) {
      perror("ftruncate");
      exit(1);
  }
  // Memory map.
  slot_t *ptr = mmap(NULL, pagesize, PROT_WRITE, MAP_SHARED, shmfd, 0);
  if (ptr == MAP_FAILED) {
      perror("mmap");
      exit(1);
  }
  shm_ptr = ptr;// pointer to the start address of the shared memeroy 


  // 
  // CS537 (Part A): Create some threads...
  //
  pthread_t workers[threads];
  for (int i = 0; i < threads; ++i) {
        pthread_create(&workers[i], NULL, consume, NULL);
  }



  listenfd = Open_listenfd(port);
  while (1) {
    clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
    pthread_mutex_lock(&mutex);
    while (numempty == 0)
	    pthread_cond_wait(&empty, &mutex);
    writeFd(connfd);
    numfull++;
    pthread_cond_signal(&full);
    pthread_mutex_unlock(&mutex);

    // 
    // CS537 (Part A): In general, don't handle the request in the main thread.
    // Save the relevant info in a buffer and have one of the worker threads 
    // do the work. Also let the worker thread close the connection.
    // 
  }
}
