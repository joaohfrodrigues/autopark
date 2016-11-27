/******************************************************************************
 *
 * File Name: graph_pos.h
 * Author:    João Rodrigues e Sara Vieira
 * Revision:  v1.0
 * NAME
 *     graph_pos.h - definition of the main graph
 *
 *****************************************************************************/
#ifndef graphposheader
#define graphposheader

#include "parque.h"
#include "graph.h"

void preenche_graph(Parque* park, Graph *g, int *num_cruz);
int procura_edge(Graph *g, int n, int x, int y, int z, int dist);
int preenche_vertice(Parque *park, Graph *g, int k, int j, int i, int n);

#endif