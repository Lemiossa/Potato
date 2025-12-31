/**
 * token.c
 * Criado por Matheus Leme Da Silva
 */
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../util.h"
#include "token.h"

// Retorna uma string de um TOKEN
const char *TokenGetString(TokenType type) {
	switch (type) {
	case TOKEN_FLOAT:
		return "FLOAT";
	case TOKEN_INTEGER:
		return "INTEGER";
	case TOKEN_STRING:
		return "STRING";
	case TOKEN_IDENTIFIER:
		return "IDENTIFIER";
	case TOKEN_SEMICOLON:
		return ";";
	case TOKEN_COMMA:
		return ",";
	case TOKEN_DOT:
		return ".";
	case TOKEN_EQUAL:
		return "=";
	case TOKEN_PLUS:
		return "+";
	case TOKEN_MINUS:
		return "-";
	case TOKEN_ASTERISK:
		return "*";
	case TOKEN_SLASH:
		return "/";
	case TOKEN_PERCENT:
		return "%";
	case TOKEN_BITWISE_OR:
		return "|";
	case TOKEN_BITWISE_XOR:
		return "^";
	case TOKEN_BITWISE_AND:
		return "&";
	case TOKEN_BITWISE_SHIFT_LEFT:
		return "<<";
	case TOKEN_BITWISE_SHIFT_RIGHT:
		return ">>";
	case TOKEN_EQUAL_EQUAL:
		return "==";
	case TOKEN_NOT_EQUAL:
		return "!=";
	case TOKEN_LESS_THAN:
		return "<";
	case TOKEN_GREATER_THAN:
		return ">";
	case TOKEN_LESS_OR_EQUAL:
		return "<=";
	case TOKEN_GREATER_OR_EQUAL:
		return ">=";
	case TOKEN_AND:
		return "&&";
	case TOKEN_OR:
		return "||";
	case TOKEN_LPAREN:
		return "(";
	case TOKEN_RPAREN:
		return ")";
	case TOKEN_LBRACE:
		return "{";
	case TOKEN_RBRACE:
		return "}";
	case TOKEN_LBRACKET:
		return "[";
	case TOKEN_RBRACKET:
		return "]";
	case TOKEN_EOF:
		return "EOF";
	default:
		return "UNKNOWN";
	}
}

// Cria um novo TokenArray
TokenArray *TokenArrayCreate(size_t initialCap) {
	TokenArray *array = (TokenArray *)malloc(sizeof(TokenArray));
	if (!array)
		return NULL;

	array->count = 0;
	array->cap = initialCap;
	array->tokens = (Token *)malloc(sizeof(Token) * initialCap);

	return array;
}

// Adiciona um token a lista do TokenArray
bool TokenArrayPush(TokenArray *array, Token token) {
	if (!array)
		return false;

	if (array->count >= array->cap) {
		size_t newCap = array->cap * 2;

		Token *newTokens =
			(Token *)realloc(array->tokens, sizeof(Token) * newCap);
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
	if (!array)
		return 1;
	if (!array->tokens)
		return 2;

	for (size_t i = 0; i < array->count; i++) {
		Token token = array->tokens[i];
		printf("Token %zu: type='%s', lexeme='%.*s', pos=(%zu,%zu)\n", i,
			   TokenGetString(token.type), (int)token.length, token.start,
			   token.line, token.column);
	}

	return true;
}

// Destroi um TokenArray
void TokenArrayDestroy(TokenArray *array) {
	if (!array)
		return;

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

	pointToWord(token.realContent, token.line - 1, token.column - 1,
				token.length);
	printf("\033[0m");
}
