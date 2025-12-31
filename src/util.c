/**
 * util.c
 * Criado por Matheus Leme Da Silva
 */
#include <stddef.h>
#include <stdio.h>

#include "util.h"

// Aponta pra uma palavra num conteudo
void pointToWord(const char *content, size_t line, size_t column,
				 size_t length) {
	const char *startLine = NULL;
	while (*content) {
		if (line == 0) {
			startLine = content;
			break;
		}

		if (*content == '\n') {
			line--;
		}
		content++;
	}

	if (!startLine)
		return;
	const char *linestr = startLine;

	while (*startLine && *startLine != '\n') {
		startLine++;
	}

	const char *endLine = startLine;
	if (linestr == endLine)
		return;
	size_t lineLength = (size_t)(endLine - linestr);

	int tabs = 0;
	for (size_t i = 0; i < lineLength; i++) {
		if (linestr[i] == '\t') {
			tabs++;
			printf("    ");
		} else {
			printf("%c", linestr[i]);
		}
	}

	printf("\n");
	for (int i = 0; i < tabs; i++) {
		printf("    ");
	}

	for (size_t i = 0; i < column; i++) {
		printf(" ");
	}

	for (size_t i = 0; i < length; i++) {
		printf("^");
	}

	printf("\n");
}
