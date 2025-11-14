#include "mycrypt.h"
#include <time.h>
#include <string.h>

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

unsigned char* load_key(const char* filename, size_t keylen){
    FILE* f = fopen(filename, "rb");
    if(!f) return NULL;
    
    unsigned char* key = malloc(keylen);
    if(!key) {fclose(f); return NULL;}
    
    size_t read = fread(key,1,keylen,f);
    fclose(f);
    
    if(read!=keylen){free(key); return NULL;}
    
    return key;
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

int decrypt_stream(FILE* in, FILE* out, const unsigned char* key, size_t keylen){
    if(!in || !out || !key || keylen==0) return -1;
    const size_t CHUNK = 4096;
    unsigned char* buf = malloc(CHUNK);
    if(!buf) return -1;
    
    size_t keypos = 0;
    size_t nread;
    
    while((nread=fread(buf, 1, CHUNK, in)) > 0){
        for(size_t i=0;i<nread;i++){
            unsigned char k = key[(keypos + i) % keylen];
            buf[i]=(unsigned char)((buf[i] - k + 256)%256);
        }
        
        keypos+=nread;
        
        if(fwrite(buf, 1, nread, out)!=nread){
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

int decrypt_file(const char* in, const char* out, const unsigned char* key, size_t keylen){
    FILE* f_in = fopen(in, "rb");
    if(!f_in) return -1;
    
    FILE* f_out = fopen(out, "wb");
    if(!f_out) {fclose(f_in); return -1;}
    
    char header[3];
    fread(header, 1, 3, f_in);
    if(memcmp(header, "ADR", 3)!=0){
        fclose(f_in);
        fclose(f_out);
        return -2;
    }
    
    int r = decrypt_stream(f_in,f_out,key,keylen);
    
    fclose(f_in);
    fclose(f_out);
    return r;
}




