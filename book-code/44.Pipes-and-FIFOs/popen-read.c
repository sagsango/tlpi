#include <stdio.h>
#include <stdlib.h>
/* XXX:
 * popen() only allows one-way communication: either read or write.
 * To do both, you'd need to go beyond popen() (e.g. with pipe() and fork()).
*/
int main() {
    FILE *fp;
    char buffer[128];

    fp = popen("ls -l", "r");
    if (fp == NULL) {
        perror("popen failed");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    pclose(fp);
    return 0;
}

