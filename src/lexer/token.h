/**
 * token.h
 * Criado por Matheus Leme Da Silva
 */
#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef enum TokenType {
    TOKEN_FLOAT, // 3.14
    TOKEN_INTEGER, // 123
    TOKEN_STRING, // "Hello World"

    TOKEN_IDENTIFIER, // main

    TOKEN_SEMICOLON, // ;
    TOKEN_COMMA, // ,
    TOKEN_DOT, // .

    TOKEN_LPAREN, // (
    TOKEN_RPAREN, // )
    TOKEN_LBRACE, // {
    TOKEN_RBRACE, // }
    TOKEN_LBRACKET, // [
    TOKEN_RBRACKET, // ]

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

TokenArray *TokenArrayCreate(size_t initialCap);
bool TokenArrayPush(TokenArray *array, Token token);
int TokenArrayPrint(TokenArray *array);
void TokenArrayDestroy(TokenArray *array);
void pointToWord(const char *content, size_t line, size_t column, size_t length);
void tokenError(Token token, const char *format, ...);
