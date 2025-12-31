/**
 * main.c
 * Criado por Matheus Leme Da Silva
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "potato.h"

#ifndef VERSION
#define VERSION "Undefined"
#endif

// Mostra a versão
int showVersion(int argc, char **argv) {
	(void)argc;
	(void)argv;
	printf("Zynta v%s\n", VERSION);
	return 0;
}

typedef struct Operand {
	const char *name;
	bool need;
} Operand;

typedef struct Command {
	char letter;
	const char *longname;
	const char *description;
	int (*function)(int argc, char **args);
	int operandCount;
	Operand *operands;
} Command;

Operand potatoRunFileOperands[] = {
	{"FILE", true},
};

Command commands[] = {
	{'v', "version", "Show version of this program", showVersion, 0, NULL},
	{'r', "run-file", "Run a potato file", runFile, 1, potatoRunFileOperands},

	{0, NULL, NULL, NULL, 0, NULL} // Termina aqui
};

// Procura um comando por nome
Command *findCommandByLongName(const char *longname) {
	for (int i = 0; commands[i].longname && commands[i].function; i++) {
		if (strcmp(longname, commands[i].longname) == 0) {
			return &commands[i];
		}
	}
	return NULL;
}

// Procura um comando por letra
Command *findCommandByLetter(char letter) {
	for (int i = 0; commands[i].letter && commands[i].function; i++) {
		if (letter == commands[i].letter) {
			return &commands[i];
		}
	}
	return NULL;
}

// Exibe a ajuda baseado na lista de comados
void showhelp(char *name) {
	printf("Usage: %s [commands]\n", name);
	printf("\033[32mShort commands starts with '-'\033[0m\n");
	printf("\033[34mLong commands starts with '--'\033[0m\n");
	printf("Commands: \n");

	for (int i = 0; commands[i].function; i++) {
		printf("    \033[32m-%c\033[0m | \033[34m--%s\033[0m ",
			   commands[i].letter, commands[i].longname);
		if (commands[i].operands) {
			for (int j = 0; j < commands[i].operandCount; j++) {
				Operand operand = commands[i].operands[j];
				if (!operand.name)
					break;
				;
				if (operand.need) {
					printf("<%s> ", operand.name);
				} else {
					printf("[%s] ", operand.name);
				}
			}
		}
		printf("- \033[0m%s\n", commands[i].description);
	}

	printf(
		"    \033[32m-h\033[0m | \033[34m--help\033[0m - Show this message\n");
}

// Parsea comandos
int parseCommands(int argc, char **argv) {
	int lastCommandReturn = 0;
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			char *arg = argv[i];
			if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
				showhelp(argv[0]);
				return 0;
			}
			if (arg[0] == '-') {
				if (arg[1] == '-') {
					Command *command = findCommandByLongName(&arg[2]);
					if (!command) {
						printf("Unknown command: %s\n", arg);
						return 1;
					}
					lastCommandReturn = command->function(argc - i, &argv[i]);
				} else {
					arg++;
					while (*arg) {
						Command *command = findCommandByLetter(*arg);
						if (!command) {
							printf("Unknown command: %c\n", *arg);
							return 1;
						}
						lastCommandReturn =
							command->function(argc - i, &argv[i]);
						arg++;
					}
				}
			}
		}
	}
	return lastCommandReturn;
}

// Func principal da  linguagem
int main(int argc, char **argv) { return parseCommands(argc, argv); }
