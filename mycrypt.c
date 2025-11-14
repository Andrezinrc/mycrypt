#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generate_key(const unsigned char* filename, size_t keylen){
    
    unsigned char key[keylen];

    for(size_t i=0;i<keylen;i++){
        key[i] = rand() % 256;
    }
    
    FILE* fk = fopen("key.bin", "wb");
    if(!fk){
        perror("Erro ao criar/abrir arquivo de chave");
        return -1;
    }
    
    size_t written = fwrite(key, 1, keylen, fk);
    fclose(fk);
    
    if(written!=keylen){
        fprintf(stderr, "Erro ao escrever chave no arquivo.\n");
        return -2;
    }
    
    return 0;
}

int main(){
    srand(time(NULL));
    
    if(generate_key("key.bin",32)==0){
        printf("chave gerada com sucesso.\n");
    } else {
        printf("Erro ao gerar chave.\n");
    }

    return 0;
}
