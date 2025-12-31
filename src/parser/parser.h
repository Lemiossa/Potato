/**
 * parser.h
 * Criado por Matheus Leme Da Silva
 */
#pragma once
#include <stddef.h>

#include "../lexer/token.h"
#include "node.h"

typedef struct Parser {
	size_t pos;
	TokenArray *tokens;
} Parser;

Parser *ParserCreate(TokenArray *tokens);
Node *ParserParse(Parser *parser);
void ParserDestroy(Parser *parser);
