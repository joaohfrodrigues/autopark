/******************************************************************************
 *
 * File Name: heap.h
 * Author:    João Rodrigues, Sara Vieira
 * Revision:  v1.0
 *
 * DESCRIPTION
 *		funções do acervo
 *
 *****************************************************************************/
 
typedef struct{
	int dist;
	int id;
}data;

/* A heap is represented by a structure: */
struct _heap {
  int (*less) (int, int);     /* função de comparação*/
  int n_elements;               /* # elements in heap */
  data *heapdata;               /* An array of data */
};


typedef struct _heap Heap;

Heap *NewHeap(int tamanho, int (*less) (int, int));

void FreeHeap(Heap * h);

int Insert(Heap * h, int dist, int id);

void Modify(Heap * h, int indice, int newvalue);

int RemoveMax(Heap * h);

int GetIndex(Heap * h, int id);

