/**
 * lexer.h
 * Criado por Matheus Leme Da Silva
 */
#pragma once
#include <stddef.h>

#include "token.h"

typedef struct Lexer {
	const char *contents;
	size_t contentsLength;
	TokenArray *tokens;

	size_t pos;
	size_t line;
	size_t column;
} Lexer;

Lexer *LexerCreate(const char *contents, size_t contentsLength);
TokenArray *LexerLex(Lexer *lexer);
void LexerDestroy(Lexer *lexer);
