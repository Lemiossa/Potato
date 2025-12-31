/**
 * parser.c
 * Criado por Matheus Leme Da Silva
 */
#include <stdlib.h>

#include "../lexer/token.h"
#include "parser.h"

// Retorna o token atual em um Parser
Token *ParserPeek(Parser *parser) {
	if (!parser)
		return NULL;
	if (!parser->tokens || !parser->tokens->tokens)
		return NULL;

	return &parser->tokens->tokens[parser->pos];
}

// Retorna o token atual e incrementa o ponteiro em um Parser
Token *ParserAdvance(Parser *parser) {
	if (!parser)
		return NULL;
	if (!parser->tokens || !parser->tokens->tokens)
		return NULL;

	return &parser->tokens->tokens[parser->pos++];
}

// Espera um tipo de token de um Parser, se não encontrado, dá erro
// Se encontrado, avança
// Retorna 0 em caso de sucesso e != 0 em caso de erro
int ParserExpect(Parser *parser, TokenType type) {
	if (!parser)
		return 1;

	Token *token = ParserPeek(parser);
	if (!token)
		return 2;

	if (token->type != type) {
		tokenError(*token, "Syntax error: Expected %s, but found %s\n",
				   TokenGetString(type), TokenGetString(token->type));
		return 3;
	}

	ParserAdvance(parser);
	return 0;
}

// Cria um novo Parser
Parser *ParserCreate(TokenArray *tokens) {
	if (!tokens)
		return NULL;

	Parser *parser = (Parser *)malloc(sizeof(Parser));
	if (!parser)
		return NULL;

	parser->pos = 0;
	parser->tokens = tokens;

	return parser;
}

// Parsea um parser
Node *ParserParse(Parser *parser) {
	if (!parser)
		return NULL;

	return NULL;
}

// Destroi um Parser
void ParserDestroy(Parser *parser) {
	if (!parser)
		return;

	free(parser);
}
