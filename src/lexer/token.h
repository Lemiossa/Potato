/**
 * token.h
 * Criado por Matheus Leme Da Silva
 */
#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef enum TokenType {
	TOKEN_FLOAT,   // 3.14
	TOKEN_INTEGER, // 123
	TOKEN_STRING,  // "Hello World"

	TOKEN_IDENTIFIER, // main

	TOKEN_SEMICOLON, // ;
	TOKEN_COMMA,	 // ,
	TOKEN_DOT,		 // .

	TOKEN_EQUAL, // =

	TOKEN_PLUS,		// +
	TOKEN_MINUS,	// -
	TOKEN_ASTERISK, // *
	TOKEN_SLASH,	// /
	TOKEN_PERCENT,	// %

	TOKEN_BITWISE_OR,  // |
	TOKEN_BITWISE_XOR, // ^
	TOKEN_BITWISE_AND, // &

	TOKEN_BITWISE_SHIFT_LEFT,  // <<
	TOKEN_BITWISE_SHIFT_RIGHT, // >>

	TOKEN_EQUAL_EQUAL, // ==
	TOKEN_NOT_EQUAL,   // !=

	TOKEN_LESS_THAN,		// <
	TOKEN_GREATER_THAN,		// >
	TOKEN_LESS_OR_EQUAL,	// <=
	TOKEN_GREATER_OR_EQUAL, // >=

	TOKEN_AND, // && ou and
	TOKEN_OR,  // || ou or

	TOKEN_LPAREN,	// (
	TOKEN_RPAREN,	// )
	TOKEN_LBRACE,	// {
	TOKEN_RBRACE,	// }
	TOKEN_LBRACKET, // [
	TOKEN_RBRACKET, // ]

	TOKEN_KEYWORD_TRUE,	 // True
	TOKEN_KEYWORD_FALSE, // False

	TOKEN_KEYWORD_NIL, // Nil

	TOKEN_EOF // \0
} TokenType;

typedef struct Token {
	const char *start;
	size_t length;
	TokenType type;

	size_t line;
	size_t column;
	const char *realContent;
} Token;

typedef struct TokenArray {
	Token *tokens;
	size_t count;
	size_t cap;
} TokenArray;

const char *TokenGetString(TokenType type);
TokenArray *TokenArrayCreate(size_t initialCap);
bool TokenArrayPush(TokenArray *array, Token token);
int TokenArrayPrint(TokenArray *array);
void TokenArrayDestroy(TokenArray *array);
void pointToWord(const char *content, size_t line, size_t column,
				 size_t length);
void tokenError(Token token, const char *format, ...);
