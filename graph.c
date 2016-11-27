/******************************************************************************
 * File Name: graph.c
 * Author:    João Rodrigues, Sara Vieira
 * Revision:  v1.0
 * NAME
 *     Projeto-Gestor de Parque
 * SYNOPSIS
 *		#include <stdlib.h>
 *		#include <stdio.h>
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "heap.h"

#define P (wt[v] + aux->dist)
#define maxWT 100

/******************************************************************************
* GRAPHinit()
*
* Arguments: numero de vértices
* Returns: graph
*
* Description: inicializa o grafo do parque, tendo em conta o num de vertices
*
*****************************************************************************/
Graph *GRAPHinit(int V)
{
	int i;

	Graph *G = (Graph*) malloc(sizeof(Graph));
	G->V = V; G->E = 0;

  	G->v_vert = (vertice *) malloc(V*(sizeof(vertice)));

  	for(i=0; i<V; i++)
  	{
  		G->v_vert[i].tipo = 0;
  		G->v_vert[i].x = 0;
  		G->v_vert[i].y = 0;
  		G->v_vert[i].z = 0;
  		G->v_vert[i].rest = 0;
  		G->v_vert[i].ar_type = 'W';
  		G->v_vert[i].list_adj = NULL;
  	}
	return G;
}

/******************************************************************************
* GRAPHinsertE()
*
* Arguments: Graph, vertice 1, vertice 2, distancia entre eles
* Returns: Edge
*
* Description: cria uma aresta entre dois vertices do parque
*
*****************************************************************************/
Edge *GRAPHinsertE(Graph *G, int v, int w, int dist)
{
	Edge *new = NULL;
	Edge *aux = NULL;

	if(G->v_vert[v].list_adj == NULL)
	{
		new = (Edge *) malloc(sizeof(Edge));
		new->next = NULL;
	}
	else
	{
		for(aux = G->v_vert[v].list_adj ; aux != NULL; aux = aux->next)
		{
			if(aux->w == w)
				return G->v_vert[v].list_adj;
		}

		new = (Edge *) malloc(sizeof(Edge));

		new->next = G->v_vert[v].list_adj;
	}

	new->w = w;
	new->dist = dist;
	
	G->E++;
	return new;
}

/******************************************************************************
* GRAPHdestroy()
*
* Arguments: graph
* Returns: -
*
* Description: liberta a memória associada ao grafo
*
*****************************************************************************/
void GRAPHdestroy(Graph *G)
{
	int i;
	Edge * aux;
	
	for(i=0; i<G->V; i++)
	{
		aux=G->v_vert[i].list_adj;
		while(aux != NULL)
		{
			G->v_vert[i].list_adj = aux;
			aux = aux->next;

			free(G->v_vert[i].list_adj);
		}
	}

	free(G->v_vert);

	free(G);
}

/******************************************************************************
* GRAPHpfs()
*
* Arguments: graph, vertice de inicio de procura, vetor st, vetor wt
* Returns: -
*
* Description: realiza uma procura no grafo, atualizando os vetores, wt com a distância ao vertice de início e st com a precedência
*
*****************************************************************************/
void GRAPHpfs(Graph *G, int s, int *st, int *wt)
{
	int v, w; 
	Heap* acervo;
	Edge * aux;
	int n = 0;

	acervo = NewHeap(G->V, &compara_heap); /*Inicializa um acervo, em que a raíz é o vértice com distância mais pequena*/

	for (v = 0; v < G->V; v++)
	{
		st[v] =-1;
		wt[v] = maxWT;
		Insert(acervo, wt[v], v); /*Insere os vértices no acervo*/
	}
	wt[s] = 0;

	Modify(acervo, s, wt[s]); /*altera a posição do elemento, atualizando a sua prioridade*/

	while (acervo->n_elements != 0)
	{
		if (wt[v = RemoveMax(acervo)] != maxWT) /*retira o elemento com mais prioridade*/
		{
			if(v==s || (G->v_vert[v].tipo != LUGAR && G->v_vert[v].tipo != ACESSO && G->v_vert[v].tipo != ENTRADA && G->v_vert[v].rest != OCUPADO))
				for (aux = G->v_vert[v].list_adj; aux != NULL; aux = aux->next)
				{
					w = aux->w;
					if (wt[w] > P && G->v_vert[w].tipo != ENTRADA && G->v_vert[w].tipo != ACESSO && G->v_vert[w].rest != OCUPADO)
					{
						wt[w] = P;
						Modify(acervo, w, wt[w]);
						st[w] = v;
					}
				}
		}

		n++;
	}

	FreeHeap(acervo); /*liberta a memória associada ao acervo*/
}

/******************************************************************************
 * GRAPHfindV()
 *
 * Arguments: Graph, coordenadas
 * Returns: 1 se vertice encontrado, -1 caso contrário
 *
 * Description: encontra o indice do grafo com as coordenadas indicadas
 *
 *****************************************************************************/

int GRAPHfindV(Graph *G, int x, int y, int z)
{
	int i;

	for(i=0; i<G->V; i++)
		if(G->v_vert[i].x == x && G->v_vert[i].y == y && G->v_vert[i].z == z)
			return i;

	return -1;
}

/******************************************************************************
 * GRAPHfindC()
 *
 * Arguments: Graph, vertice, coordenadas
 * Returns: -1
 *
 * Description: encontra as coordenadas de um vertice v
 *
 *****************************************************************************/

void GRAPHfindC(Graph *G, int v, int *x, int *y, int *z)
{
	(*x) = G->v_vert[v].x;
	(*y) = G->v_vert[v].y;
	(*z) = G->v_vert[v].z;

	return;
}

/******************************************************************************
 * compara_heap()
 *
 * Arguments: par de inteiros
 * Returns: 1 se a>=b, 0 caso contrario
 *
 * Description: função de comparação para o acervo, de modo a que a distância mais pequena fique na raíz
 *
 *****************************************************************************/
int compara_heap(int a, int b)
{
	if(a < b)
		return 0;
	else
		return 1;
}