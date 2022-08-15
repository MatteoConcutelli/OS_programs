#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	
	const int SIZE = 4096;
	const char *nome = "/memcond";
	int shm_fd;

	void *shm_ptr;
	
	shm_fd = shm_open(nome, O_RDONLY, 0666);

	shm_ptr = mmap (0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

	printf("%s", (char*) shm_ptr);	// lettura segmento mem condivisa

	shm_unlink(nome); // rimozione segmento di memoria condivisa
	return 0;
}
