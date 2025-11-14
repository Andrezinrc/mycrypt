#include "mycrypt.h"
#include <time.h>

int generate_key(const char* filename, size_t keylen){
    
    unsigned char key[keylen];

    for(size_t i=0;i<keylen;i++){
        key[i] = rand() % 256;
    }
    
    FILE* fk = fopen(filename, "wb");
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

int encrypt_stream(FILE* in, FILE* out, const unsigned char* key, size_t keylen){
    if(!in || !out || !key || keylen==0) return -1;
    
    const size_t CHUNK = 4096;
    unsigned char* buf = malloc(CHUNK);
    if(!buf) return -1;
    
    size_t keypos = 0;
    size_t nread;
    
    while((nread=fread(buf, 1, CHUNK, in)) > 0){
        for(size_t i=0;i<nread;i++){
            unsigned char k = key[(keypos + i) % keylen];
            buf[i]=(unsigned char)((buf[i] + k) % 256);
        }
        
        keypos+=nread;
        
        if(fwrite(buf, 1, nread, out) != nread){
            free(buf);
            return -1;
        }
    }
    
    free(buf);
    return 0;
}

int encrypt_file(const char*in, const char* out, const unsigned char* key, size_t keylen){
    FILE* f_in = fopen(in, "rb");
    if(!f_in) return -1;
    
    FILE* f_out = fopen(out, "wb");
    if(!f_out) {fclose(f_in); return -1;}
    
    const char header[] = "ADR";
    fwrite(header, 1, 3, f_out);
    
    int r = encrypt_stream(f_in,f_out,key,keylen);
    fclose(f_in);
    fclose(f_out);
    return r;
}

int main(){
    srand(time(NULL));
    
    encrypt_file("test.txt", "test.enc", "key.bin", 32);

    return 0;
}
