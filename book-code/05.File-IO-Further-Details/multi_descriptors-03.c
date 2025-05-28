/*
 * 1. parent had fd1
 * 2. parent did fd2 = dup(fd1)
 *    Now fd1 & fd2 points to same file
 * 3. parent did fork()
 *    As eveything in parent is going to get copied to child
 *    So does the file pointer of the fd table too
 *    Now parent's (fd1 & fd2) and child's (fd1 & fd2) points to same file
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    const char *filename = "example.txt";

    // Open the file in read-only mode before forking.
    // This returns a file descriptor (fd1) referring to a file *description*,
    // which includes metadata like current file offset, access mode, etc.
    int fd1 = open(filename, O_RDONLY);
    if (fd1 < 0) {
        perror("open");
        return 1;
    }

    // Fork the process. This creates a new child process.
    // Both the parent and child get a copy of the file descriptor table.
    // Crucially: file descriptors in both processes point to the *same* file struct.
    // This means the file offset is shared between parent and child.
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        close(fd1);
        return 1;
    }

    // Duplicate the file descriptor in each process.
    // fd2 will refer to the *same* underlying file description as fd1,
    // so reads using fd1 and fd2 will share the same offset.
    int fd2 = dup(fd1);
    if (fd2 < 0) {
        perror("dup");
        close(fd1);
        return 1;
    }

    // Identify the process.
    if (pid == 0) {
        // Child process
        printf("Child started (PID: %d)\n", getpid());
    } else {
        // Parent process
        printf("Parent started (PID: %d)\n", getpid());
    }

    // Both parent and child enter this loop and alternate reading
    // from fd1 and fd2, which share the same file offset.
    //
    // Because of shared offset:
    // - Reading from fd1 advances the file offset
    // - Reading from fd2 sees the updated offset
    //
    // Thus, they *compete* to read the same stream of data.
    // The reads will NOT be independent, and the output may be interleaved.
    char ch;
    int turn = 0;
    while (1) {
        // Alternate between fd1 and fd2
        int fd = (turn % 2 == 0) ? fd1 : fd2;

        // Read one character
        ssize_t bytesRead = read(fd, &ch, 1);

        if (bytesRead == 0) {
            // End of file: break the loop
            break;
        } else if (bytesRead < 0) {
            perror("read");
            break;
        }

        // Print which process and which fd read the character
        fprintf(stderr,"[%s] Read from fd%d: %c\n",
               (pid == 0) ? "Child" : "Parent",
               (fd == fd1) ? 1 : 2, ch);

        // Sleep to make interleaved output easier to observe
        usleep(100000); // 100ms
        turn++;
    }

    // Clean up: close both descriptors
    close(fd1);
    close(fd2);

    // Parent waits for child to avoid zombie process
    if (pid > 0) {
        wait(NULL);
        printf("Parent exiting.\n");
    } else {
        printf("Child exiting.\n");
    }

    return 0;
}

