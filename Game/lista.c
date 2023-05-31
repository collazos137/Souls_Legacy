#include "lista.h"

/*-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_- operaciones sobre listas -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/

/* creacion de lista */
List createList() {
	List list;
	list.size = 0;
	list.firts = NULL;
	list.last = NULL;
	return list;
}

/* creacion de Nodo */
Node* createNode(int date) {
	Node* node = (Node*)malloc(sizeof(Node));
	node->date = date;
	node->next = NULL;
	return node;
}

/* inserta un elemento al final de la lista */
void append(List* list, int num){
	Node* unused = createNode(num);
	if ( list->last == NULL ){
		list->firts = unused;
		list->last = unused;
	} else {
		(list->last)->next = unused;
		list->last = unused; 
	}
	list->size++;
}

/* libera la lista */
void kill(List* list){
	if ( list->last == NULL ){
		return;
	}
	Node* tail = list->firts;
	Node* head = tail->next;
	list->firts = NULL;
	list->last = NULL;
	while (head != NULL){
		free(tail);
		tail = head;
		head = head->next;
	}
	free(tail);
}

/* insercion ordenda de elementos */
void pushSort(List* list, int num){
    Node* unused = createNode(num);
    Node* ax = list->firts;
    if(ax != NULL){
        if(ax->date >= num){
            unused->next = list->firts;
            list->firts = unused;
        }else{
            while(ax->next != NULL && ax->next->date < num){
                ax = ax->next;
            }
            unused->next = ax->next;
            ax->next = unused;
        }
    }else{
        list->firts = unused;
    }
    list->size++;
}


/*
Elimina un elemento de lista dada su posicion
*/
int pop(List* list, int num){
	int ans = -1;
	if(num < list->size && num >= 0){
		--list->size;
		Node* ax1 = list->firts;
		if(num == 0){
			ans = list->firts->date;
			list->firts = ax1->next;
			free(ax1);
		}else{
			int i;
			Node* ax2;
			for(i = 0; i < num - 1; ++i){
				ax1 = ax1->next;
			}
			ans = ax1->next->date;
			ax2 = ax1->next;
			ax1->next = ax2->next;
			free(ax2);
		}
	}
	return ans;
}