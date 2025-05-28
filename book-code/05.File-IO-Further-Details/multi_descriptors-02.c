#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    const char *filename = "example.txt";
    int fd1 = open(filename, O_RDONLY);
    if (fd1 < 0) {
        perror("open");
        return 1;
    }

    int fd2 = dup(fd1);
    if (fd2 < 0) {
        perror("dup");
        close(fd1);
        return 1;
    }

    char ch;
    int turn = 0;
    while (1) {
        int fd = (turn % 2 == 0) ? fd1 : fd2;
        ssize_t bytesRead = read(fd, &ch, 1);
        if (bytesRead == 0) {
            // End of file
            break;
        } else if (bytesRead < 0) {
            perror("read");
            break;
        }
        printf("Read from fd%d: %c\n", (fd == fd1) ? 1 : 2, ch);
        turn++;
    }

    close(fd1);
    close(fd2);
    return 0;
}

