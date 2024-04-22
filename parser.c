// Joao P S L P
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definição da estrutura de transição do autômato
typedef struct {
    char entrada;  // Símbolo de entrada
    char pilha_topo;  // Topo da pilha
    char *pilha_empilha;  // String a ser empilhada (ou NULL se não empilhar)
    int prox_estado;  // Próximo estado
} Transicao;

// Definição da estrutura do autômato de pilha
typedef struct {
    int num_estados;  // Número total de estados
    char *alfabeto;  // Alfabeto de entrada
    char *pilha_simbolos;  // Alfabeto da pilha
    Transicao *transicoes;  // Lista de transições
    int estado_inicial;  // Estado inicial
    int *estados_finais;  // Estados finais
    int num_estados_finais;  // Número de estados finais
} AutomatoPilha;

// Função para inicializar o autômato de pilha
AutomatoPilha *inicializarAutomato(int num_estados, char *alfabeto, char *pilha_simbolos,
                                   Transicao *transicoes, int estado_inicial,
                                   int *estados_finais, int num_estados_finais) {
    AutomatoPilha *ap = (AutomatoPilha *)malloc(sizeof(AutomatoPilha));
    if (ap == NULL) {
        printf("Erro ao alocar memória para o autômato.\n");
        exit(1);
    }
    ap->num_estados = num_estados;
    ap->alfabeto = alfabeto;
    ap->pilha_simbolos = pilha_simbolos;
    ap->transicoes = transicoes;
    ap->estado_inicial = estado_inicial;
    ap->estados_finais = estados_finais;
    ap->num_estados_finais = num_estados_finais;
    return ap;
}

// Função para verificar se um estado é final
bool estadoFinal(AutomatoPilha *ap, int estado) {
    for (int i = 0; i < ap->num_estados_finais; i++) {
        if (ap->estados_finais[i] == estado) {
            return true;
        }
    }
    return false;
}

// Função para simular o autômato de pilha
bool simularAutomato(AutomatoPilha *ap, char *entrada) {
    int estado_atual = ap->estado_inicial;
    char *pilha = (char *)malloc(sizeof(char));  // Pilha inicialmente vazia
    int topo_pilha = -1;  // Índice do topo da pilha (-1 indica pilha vazia)

    // Percorrer a entrada
    for (int i = 0; entrada[i] != '\0'; i++) {
        char simbolo_entrada = entrada[i];
        bool transicao_encontrada = false;

        // Verificar transições possíveis a partir do estado atual e símbolo de entrada
        for (int j = 0; j < ap->num_estados; j++) {
            Transicao t = ap->transicoes[j];
            if (t.prox_estado == estado_atual && t.entrada == simbolo_entrada &&
                (t.pilha_topo == pilha[topo_pilha] || t.pilha_topo == '\0')) {
                transicao_encontrada = true;

                // Desempilhar símbolos da pilha
                while (topo_pilha >= 0 && t.pilha_topo != '\0' && pilha[topo_pilha] != t.pilha_topo) {
                    topo_pilha--;
                }

                // Empilhar novos símbolos na pilha
                if (t.pilha_empilha != NULL) {
                    for (int k = 0; t.pilha_empilha[k] != '\0'; k++) {
                        topo_pilha++;
                        pilha = (char *)realloc(pilha, (topo_pilha + 1) * sizeof(char));
                        pilha[topo_pilha] = t.pilha_empilha[k];
                    }
                }

                estado_atual = t.prox_estado;
                break;  // Transição encontrada, então sair do loop
            }
        }

        if (!transicao_encontrada) {
            // Não há transição possível para o símbolo de entrada atual
            free(pilha);
            return false;
        }
    }

    // Verificar se o estado atual é um estado final
    bool resultado = estadoFinal(ap, estado_atual);

    free(pilha);
    return resultado;
}

int main() {
    // Abrir o arquivo de entrada
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    // Ler a entrada do arquivo
    char entrada[100]; // Ajuste o tamanho conforme necessário
    fscanf(file, "%s", entrada);
    fclose(file);

    // Definição do autômato de pilha
    Transicao transicoes[] = {
        {'\0', '\0', "S", 1},  // δ0: ε, ε, S -> push(S)
        {'\0', 'S', "aSb", 1}, // δ1: ε, S, aSb
        {'\0', 'S', "c", 1},   // δ2: ε, S, c
        {'a', 'a', "\0", 1},   // δ3: a, a, ε
        {'b', 'b', "\0", 1},   // δ4: b, b, ε
        {'c', 'c', "\0", 1}    // δ5: c, c, ε
    };
    int estados_finais[] = {1};
    AutomatoPilha *ap = inicializarAutomato(2, "abc", "S", transicoes, 0, estados_finais, 1);

    // Teste da função de simulação
    if (simularAutomato(ap, entrada)) {
        printf("A entrada \"%s\" é aceita pelo autômato.\n", entrada);
    } else {
        printf("A entrada \"%s\" não é aceita pelo autômato.\n", entrada);
    }

    // Liberação de memória
    free(ap);
    return 0;
}


    fclose(fp);
    return 0;
}
