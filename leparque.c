/******************************************************************************
 * File Name: leparque.c
 * Author:    João Rodrigues, Sara Vieira
 * Revision:  v1.0
 * NAME
 *     Projeto-Gestor de Parque
 * SYNOPSIS
 *    #include <stdlib.h>
 *    #include <stdio.h>
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "parque.h"

/******************************************************************************
 * PARQUEinit()
 *
 * Arguments: Park configuration
 * Returns: Empty Park
 *
 * Description: Park Inicialization
 *
 *****************************************************************************/
Parque* PARQUEinit(int N, int M, int P, int E, int S)
{
  Parque *park;
  int i, j;

  park = (Parque *) malloc (sizeof(Parque));

  park->N = N;
  park->M = M;
  park->P = P;
  park->E = E;
  park->S = S;
  park->NT = 0;
  park->NL = 0;

  park->entry = (EA *) malloc(E*(sizeof(EA)));

  park->access = (EA *) malloc(S*(sizeof(EA)));

  park->cfg = (char ***) malloc(N*(sizeof(char**)));

  for(i=0; i<N; i++)
  {
    park->cfg[i] = (char **) malloc(M*(sizeof(char*)));

    for(j=0; j<M; j++)
      park->cfg[i][j] = (char *) malloc(P*(sizeof(char)));
  }

  return park;
}

/******************************************************************************
 * leficheiro()
 *
 * Arguments: char* - pointer to file name
 * Returns: Park Inicializated
 *
 * Description: Read Entry File with park configuration
 *
 *****************************************************************************/

Parque* leconfig(char *fileparquecfg, int *n_vert)
{
  Parque *park;
  FILE *fp;
  int N; /*num de colunas*/
  int M; /*num de linhas*/
  int P; /*num de pisos*/
  int E; /*total de entradas*/
  int S; /*total de acessos*/
  int i, j, k;
  char test;
  int Episo = 0, Spiso = 0;
  int Ecnt = 0, Scnt = 0;

  fp = fopen(fileparquecfg, "r");

  if(fscanf(fp, "%d%d%d%d%d\n", &N, &M, &P, &E, &S) != 5)
    exit(0);

  /* Initialize park*/
  park = PARQUEinit(N, M, P, E, S);

  for(i=0; i< (park->P) ; i++)
  {
    Episo = 0;
    Spiso = 0;

    for(j=(park->M)-1 ; j >= 0 ; j--)
    {
      for(k=0; k< (park->N) ; k++)
      {
        if(fscanf(fp, "%c", &(park->cfg[k][j][i])) != 1)
          exit(0);
        
        if(park->cfg[k][j][i] != '@')
          (*n_vert)++;
        if(park->cfg[k][j][i] == 'e')
          Episo++;
        if(park->cfg[k][j][i] == 'a')
          Spiso++;
      }
      
      if(fscanf(fp, "\n") != 0)
        exit(0);
    }


    for(j=0; j< Episo ; j++)
    {
      if(fscanf(fp, "%s", park->entry[Ecnt].id) != 1)
        exit(0);
      
      if(fscanf(fp, "%d %d %d %c\n", &(park->entry[Ecnt].x), &(park->entry[Ecnt].y), &(park->entry[Ecnt].z), &(park->entry[Ecnt].type)) != 4)
        exit(0);

      Ecnt++;
    }

    for(j=0; j< Spiso ; j++)
    {
      if(fscanf(fp, "%s", park->access[Scnt].id) != 1)
        exit(0);

      if(fscanf(fp, "%d %d %d %c\n", &(park->access[Scnt].x), &(park->access[Scnt].y), &(park->access[Scnt].z), &(park->access[Scnt].type)) != 4)
        exit(0);

      Scnt++;
    }

    if(i != P-1)
    {
      if(fscanf(fp, "%c\n", &test) != 1)
        exit(0);
    }
  }

  fclose(fp);
  return park;
}