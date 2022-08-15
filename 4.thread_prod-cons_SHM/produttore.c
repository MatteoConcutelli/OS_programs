#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main () {
	const int SIZE = 4096;
	const char *nome = "memcond";

	void *shm_ptr;

	const char *string_0 = "saluti";
	const char *string_1 = "a tutti!";

	int shm_fd;

	shm_fd = shm_open(nome, O_CREAT | O_RDWR, 0666); // crea o apri segmento
	// 0666 riguardo la protezione
	
  // ridimensiona la dimensione del segmento creato/aperto
	ftruncate(shm_fd, SIZE);

  // mappa il segmento in memoria
	shm_ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0); //


	sprintf(shm_ptr, "%s ",string_0); //scrive nel segmento

  // sposta il puntatore alla fine della prima stringa per poi mettere la
  // seconda
  shm_ptr += strlen(string_0);
	
  sprintf(shm_ptr, "%s ", string_1);
	shm_ptr += strlen(string_1);

  //sleep(60); // do tempo al consumatore di leggere quella memoria

	return 0;

}
