/******************************************************************************
 *
 * File Name: graph_pos.c
 * Author:    João Rodriguesm Sara Vieira
 * Revision:  v1.0
 * NAME
 *     graph_pos.c - definition of Parque
 *
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>

#include "graph_pos.h"

 /******************************************************************************
 * preenche_graph()
 *
 * Arguments: Park configuration
 * Returns: num cruzamentos, unidades de estrada
 *
 * Description: Procura cruzamentos e unidades de estrada
 *
 *****************************************************************************/
void preenche_graph(Parque* park, Graph *g, int *num_cruz)
{
	int i, j, k;
 	int n = 0;
 	
	for(i=0; i<park->P; i++) /*pisos || z*/
 	{
 		for(j=0; j<(park->M) ; j++) /*linhas || y*/
 		{
 			for(k=0; k<(park->N); k++) /*colunas || x*/
 			{

 				if(park->cfg[k][j][i] != '@')
 				{
 					n = preenche_vertice(park, g, k, j, i, n);
 				}
 			}
 		}
 	}
}

 /******************************************************************************
 * preenche_vertice()
 *
 * Arguments: Graph, one position - its coordinates, # of vert
 * Returns: numero de vertices preenchidos ate agora
 *
 * Description: processa a informaçao do vertice
 *
 *****************************************************************************/
int preenche_vertice(Parque *park, Graph *g, int k, int j, int i, int n)
{
	int aux;
	g->v_vert[n].x = k;
 	g->v_vert[n].y = j;
 	g->v_vert[n].z = i;

 	switch(park->cfg[k][j][i])
 	{
 		case 'a':
 			g->v_vert[n].tipo = ACESSO;
 			g->v_vert[n].estado = LIVRE;
 			g->v_vert[n].rest = LIVRE;
 			for(aux = 0; aux<park->S; aux++)
 				if(park->access[aux].x == k && park->access[aux].y == j && park->access[aux].z == i)
 					g->v_vert[n].ar_type = park->access[aux].type;
 			break;

 		case 'e':
 			g->v_vert[n].tipo = ENTRADA;
 			g->v_vert[n].estado = LIVRE;
 			g->v_vert[n].rest = LIVRE;
 			break;

 		case 'u':
 			g->v_vert[n].tipo = RAMPA;
 			g->v_vert[n].rest = LIVRE;
 			g->v_vert[n].ar_type = 'u';
 			break;

		case 'd':
 			g->v_vert[n].tipo = RAMPA;
 			g->v_vert[n].rest = LIVRE;
 			g->v_vert[n].ar_type = 'd';
 			break;

 		case ' ':
 			g->v_vert[n].tipo = VIA;
 			g->v_vert[n].rest = LIVRE;
 			break;

 		case '.':
 			g->v_vert[n].tipo = LUGAR;
 			g->v_vert[n].rest = LIVRE;
 			g->v_vert[n].estado = LIVRE;
 			park->NL++;
 			park->NT++;
 			break;

 		case 'x':
 			g->v_vert[n].tipo = LUGAR;
 			g->v_vert[n].rest = LIVRE;
 			g->v_vert[n].estado = OCUPADO;
 			park->NT++;
 			break;

 		default:
 			break;
 	}
 	n++;

 	if(k > 0)
 		procura_edge(g, (n-1), k-1, j, i, 1);

 	if(j > 0)
 		procura_edge(g, (n-1), k, j-1, i, 1);

 	if(i > 0 && g->v_vert[n-1].ar_type == 'd')
 		procura_edge(g, (n-1), k, j, i-1, 1);

 	return n;
}

 /******************************************************************************
 * procura_edge()
 *
 * Arguments: Graph, two different positions and its coordinates, # of vert and distance between them
 * Returns: Value that identifies existence or absence of edge
 *
 * Description: Verifica relaçao entre vertices e cria respetiva aresta
 *
 *****************************************************************************/
int procura_edge(Graph *g, int n, int x, int y, int z, int dist)
{
 	int i, valid = 0;
 	int v=0, w=0;

 	for(i=0; i<n; i++)
 	{

 		if(x == g->v_vert[i].x && y == g->v_vert[i].y && z == g->v_vert[i].z)
		{
			v=n;
			w=i;
 			if((g->v_vert[n].tipo == VIA || g->v_vert[i].tipo == VIA) && g->v_vert[i].z == g->v_vert[n].z)
 			{
 				valid = 1;
 			}
 			else if(g->v_vert[n].tipo == RAMPA && g->v_vert[i].tipo == RAMPA)
 			{
 				dist = 2;
 				valid = 1;
 			}
			
			if(valid == 1)
			{
				g->v_vert[v].list_adj = GRAPHinsertE(g, v, w, dist);
				g->v_vert[w].list_adj = GRAPHinsertE(g, w, v, dist);

			}
		}
 	}

 	return valid;
}