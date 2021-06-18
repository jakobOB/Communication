#ifndef ASS5_SERVER_H
#define ASS5_SERVER_H

#include "util.h"

/*
 * Initialization of global variables
 */
filedescriptors fds = { -1, -1, -1};
mappings mmaps = { NULL, NULL, NULL};
int process_id = -1;

/*
 * Various headers for used functions in server.c
 */
void initSharedMemoriesAnsweringProcess();
void initMmapingsAnsweringProcess();
void closeMmapingsAnsweringProcess();
char* dispatcher(shmrequest* request);

#endif
