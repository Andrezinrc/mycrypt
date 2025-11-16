#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mycrypt.h"


/*
 * MyCrypt - Ferramenta educacional de criptografia
 * 
 * AVISO: Este é um software para fins de estudo e pesquisa.
 *        A implementação NÃO é segura para uso em arquivos reais.
 *        O autor NÃO se responsabiliza por qualquer uso indevido.
 */

void show_usage(char *p_name) {
    printf("Uso: %s <comando> <arquivo_entrada> <arquivo_saida> <arquivo_chave>\n", p_name);
    printf("Comandos:\n");
    printf("encrypt - Criptografar arquivo\n");
    printf("decrypt - Descriptografar arquivo\n");
    printf("Exemplos:\n");
    printf("%s encrypt texto.txt texto.enc key.bin\n", p_name);
    printf("%s decrypt texto.enc texto.txt key.bin\n", p_name);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    if (argc!=5) {
        show_usage(argv[0]);
        return 1;
    }
    
    char *command = argv[1];
    char *f_in = argv[2];
    char *f_out = argv[3];
    char *f_k = argv[4];

    // Carrega a chave
    unsigned char* key = load_key(f_k, DEFAULT_KEYLEN);
    if (!key) { 
        printf("Erro ao carregar chave do arquivo: %s\n", f_k); 
        return 1; 
    }
    
    int result=0;
    
    if (strcmp(command, "encrypt")==0) {
        printf("Criptografando %s -> %s\n", f_in, f_out);
        result = encrypt_file(f_in, f_out, key, DEFAULT_KEYLEN);
        if (result!=0) {
            printf("Erro na criptografia!\n");
        } else {
            printf("Criptografia concluída com sucesso!\n");
        }
    }
    else if (strcmp(command, "decrypt")==0){
        printf("Descriptografando %s -> %s\n", f_in, f_out);
        result = decrypt_file(f_in,f_out,key,DEFAULT_KEYLEN);
        if (result!=0) {
            printf("Erro na descriptografia!\n");
        } else {
            printf("Descriptografia concluída com sucesso!\n");
        }
    }
    else {
        printf("Comando invalido: %s\n", command);
        show_usage(argv[0]);
        result=1;
    }
    
    free(key);
    return result;
}





