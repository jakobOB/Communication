#include "util.h"

/*-----------------------------------------------------------------
 * wrapper function for launching the obAnsweringProcess thread.
 *
 * @param none
 *
 * @return none
 */
void checkAnsweringProcess() {
  if (process_id == -1)
    exit(-3);

}
/*-----------------------------------------------------------------
 * checks the read- and write capabilities of the shared objects
 * initialized before.
 *
 * @param none
 *
 * @return none
 */
void checkSHMrw(int fd) {
  if (fd == -1) {
    exit(-1);
  }

}
/*-----------------------------------------------------------------
 * checks the read-only capabilities of the shared objects
 * initialized before.
 *
 * @param none
 *
 * @return none
 */
void checkSHMro(int fd) {
  if (fd == -1) {
    exit(-1);
  }
}

/*-----------------------------------------------------------------
 * wrapper function for testing the shared objects
 *
 * @param none
 *
 * @return none
 */
void checkSHMSendingProcess() {

  checkSHMrw(fds.fd_shm_request_);
  checkSHMrw(fds.fd_shm_semaphores_);
  checkSHMro(fds.fd_shm_response_);
}
/*-----------------------------------------------------------------
 * predefined function for testing.
 * wrapper function for testing the mmap capabilities.
 *
 * @param none
 *
 * @return none
 */
void checkMMAPSendingProcess() {
  if (mmaps.mapped_region_response_ == NULL
      || mmaps.mapped_region_request_ == NULL
      || mmaps.mapped_region_semaphores_ == NULL) {
    exit(-1);
  }
}

/*-----------------------------------------------------------------
 * predefined function for testing. This is just a wrapper for testing
 * the specific results
 *
 * @param none
 *
 * @return none
 */
void checkResults() {
  printf("[ROBERT]: %s\n", mmaps.mapped_region_response_->message);
}
/*-----------------------------------------------------------------
 * predefined function for testing. Checks if everything was cleaned
 * up properly in both processes.
 *
 * @param none
 *
 * @return none
 */
void checkCleanup() {
  if (shm_open(SHM_NAME_REQUEST, O_RDONLY, MODERW) != -1) {
    exit(-1);

  }
  if (shm_open(SHM_NAME_RESPONSE, O_RDONLY, MODERW) != -1) {
    exit(-1);

  }
  if (shm_open(SHM_NAME_SEMAPHORES, O_RDONLY, MODERW) != -1) {
    exit(-1);
  }
}

/*-----------------------------------------------------------------
 * reads from stdin and processes the string entered for an easier
 * handling of commands.
 *
 * @param char pointer to the buffer that will be processed
 *
 * @return int true/false when successfully read from the stdin
 */
int getCommand(char *buffer) {
  memset(buffer, 0, 20 * sizeof(char));
  if (fgets(buffer, 20 * sizeof(char), stdin) != NULL)
    return 1;
  else
    return 0;
}

void checkSHMAnsweringProcess() {
  checkSHMrw(fds.fd_shm_response_);
  checkSHMro(fds.fd_shm_request_);
}

