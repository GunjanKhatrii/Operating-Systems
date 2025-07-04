#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define MSGLEN 100

int main(void) {
    char *memfile = "shtrans";
    int fd;
    int wstat;
    pid_t npid;
    char *ptr = NULL, *hptr = NULL;

    // Open or create a shared memory object
    if ((fd = shm_open(memfile, O_RDWR | O_CREAT, MODE)) == -1) {
        printf("Error: 'shm_open(create)' doesn't work!\n");
        return EXIT_FAILURE;
    }

    // Resize the shared memory object to the desired size
    if (ftruncate(fd, sizeof(*ptr) + MSGLEN) == -1) {
        printf("Error: 'ftruncate()' doesn't work!\n");
        return EXIT_FAILURE;
    }

    // Map the shared memory into the process's address space
    if ((ptr = (char *)mmap(0, sizeof(*ptr) + MSGLEN,
                            PROT_READ | PROT_WRITE,
                            MAP_SHARED, fd, 0)) == (char *)-1) {
        printf("Error: 'mapping' impossible!\n");
        return EXIT_FAILURE;
    }

    // Close the file descriptor (not needed after mmap)
    close(fd);

    //Set a pointer to the message area after the flag byte
    hptr = ptr + 1;

    // Initialize the first byte as 0 to indicate memory is free
    *ptr = 0;

    // Create a child process using fork()
    npid = fork();
    if (npid) {
        // Parent process (Writer)
        usleep(100);  // Small delay
        printf("Parent process: Please type a message:\n");
        *hptr = '\0';
        fflush(stdin);

        // Read a line from stdin and store it in shared memory
        scanf("%[^\n]", hptr);

        // Set the flag byte to 1 to indicate data is ready
        *ptr = 1;

        // Wait for child to finish
        wait(&wstat);
        printf("Parent process: EXIT\n");
        return EXIT_SUCCESS;

    } else {
        // Child process (Reader)
        printf("Child process: Waiting for a message...\n");

        // Busy-wait until parent sets the flag
        while (*ptr == 0);

        // Read and print the message
        printf("Child process: I received this: \n %s\n", hptr);

        // Unmap the shared memory
        munmap(ptr, sizeof(*ptr) + MSGLEN);

        // Unlink the shared memory object
        if (shm_unlink(memfile) == -1) {
            printf("Error: 'Unlink' impossible!\n");
            return EXIT_FAILURE;
        }

        printf("Child process: EXIT\n");
        return EXIT_SUCCESS;
    }
}

