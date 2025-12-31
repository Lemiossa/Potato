/**
 * lexer.c
 * Criado por Matheus Leme Da Silva
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keywords.h"
#include "lexer.h"
#include "token.h"

// Retorna true se esta no fim
bool LexerAtEnd(Lexer *lexer) {
	if (!lexer || !lexer->contents || !lexer->tokens)
		return false; // prevenir um pouco
	if (lexer->pos >= lexer->contentsLength)
		return true;

	return false;
}

// Retorna o caractere atual
char LexerPeek(Lexer *lexer) {
	if (LexerAtEnd(lexer))
		return 0;
	return lexer->contents[lexer->pos];
}

// Retorna o caractere atual e avança o ponteiro
char LexerAdvance(Lexer *lexer) {
	if (LexerAtEnd(lexer))
		return 0;
	lexer->pos++;
	lexer->column++;
	return LexerPeek(lexer);
}

// Retorna o proximo caractere sem avançar
char LexerNext(Lexer *lexer) {
	if (LexerAtEnd(lexer))
		return 0;
	if (lexer->pos + 1 >= lexer->contentsLength)
		return 0;
	return lexer->contents[lexer->pos + 1];
}

// Pula espaços, \t, \r e \n em um Lexer
void LexerSkipWhitespaces(Lexer *lexer) {
	if (!lexer)
		return;

	char c = LexerPeek(lexer);

	while (!LexerAtEnd(lexer) &&
		   (c == '\n' || c == '\t' || c == '\r' || c == ' ')) {
		if (c == '\n') {
			lexer->line++;
			lexer->column = 1;
		} else {
			lexer->column++;
		}
		lexer->pos++;
		c = LexerPeek(lexer);
	}
}

// Adiciona um token no array de Tokens do lexer
bool LexerAddToken(Lexer *lexer, TokenType type, const char *start,
				   size_t length) {
	if (!lexer)
		return false;
	Token t;
	t.start = start;
	t.length = length;
	t.type = type;
	t.line = lexer->line;
	t.column = lexer->column;
	t.realContent = lexer->contents;

	if (!TokenArrayPush(lexer->tokens, t)) {
		printf(
			"Lexer internal error: Failed to add token to tokenlist, sorry\n");
		return false;
	}
	return true;
}

// Retorna o ponteiro atual para o content real de um Lexer
const char *LexerPeekReal(Lexer *lexer) {
	if (!lexer)
		return NULL;
	if (LexerAtEnd(lexer))
		return NULL;
	return &lexer->contents[lexer->pos];
}

// Cria um Lexer novo
Lexer *LexerCreate(const char *contents, size_t contentsLength) {
	if (!contents || contentsLength == 0)
		return NULL;
	Lexer *lexer = (Lexer *)malloc(sizeof(Lexer));
	if (!lexer)
		return NULL;

	lexer->contents = contents;
	lexer->contentsLength = contentsLength;
	lexer->tokens = TokenArrayCreate(64);
	if (!lexer->tokens) {
		printf("Lexer internal error: Failed to create token array\n");
		free(lexer);
		return NULL;
	}

	lexer->pos = 0;
	lexer->line = 1;
	lexer->column = 1;

	return lexer;
}

// Analiza um Lexer
// Por enquanto, só inteiros, strings e identificadores
TokenArray *LexerLex(Lexer *lexer) {
	if (!lexer) {
		printf("Lexer internal error: Lexer is null\n");
		return NULL;
	}

	while (!LexerAtEnd(lexer)) {
		LexerSkipWhitespaces(lexer);
		char c = LexerPeek(lexer);

		// Comentários
		if (c == '#') {
			while (!LexerAtEnd(lexer) && c != '\n')
				c = LexerAdvance(lexer);
			if (c == '\n') {
				lexer->line++;
				lexer->column = 1;
			}
			continue;
		}

		// Strings
		if (c == '"' || c == '\'') {
			char pair = c;
			LexerAdvance(lexer);
			const char *start = LexerPeekReal(lexer);

			const char *end = NULL;
			while (!LexerAtEnd(lexer)) {
				c = LexerAdvance(lexer);
				end = LexerPeekReal(lexer);
				if (c == pair)
					break;
				if (c == '\n') {
					lexer->line++;
					lexer->column = 1;
					break;
				}
			}

			size_t length = (size_t)(end - start);
			if (!LexerAddToken(lexer, TOKEN_STRING, start, length))
				return NULL;

			if (c != pair) {
				tokenError(lexer->tokens->tokens[lexer->tokens->count - 1],
						   "Lexical error: Unclosed string\n");
				return NULL;
			}
			LexerAdvance(lexer);
			continue;
		}

		// Números
		if (isdigit(c)) {
			const char *start = LexerPeekReal(lexer);
			char *end;

			strtoll(start, &end, 0);

			// double
			if (*end == '.') {
				strtod(start, &end);
				size_t length = (size_t)(end - start);
				lexer->pos += length;
				lexer->column += length;

				if (!LexerAddToken(lexer, TOKEN_FLOAT, start, length))
					return NULL;
			} else {
				size_t length = (size_t)(end - start);
				lexer->pos += length;
				lexer->column += length;

				if (!LexerAddToken(lexer, TOKEN_INTEGER, start, length))
					return NULL;
			}
			continue;
		}

		// Identificadores
		if (c == '_' || isalpha(c)) {
			const char *start = LexerPeekReal(lexer);
			while (!LexerAtEnd(lexer) && isalnum(c))
				c = LexerAdvance(lexer);

			const char *end = LexerPeekReal(lexer);
			size_t length = (size_t)(end - start);
			if (!LexerAddToken(lexer, TOKEN_IDENTIFIER, start, length))
				return NULL;
			continue;
		}

		const char *start = LexerPeekReal(lexer);
		switch (c) {
		case ';': {
			LexerAddToken(lexer, TOKEN_SEMICOLON, start, 1);
		} break;
		case ',': {
			LexerAddToken(lexer, TOKEN_COMMA, start, 1);
		} break;
		case '.': {
			LexerAddToken(lexer, TOKEN_DOT, start, 1);
		} break;
		case '=': {
			if (LexerNext(lexer) == '=') {
				LexerAddToken(lexer, TOKEN_EQUAL_EQUAL, start, 2);
				LexerAdvance(lexer);
			} else {
				LexerAddToken(lexer, TOKEN_EQUAL, start, 1);
			}
		} break;
		case '<': {
			if (LexerNext(lexer) == '=') {
				LexerAddToken(lexer, TOKEN_LESS_OR_EQUAL, start, 2);
				LexerAdvance(lexer);
			} else if (LexerNext(lexer) == '<') {
				LexerAddToken(lexer, TOKEN_BITWISE_SHIFT_LEFT, start, 2);
				LexerAdvance(lexer);
			} else {
				LexerAddToken(lexer, TOKEN_LESS_THAN, start, 1);
			}
		} break;
		case '>': {
			if (LexerNext(lexer) == '=') {
				LexerAddToken(lexer, TOKEN_GREATER_OR_EQUAL, start, 2);
				LexerAdvance(lexer);
			} else if (LexerNext(lexer) == '>') {
				LexerAddToken(lexer, TOKEN_BITWISE_SHIFT_RIGHT, start, 2);
				LexerAdvance(lexer);
			} else {
				LexerAddToken(lexer, TOKEN_GREATER_THAN, start, 1);
			}
		} break;
		case '+': {
			LexerAddToken(lexer, TOKEN_PLUS, start, 1);
		} break;
		case '-': {
			LexerAddToken(lexer, TOKEN_MINUS, start, 1);
		} break;
		case '*': {
			LexerAddToken(lexer, TOKEN_ASTERISK, start, 1);
		} break;
		case '/': {
			LexerAddToken(lexer, TOKEN_SLASH, start, 1);
		} break;
		case '%': {
			LexerAddToken(lexer, TOKEN_PERCENT, start, 1);
		} break;
		case '|': {
			if (LexerNext(lexer) == '|') {
				LexerAddToken(lexer, TOKEN_OR, start, 2);
				LexerAdvance(lexer);
			} else {
				LexerAddToken(lexer, TOKEN_BITWISE_OR, start, 1);
			}
		} break;
		case '^': {
			LexerAddToken(lexer, TOKEN_BITWISE_XOR, start, 1);
		} break;
		case '&': {
			if (LexerNext(lexer) == '&') {
				LexerAddToken(lexer, TOKEN_AND, start, 2);
				LexerAdvance(lexer);
			} else {
				LexerAddToken(lexer, TOKEN_BITWISE_AND, start, 1);
			}
		} break;
		case '(': {
			LexerAddToken(lexer, TOKEN_LPAREN, start, 1);
		} break;
		case ')': {
			LexerAddToken(lexer, TOKEN_RPAREN, start, 1);
		} break;
		case '[': {
			LexerAddToken(lexer, TOKEN_LBRACKET, start, 1);
		} break;
		case ']': {
			LexerAddToken(lexer, TOKEN_RBRACKET, start, 1);
		} break;
		case '{': {
			LexerAddToken(lexer, TOKEN_LBRACE, start, 1);
		} break;
		case '}': {
			LexerAddToken(lexer, TOKEN_RBRACE, start, 1);
		} break;
		}

		LexerAdvance(lexer);
	}

	if (!LexerAddToken(lexer, TOKEN_EOF, NULL, 0)) {
		return NULL;
	}

	for (size_t i = 0; i < lexer->tokens->count; i++) {
		Token *token = &lexer->tokens->tokens[i];
		if (token->type == TOKEN_IDENTIFIER) {
			for (size_t j = 0; keywords[j].name; j++) {
				if (token->length != (size_t)strlen(keywords[j].name))
					continue;
				if (strncmp(token->start, keywords[j].name, token->length) ==
					0) {
					token->type = keywords[j].token;
				}
			}
		}
	}

	return lexer->tokens;
}

// Destroi um Lexer
void LexerDestroy(Lexer *lexer) {
	if (!lexer)
		return;

	if (lexer->tokens)
		TokenArrayDestroy(lexer->tokens);
	free(lexer);
}
