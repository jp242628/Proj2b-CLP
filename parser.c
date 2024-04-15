// Joao P S L P
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STACK_SIZE 100

// Definindo a estrutura de pilha
typedef struct {
    char items[STACK_SIZE][3]; // Alterado para armazenar elementos de 3 caracteres (nó da árvore)
    int top;
} Stack;

// Função para inicializar a pilha
void initialize(Stack *stack) {
    stack->top = -1;
}

// Função para verificar se a pilha está vazia
bool isEmpty(Stack *stack) {
    return stack->top == -1;
}

// Função para empilhar um elemento na pilha
void push(Stack *stack, char item[3]) {
    if (stack->top == STACK_SIZE - 1) {
        printf("Erro: Pilha cheia\n");
        exit(EXIT_FAILURE);
    }
    stack->top++;
    strcpy(stack->items[stack->top], item);
}

// Função para desempilhar um elemento da pilha
void pop(Stack *stack) {
    if (isEmpty(stack)) {
        printf("Erro: Pilha vazia\n");
        exit(EXIT_FAILURE);
    }
    stack->top--;
}

// Função principal do parser
void parse(FILE *fp) {
    Stack stack;
    initialize(&stack);
    push(&stack, "[0,S,0]"); // Empilhando o nó raiz

    char token = fgetc(fp);
    while (!isEmpty(&stack)) {
        char *topStack = stack.items[stack.top];

        // Implementação da lógica para cada estado do autômato de pilha
        if (strcmp(topStack, "[0,S,0]") == 0) {
            if (token == 'a') {
                pop(&stack);
                push(&stack, "[b,S,a]");
            } else {
                printf("Erro de parsing: Esperava 'a', mas obteve '%c'\n", token);
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(topStack, "[b,S,a]") == 0) {
            if (token == 'b') {
                pop(&stack);
                push(&stack, "[a,S,b]");
            } else {
                printf("Erro de parsing: Esperava 'b', mas obteve '%c'\n", token);
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(topStack, "[a,S,b]") == 0) {
            if (token == 'c') {
                pop(&stack);
                printf("Push(c)\n");
                token = fgetc(fp); // Avança para o próximo token
            } else {
                printf("Erro de parsing: Esperava 'c', mas obteve '%c'\n", token);
                exit(EXIT_FAILURE);
            }
        } else {
            printf("Erro de parsing: Transição inválida\n");
            exit(EXIT_FAILURE);
        }
    }
}

int main() {
    FILE *fp = fopen("input.txt", "r"); // Substitua "input.txt" pelo nome do arquivo de entrada
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    parse(fp);

    fclose(fp);
    return 0;
}
