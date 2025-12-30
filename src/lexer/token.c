/**
 * token.c
 * Criado por Matheus Leme Da Silva
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>

#include "token.h"
#include "../util.h"

// Cria um novo TokenArray
TokenArray *TokenArrayCreate(size_t initialCap) {
	TokenArray *array = (TokenArray *)malloc(sizeof(TokenArray));
	if (!array) return NULL;

	array->count = 0;
	array->cap = initialCap;
	array->tokens = (Token *)malloc(sizeof(Token) * initialCap);

	return array;
}

// Adiciona um token a lista do TokenArray
bool TokenArrayPush(TokenArray *array, Token token) {
    if (!array) return false;

    if (array->count >= array->cap) {
        size_t newCap = array->cap * 2;

        Token *newTokens = (Token *)realloc(array->tokens, sizeof(Token) *newCap);
        if (!newTokens)
            return false;

        array->cap = newCap;
        array->tokens = newTokens;
    }

    array->tokens[array->count++] = token;

    return true;
}

// Imprime um TokenArray
int TokenArrayPrint(TokenArray *array) {
    if (!array) return 1;
	if (!array->tokens) return 2;

    for (size_t i = 0; i < array->count; i++) {
        Token token = array->tokens[i];
        printf("Token %zu: type=%d, lexeme=%.*s, pos=(%zu,%zu)\n", i, token.type, (int)token.length, token.start, token.line, token.column);
    }

    return true;
}

// Destroi um TokenArray
void TokenArrayDestroy(TokenArray *array) {
    if (!array) return;

    free(array->tokens);
    free(array);
}

// Aponta pra um erro e mostra ele usando Token
void tokenError(Token token, const char *format, ...) {
    printf("\033[31mERROR[%zu:%zu]: ", token.line, token.column);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");

    pointToWord(token.realContent, token.line - 1, token.column - 1, token.length);
    printf("\033[0m");
}
