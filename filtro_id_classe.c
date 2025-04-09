#ifndef FILTRO_ID_CLASSE_H
#define FILTRO_ID_CLASSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 512  // tamanho máximo de uma linha

// estrutura para guardar uma linha do arquivo e o id_classe
typedef struct {
    char linha[MAX_LEN];
    int id_classe;
} Gravacao;

// função para extrair o número de dentro do campo entre "{ }"
// exemplo: "{123}" -> 123
int extrair_id_classe(const char *campo) {
    int id;
    sscanf(campo, "{%d}", &id);  
    // le um número inteiro entre chaves
    // sscanf, primeiro valor é um vuffer no caso o ponteiro char *campo, depois o campo para ler "{%d}", e por último o endereço da variável id
    return id;
}

// Função principal que faz a filtragem
void filtrar_por_id_classe(const char *entrada, const char *saida, int id_classe_alvo) {
    FILE *file = fopen(entrada, "r");  // abre o arquivo CSV original
    if (!file) {
        perror("Erro ao abrir arquivo de entrada");
        // aqui o programa não consegue abrir o arquivo de entrada, então ele imprime a mensagem de erro e sai da função
        return;
    }

    FILE *output = fopen(saida, "w");  // cria o arquivo para salvar os resultados
    if (!output) {
        perror("Erro ao criar arquivo de saída");
        fclose(file);
        // aqui o programa não consegue criar o arquivo de saída, então ele imprime a mensagem de erro e fecha o arquivo
        return;
    }

    char linha[MAX_LEN];
    int contador = 0;
    //isso daqui serve para contar quantas linhas foram filtradas e depois imprimir no final

    // le e escreve o cabeçalho (a primeira linha do CSV)
    if (fgets(linha, MAX_LEN, file)) {
        fputs(linha, output);
    }

    // aqui ele vai ler linha por linha do arquivo de entrada e seprar os campos
    while (fgets(linha, MAX_LEN, file)) {
        char copia[MAX_LEN];
        strcpy(copia, linha);  // le uma linha inteira do csv
        // funçaõ do strtok é separar a string em partes, delimitadas por um caractere específico, no caso o ';'

        char *token = strtok(copia, ";");  // Começa a dividir a linha pelos ';'
        int coluna = 1;
        char *campo_classe = NULL;

        // percorre os campos da linha até chegar no 4º campo (id_classe)
        while (token != NULL && coluna <= 6) {
            if (coluna == 4) {
                campo_classe = token;  // achou o campo que queremos e copia a linha e depois faz a filtragem tirando os "{}" deixando só o id
                break;
            }
            token = strtok(NULL, ";");  // Vai pro próximo campo
            coluna++;
        }

        // se achou o campo id_classe, extrai o valor
        if (campo_classe) {
            int id_extraido = extrair_id_classe(campo_classe);
            if (id_extraido == id_classe_alvo) {
                fputs(linha, output);  // copia a linha para o novo arquivo
                contador++;
            }
        }
    }
    // fecha os arquivos
    fclose(file);
    fclose(output);

    // exibe o total encontrado
    printf("total de registros com id_classe %d: %d\n", id_classe_alvo, contador);
    printf("regitros filtrados foram salvos em '%s'\n", saia);
}

#endif
// Fim do arquivo filtro_id_classe.h
