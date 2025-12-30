/**
 * potato.c
 * Criado por Matheus Leme Da Silva
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>
#include <errno.h>

#include "potato.h"
#include "lexer/token.h"
#include "util.h"

typedef struct Lexer {
    const char *contents;
    size_t contentsLength;
    TokenArray *tokens;

    size_t pos;
    size_t line;
    size_t column;
} Lexer;

// Cria um Lexer novo
Lexer *LexerCreate(const char *contents, size_t contentsLength) {
    if (!contents || contentsLength == 0) return NULL;
    Lexer *lexer = (Lexer *)malloc(sizeof(Lexer));
    if (!lexer) return NULL;

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

// Retorna true se esta no fim
bool LexerAtEnd(Lexer *lexer) {
    if (!lexer || !lexer->contents || !lexer->tokens) return false; // prevenir um pouco
    if (lexer->pos >= lexer->contentsLength) return true;

    return false;
}

// Retorna o caractere atual
char LexerPeek(Lexer *lexer) {
    if (LexerAtEnd(lexer)) return 0;
    return lexer->contents[lexer->pos];
}

// Retorna o caractere atual e avança o ponteiro
char LexerAdvance(Lexer *lexer) {
    if (LexerAtEnd(lexer)) return 0;
    lexer->pos++;
    lexer->column++;
    return LexerPeek(lexer);
}

// Pula espaços, \t, \r e \n em um Lexer
void LexerSkipWhitespaces(Lexer *lexer) {
    if (!lexer) return;

    char c = LexerPeek(lexer);

    while (c == '\n' || c == '\t' || c == '\r' || c == ' ') {
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
bool LexerAddToken(Lexer *lexer, TokenType type, const char *start, size_t length) {
    if (!lexer) return false;
    Token t;
    t.start = start;
    t.length = length;
    t.type = type;
    t.line = lexer->line;
    t.column = lexer->column;
    t.realContent = lexer->contents;

    if (!TokenArrayPush(lexer->tokens, t)) {
        printf("Lexer internal error: Failed to add token to tokenlist, sorry\n");
        return false;
    }
    return true;
}

// Retorna o ponteiro atual para o content real de um Lexer
const char *LexerPeekReal(Lexer *lexer) {
    if (!lexer) return NULL;
    if (LexerAtEnd(lexer)) return NULL;
    return &lexer->contents[lexer->pos];
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
                if (c == pair) break;
            }

            if (c != pair) {
                printf("Lexical error(%zu, %zu): Unclosed string\n", lexer->line, lexer->column);
                pointToWord(lexer->contents, lexer->line, lexer->column, 1);
                return NULL;
            }

            size_t length = (size_t)(end - start);
            if (!LexerAddToken(lexer, TOKEN_STRING, start, length))
                return NULL;
            LexerAdvance(lexer);
            continue;
        }

        // Números
        if (isdigit(c)) {
            const char *start = LexerPeekReal(lexer);
            while (!LexerAtEnd(lexer) && isdigit(c))
                c = LexerAdvance(lexer);

            const char *end = LexerPeekReal(lexer);
            size_t length = (size_t)(end - start);
            if (!LexerAddToken(lexer, TOKEN_INTEGER, start, length))
            	return NULL;
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

        LexerAdvance(lexer);
    }

    return lexer->tokens;
}

// Destroi um Lexer
void LexerDestroy(Lexer *lexer) {
    if (!lexer) return;

    if (lexer->tokens) TokenArrayDestroy(lexer->tokens);
    free(lexer);
}

// Roda um arquivo em potato
int runFile(int argc, char **argv) {
    (void)argv;
    if (argc < 2) {
        printf("Inválid arguments\n");
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("Internal error: Failed to open %s: %s\n", argv[1], strerror(errno));
        return 1;
    }

    if (fseek(f, 0, SEEK_END) < 0){
        printf("Internal error: Failed to seek %s: %s\n", argv[1], strerror(errno));
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
        printf("Internal error: Failed to read %s: %s\n", argv[0], strerror(errno));
        return 1;
    }


    Lexer *lexer = LexerCreate(filecontent, strlen(filecontent));

    if (!lexer) return 1;

    TokenArray *tokens = LexerLex(lexer);
    if (!tokens) {
        free(filecontent);
        fclose(f);
        return 1;
    }

    TokenArrayPrint(tokens);
    LexerDestroy(lexer);

    free(filecontent);
    fclose(f);

    return 0;
}
