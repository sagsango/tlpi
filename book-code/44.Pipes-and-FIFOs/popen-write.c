#include <stdio.h>
#include <stdlib.h>

/* XXX:
 * popen() only allows one-way communication: either read or write.
 * To do both, you'd need to go beyond popen() (e.g. with pipe() and fork()).
*/

int main() {
    FILE *fp;

    // Open a pipe to 'sort' and write to its stdin
    fp = popen("sort", "w");
    if (fp == NULL) {
        perror("popen failed");
        return 1;
    }

    fprintf(fp, "banana\n");
    fprintf(fp, "apple\n");
    fprintf(fp, "cherry\n");

    pclose(fp);
    return 0;
}
