/**
 * potato.c
 * Criado por Matheus Leme Da Silva
 */
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/lexer.h"
#include "lexer/token.h"

#include "potato.h"

typedef struct Parser {
	size_t pos;
	TokenArray *tokens;
} Parser;

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
	if (!tokens) return NULL;

	Parser *parser = (Parser *)malloc(sizeof(Parser));
	if (!parser) return NULL;

	parser->pos = 0;
	parser->tokens = tokens;

	return parser;
}

// Destroi um Parser
void ParserDestroy(Parser *parser) {
	if (!parser) return;

	free(parser);
}

int run(char *content, size_t contentSize);
int potatoProgramReturn = 0;

// Roda um arquivo em potato
int runFile(int argc, char **argv) {
	(void)argv;
	if (argc < 2) {
		printf("Inválid arguments\n");
		return 1;
	}

	FILE *f = fopen(argv[1], "r");
	if (!f) {
		printf("Internal error: Failed to open %s: %s\n", argv[1],
			   strerror(errno));
		return 1;
	}

	if (fseek(f, 0, SEEK_END) < 0) {
		printf("Internal error: Failed to seek %s: %s\n", argv[1],
			   strerror(errno));
		return 1;
	}

	size_t filesize = ftell(f);
	rewind(f);

	char *filecontent = (char *)malloc(filesize);
	if (!filecontent) {
		printf("Internal error: Failed to alloc memory for file content\n");
		return 1;
	}

	if (fread(filecontent, 1, filesize, f) != filesize) {
		printf("Internal error: Failed to read %s: %s\n", argv[0],
			   strerror(errno));
		return 1;
	}

	run(filecontent, filesize);

	free(filecontent);
	fclose(f);

	return 0;
}

// Roda um conteúdo
int run(char *content, size_t contentSize) {
	Lexer *lexer = LexerCreate(content, contentSize);
	TokenArray *tokens = LexerLex(lexer);
#ifdef DEBUG
	TokenArrayPrint(tokens);
#endif
	Parser *parser = ParserCreate(tokens);


	ParserDestroy(parser);
	LexerDestroy(lexer);

	return 0;
}
