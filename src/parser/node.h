/**
 * node.h
 * Criado por Matheus Leme Da Silva
 */
#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef enum NodeType {
	NODE_LITERAL_INTEGER,
	NODE_LITERAL_FLOATING,
	NODE_LITERAL_STRING,
	NODE_LITERAL_BOOLEAN,
	NODE_LITERAL_NIL,
} NodeType;

typedef union NodeData {
	// NODE_LITERAL_INTEGER
	long long integer;

	// NODE_LITERAL_FLOATING
	double floating;

	// NODE_LITERAL_STRING,
	struct {
		const char *start;
		size_t length;
	} string;

	// NODE_LITERAL_BOOLEAN
	bool boolean;
} NodeData;

typedef struct Node {
	NodeType type;
	NodeData data;
} Node;
