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
	{"and", TOKEN_AND},			  {"or", TOKEN_OR},
	{"True", TOKEN_KEYWORD_TRUE}, {"False", TOKEN_KEYWORD_FALSE},
	{"Nil", TOKEN_KEYWORD_NIL},	  {NULL, TOKEN_EOF}};
