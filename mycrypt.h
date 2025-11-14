#ifndef MYCRYPT_H
#define MYCRYPT_H

#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_KEYLEN 32

/*
 * Gera uma chave aleatória e salva em arquivo
 */
int generate_key(const char* filename, size_t keylen);

/*
 * Criptografa um stream de dados usando XOR com chave
 */
int encrypt_stream(FILE* in, FILE* out, const unsigned char* key, size_t keylen);

/*
 * Criptografa um arquivo completo com header de identificação
 */
int encrypt_file(const char* in, const char* out, const unsigned char* key, size_t keylen);

#endif
