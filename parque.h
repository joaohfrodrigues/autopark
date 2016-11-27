/******************************************************************************
 *
 * File Name: parque.h
 * Author:    João Rodrigues e Sara Vieira
 * Revision:  v1.0
 * NAME
 *     parque.h - definition of Parque
 *
 *****************************************************************************/
#ifndef parqueheader
#define parqueheader

#include "LinkedList.h"
#include "graph.h"
/*
 *  Data type: EA
 *
 *  Description: Entry and Acess description
 */
typedef struct{
	char id[5];
	int x;
	int y;
	int z;
	char type;
}EA;


/*
 *  Data type: cfg
 *
 *  Description: Park Configuration
 */
typedef struct{
	int N; /*colunas*/
	int M; /*linhas*/
	int P;
	int E;
	int S;
	int NL; /*numero de lugares livres*/
	int NT; /*numero total de lugares*/
	char ***cfg;
	EA *entry;
	EA *access;
}Parque;

typedef struct carro{
	char id[6];
	int ta;
	int tb;
	char access;
	int x;
	int y;
	int z;
}car;

typedef struct r{
	int ta;
	int tb;
	int piso;
	int x;
	int y;
	int z;
}rest;

Parque* PARQUEinit(int N, int M, int P, int E, int S);
Parque* leconfig(char *fileparquecfg, int *n_vert);

void retira_carro(FILE *fp, Parque *park, Graph *graph, car *new_car, LinkedList *l_parked);
int compara_carro(Item c1, Item c2);
void free_carro(Item carro);
void free_rest(Item rest);
void freeparque(Parque *park);

int lefilecarro(FILE *, car**);
int vem_depois(Item c1, Item c2);

LinkedList * lefilerest(char *file_r);
int acontece_depois(Item c1, Item c2);

int escreve_saida(FILE *fp, char *vid, int tk, int pX, int pY, int pZ, char tm);

void estaciona(FILE *, Parque *p, Graph *graph, car *new_car, LinkedList *l_rest, LinkedList **l_parked , LinkedList **l_espera);
void escreve_estaciona(FILE *fp, Parque *p, Graph *graph, int *vetor, int pos, car *carro, int lugar, int prox, int ant);
void escreve_acesso(FILE *fp, Parque *p, Graph *graph, int *vetor, int pos, car *carro, int lugar);
int tempo_direcao(Parque* park, Graph *, int x, int y, int z, int ant, int prox);

int limpa_rest(Graph *graph, LinkedList *l_rest, car* new_car);
int v_rest(Graph *graph, LinkedList *l_rest, car* new_car);

int temp_rest(Graph *graph, LinkedList *l_rest, car* new_car, int temp);

#endif