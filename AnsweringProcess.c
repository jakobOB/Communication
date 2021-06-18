#include "AnsweringProcess.h"

/*-----------------------------------------------------------------
 * Message Description
 * in the following array any known requests and responses are listed
 * if a request does not have a answer the response should look like
 * "I do not understand: " + requestmessage
 *
 **/
char* messages[] = {
        "bye\n",                        //answer: "bye\n", ends both processes
        "hello\n",                      //answer: "hello\n"
        "how are you?\n",               //answer: "good\n"
        "what is your name?\n",         //answer: "Robert\n"
        "Robert\n",                     //answer: "Yes, that is my name.\n"
        "good\n",                       //"I do not understand: " + requestmessage
        "Yes, that is my name.\n"       //"I do not understand: " + requestmessage
};

/*-----------------------------------------------------------------
 *  Main routine that loops and forwards requests and eventually
 *  makes it public to the SendingProcess. stops when "bye" is received.
 *  Initialize everything needed in here.
 *
 *  @param argc number of arguments given
 *  @param argv char* that points to the argument
 *
 *  @return predefined value
 **/
int main(int argc, char **argv)
{
  printf("\n[ROBERT] I AM ALIVE!\n");
  initSharedMemoriesAnsweringProcess();
  checkSHMAnsweringProcess();
  initMmapingsAnsweringProcess();
  do
  {
    // add synchronisation, use dispacher to get answer to request and write it into response
    sem_post(&mmaps.mapped_region_semaphores_->request_sem);
    sem_wait(&mmaps.mapped_region_semaphores_->response_sem);

    if (strlen(mmaps.mapped_region_request_->message) == 0) {
      break;
    }

    char *response = dispatcher(mmaps.mapped_region_request_);
    strcpy(mmaps.mapped_region_response_->message, response);
    free(response);
    sem_post(&mmaps.mapped_region_semaphores_->request_sem);
  } while (strcmp(mmaps.mapped_region_request_->message, "bye\n") != 0);

  closeMmapingsAnsweringProcess();


  return -3;

}
/*-----------------------------------------------------------------
 * initializes shared objects on the AnsweringProcess and resizes them
 *
 * @param none
 * @return none
 */
void initSharedMemoriesAnsweringProcess()
{
  //Open shared resources made in the client.
  fds.fd_shm_response_ = shm_open(SHM_NAME_RESPONSE, O_RDWR, 0);
  ftruncate(fds.fd_shm_response_, sizeof(shmresponse));
  fds.fd_shm_request_ = shm_open(SHM_NAME_REQUEST, O_RDONLY, 0);
  fds.fd_shm_semaphores_ = shm_open(SHM_NAME_SEMAPHORES, O_RDWR, 0);
  ftruncate(fds.fd_shm_semaphores_, sizeof(shmlocks));
}
/*-----------------------------------------------------------------
 * maps the shared objects to the virtual memory space of the AnsweringProcess
 *
 * @param none
 *
 * @return none
 */
void initMmapingsAnsweringProcess()
{
    // Map all shared resources opened in this process to the
    //virtual address space of this process
  mmaps.mapped_region_response_ = (shmresponse*)mmap(0, sizeof(shmresponse), PROT_READ | PROT_WRITE, MAP_SHARED, fds.fd_shm_response_, 0);
  mmaps.mapped_region_request_ = (shmrequest*)mmap(0, sizeof(shmrequest), PROT_READ, MAP_PRIVATE, fds.fd_shm_request_, 0);
  mmaps.mapped_region_semaphores_ = (shmlocks*)mmap(0, sizeof(shmlocks), PROT_READ | PROT_WRITE, MAP_SHARED, fds.fd_shm_semaphores_, 0);
}
/*-----------------------------------------------------------------
 * removes all mappings and shared objects as seen from the AnsweringProcess
 * this part is an essential function for closing this application
 * accordingly without leaving artifacts on the system!
 *
 * @param none
 *
 * @return none
 */
void closeMmapingsAnsweringProcess()
{
  munmap(mmaps.mapped_region_response_, sizeof(shmresponse));
  munmap(mmaps.mapped_region_request_, sizeof(shmrequest));
  munmap(mmaps.mapped_region_semaphores_, sizeof(shmlocks));
}
/*-----------------------------------------------------------------
 * dispatches the messages from the request
 *
 * @param shmrequest the pointer to the request object
 *
 * @return char* the answer to the request
 */
char* dispatcher(shmrequest *request)
{
  //dispatch the request
  const char* response[] = {
      "bye\n",
      "hello\n",
      "good\n",
      "Robert\n",
      "Yes, that is my name.\n"
  };

  int len = sizeof(messages)/sizeof(messages[0]);
  char* buffer = (char*)malloc(45);

  for (int i = 0; i < len - 2; ++i) {
    if(strncmp(request->message, messages[i], sizeof(request->message)) == 0) {
      memcpy(buffer, response[i], 45);
      return buffer;
    }
  }
  char tmp[45] = {"I do not understand: "};
  strcat(tmp, request->message);

  memcpy(buffer, tmp, sizeof(tmp));
  return buffer;
}

