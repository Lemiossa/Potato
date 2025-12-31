/**
 * keywords.h
 * Criado por Matheus Leme Da Silva
 */
#pragma once
#include "token.h"

typedef struct Keyword {
	const char *name;
	TokenType token;
} Keyword;

static Keyword keywords[] = {
	{"and", TOKEN_AND}, {"or", TOKEN_OR}, {NULL, TOKEN_EOF}};
