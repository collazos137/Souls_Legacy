#ifndef LISTA_H
#define LISTA_H

#include <stdlib.h>

typedef struct Node Node;
typedef struct List List;

/* estructura lista (para inventario)*/
struct Node {
	int date;
	struct Node* next;
};

struct List {
	int size;
	Node* firts;
	Node* last;
};

/* operaciones sobre listas */
List createList();
Node* createNode( int );
void pushSort( List *, int );
void kill(List * );
void append(List * , int );

#endif