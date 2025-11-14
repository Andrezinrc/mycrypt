#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mycrypt.h"

int main() {
    srand(time(NULL));

    unsigned char* key = load_key("key.bin", DEFAULT_KEYLEN);
    if (!key) { 
        printf("Erro ao carregar chave\n"); 
        return 1; 
    }

    // encrypt_file("test.txt", "test.enc", key, DEFAULT_KEYLEN);
    decrypt_file("test.enc", "test.txt", key, DEFAULT_KEYLEN);

    free(key);
    return 0;
}
