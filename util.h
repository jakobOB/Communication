#ifndef ASS5_UTIL_H
#define ASS5_UTIL_H

#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <semaphore.h>
//#include <linux/seccomp.h>
#include <linux/filter.h>
#include <linux/audit.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
//#include <seccomp.h>
#include <string.h>
#include <sys/wait.h>


//Number of numbers that have to be read
#define EXT_TESTS 1

#define SHM_NAME_REQUEST "request"
#define SHM_NAME_RESPONSE "response"
#define SHM_NAME_SEMAPHORES "semaphores"

#define TEST_STR "DEAD"
#define LENGTH_TEST_STR 0x4

#define FLAGS_SHM_READONLY O_RDONLY|O_CREAT
#define FLAGS_SHM_READWRITE O_RDWR|O_CREAT
#define MODERW S_IRUSR|S_IWUSR

#define ERROR_SHM_CLIENT -1
#define ERROR_SHM SERVER -2
#define ERROR_MMAP_CLIENT -3
#define ERROR_MMAP_SERVER -4
#define ERROR_REQ_SERVER_F -5
#define ERROR_RES_CLIENT_F -5
#define ERROR_REQ_SERVER_P -6
#define ERROR_RES_CLIENT_P -6
#define ERROR_REQ_SERVER_I -7
#define ERROR_RES_CLIENT_I -7
#define ERROR_UNMAP_CLOSE_CLIENT -8
#define ERROR_UNMAP_CLOSE_SERVER -8


/*
 * file descriptors of shared objects
 */
typedef struct
{
  int fd_shm_request_;
  int fd_shm_response_;
  int fd_shm_semaphores_;
} filedescriptors;

/*
 * shared objects for locks
 */
typedef struct
{
  sem_t request_sem;
  sem_t response_sem;
} shmlocks;

/*
 * the response struct
 */
typedef struct
{
  char message [45];
} shmresponse;

/*
 * the request struct
 */
typedef struct
{
  char message [20];
} shmrequest;

/*
 *the according address pointing to the mapped region
 */
typedef struct
{
  shmrequest *mapped_region_request_;
  shmresponse *mapped_region_response_;
  shmlocks *mapped_region_semaphores_;
} mappings;

/*
 * global variables.
 */
extern filedescriptors fds;
extern mappings mmaps;
extern pid_t process_id;

/*
 * various function signatures used by the
 * util.c file
 */
void removeAllSHM();
void checkResults();
void checkSHMAnsweringProcess();
void *obServer();
void checkMMAPSendingProcess();
void checkSHMSendingProcess();
void checkPid();
void checkAnsweringProcess();
void checkCleanup();
int getCommand(char *buffer);

#endif
