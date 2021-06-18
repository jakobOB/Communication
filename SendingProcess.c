#include "SendingProcess.h"
/*-----------------------------------------------------------------
 *  Main routine that loops and makes requests and eventually
 *  makes it public to the server. Stops when the operation equals
 *  zero.
 *
 *  @param argc number of arguments given
 *  @param argv char* that points to the argument
 *
 *  @return predefined value
 **/
int main(int argc, char **argv)
{
    char buffer[20];

    //Initialize shared memory
    initSharedMemoriesSendingProcess();
    checkSHMSendingProcess();
    //Initialize memory mappings
    initMmapingsSendingProcess();
    checkMMAPSendingProcess();


    //Initialize semaphore locks
    initLocks();

    //starts Answering Process
    initProcess();

    //Check if Server is open and working
    checkAnsweringProcess();
    do
    {
      //process commands and values
      //synchronization
      //assigning/receiving to/from shared memory

      sem_wait(&mmaps.mapped_region_semaphores_->request_sem);
      if(getCommand(buffer)) {
        memcpy(mmaps.mapped_region_request_->message, &buffer, sizeof(buffer));
        sem_post(&mmaps.mapped_region_semaphores_->response_sem);
      } else {
        mmaps.mapped_region_request_->message[0] = '\0';
        sem_post(&mmaps.mapped_region_semaphores_->response_sem);
        break;
      }

      sem_wait(&mmaps.mapped_region_semaphores_->request_sem);
      checkResults();
    } while (strcmp(buffer, "bye\n")!=0);

    closeMmapingsSendingProcess();
    checkCleanup();
    return 0;

}

/*-----------------------------------------------------------------
 *  This function starts and initializes the AnsweringProcess
 *
 *  @param none
 *
 *  @return none
 **/
void initProcess()
{
    //launch AnsweringProcess and load the right executable
    process_id = fork();

    if (process_id == 0) {
      execl("./AnsweringProcess", "AnsweringProcess", NULL);
    }
}

/*-----------------------------------------------------------------
 * initializes shared objects on the SendingProcess and resizes them
 *
 * @param none
 *
 * @return none
 */
void initSharedMemoriesSendingProcess()
{
    //create and load all shared objects needed
    fds.fd_shm_response_ = shm_open(SHM_NAME_RESPONSE, FLAGS_SHM_READONLY, MODERW);
    fds.fd_shm_request_ = shm_open(SHM_NAME_REQUEST, FLAGS_SHM_READWRITE, MODERW);
    ftruncate(fds.fd_shm_request_, sizeof(shmrequest));
    fds.fd_shm_semaphores_ = shm_open(SHM_NAME_SEMAPHORES, FLAGS_SHM_READWRITE, MODERW);
    ftruncate(fds.fd_shm_semaphores_, sizeof(shmlocks));
}
/*-----------------------------------------------------------------
 * maps the shared objects to the virtual memory space of the SendingProcess
 *
 * @param none
 *
 * @return none
 */
void initMmapingsSendingProcess()
{
    //Initialize all mappings of the shared objects
    mmaps.mapped_region_response_ = (shmresponse*)mmap(0, sizeof(shmresponse), PROT_READ, MAP_PRIVATE, fds.fd_shm_response_, 0);
    mmaps.mapped_region_request_ = (shmrequest*)mmap(0, sizeof(shmrequest), PROT_READ | PROT_WRITE, MAP_SHARED, fds.fd_shm_request_, 0);
    mmaps.mapped_region_semaphores_ = (shmlocks*)mmap(0, sizeof(shmlocks), PROT_READ | PROT_WRITE, MAP_SHARED, fds.fd_shm_semaphores_, 0);
}
/*-----------------------------------------------------------------
 * initializes the locks on the shared object
 *
 * @param none
 *
 * @return none
 */
void initLocks()
{
    //Initialize all semaphores on the appropriate mapped
    //address space
    sem_init(&mmaps.mapped_region_semaphores_->response_sem, 1, 0);
    sem_init(&mmaps.mapped_region_semaphores_->request_sem, 1, 0);
}
/*-----------------------------------------------------------------
 * removes all mappings and shared objects as seen from the SendingProcess
 * this part is an essential function for closing this application
 * accordingly without leaving artifacts on the system!
 *
 * @param none
 *
 * @return none
 */
void closeMmapingsSendingProcess()
{
    //Clean up all resources allocated and created before!
    wait(NULL);
    sem_destroy(&mmaps.mapped_region_semaphores_->response_sem);
    sem_destroy(&mmaps.mapped_region_semaphores_->request_sem);
    shm_unlink(SHM_NAME_REQUEST);
    shm_unlink(SHM_NAME_RESPONSE);
    shm_unlink(SHM_NAME_SEMAPHORES);
    munmap(mmaps.mapped_region_response_, sizeof(shmresponse));
    munmap(mmaps.mapped_region_request_, sizeof(shmrequest));
    munmap(mmaps.mapped_region_semaphores_, sizeof(shmlocks));
}
