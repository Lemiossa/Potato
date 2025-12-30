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
#include "lexer/lexer.h"
#include "util.h"

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
