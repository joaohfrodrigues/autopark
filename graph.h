/******************************************************************************
 * File Name: graph.h
 * Author:    João Rodrigues, Sara Vieira
 * Revision:  v1.0
 * NAME
 *     Projeto-Gestor de Parque
 * SYNOPSIS
 *		#include <stdlib.h>
 *		#include <stdio.h>
 *****************************************************************************/
#ifndef GraphHeader
#define GraphHeader

#define VIA 0
#define ENTRADA 1
#define ACESSO 2
#define RAMPA 3
#define LUGAR 4

#define LIVRE 0
#define OCUPADO 1


/*
 *  Data type: vertice
 *
 *  Description: description of any kind of crossroad, entries, exits or ramp
 */

typedef struct edge
{
	int w;
	int dist;
	struct edge *next;
} Edge;

typedef struct{
	int tipo;
	int x;
	int y;
	int z;
	int estado; /*ocupaçao de lugar ou via*/
	int rest;
	char ar_type; /*tipo de acesso ou direçao da rampa - u or d*/
	Edge *list_adj;
}vertice;

typedef struct graph
{
	int V;
	int E;
	vertice *v_vert;
} Graph;

Graph *GRAPHinit(int); /*Cria grafico com o numero final de vertices, sem arestas*/

void GRAPHpfs(Graph *G, int s, int *st, int *wt); /*calcula a arvore de caminhos mais curtos para um dado vertice*/

Edge* GRAPHinsertE(Graph *G, int v, int w, int dist); /*insere uma aresta no grafo*/

void GRAPHdestroy(Graph *G);

int GRAPHfindV(Graph *G, int x, int y, int z);

void GRAPHfindC(Graph *G, int v, int *x, int *y, int *z);

int compara_heap(int a, int b);

#endif