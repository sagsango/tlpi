#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <unistd.h>
#else
#include <crypt.h>
#endif



enum {MAX_LEN = 1024};

int main(int argc, char *argv[]) {
    char *text, *encrypted, *salt;
    size_t len;
    long lnmax;

    text = malloc(MAX_LEN);

    printf("Input string to be hashed: ");
    if (fgets(text, MAX_LEN, stdin) == NULL)
        exit(EXIT_FAILURE);

    len = strlen(text);
    if (text[len - 1] == '\n')
        text[len - 1] = '\0';

#ifdef __APPLE__
		salt = "ax"; // see man page 'crypt' for salt info.
#else
    salt = crypt_gensalt("$2b$", 15, NULL, 0);
#endif

    encrypted = crypt(text, salt); // will be 13 char string 

    printf("Encrypted: %s", encrypted);

    free(text);
    exit(EXIT_SUCCESS);
}
