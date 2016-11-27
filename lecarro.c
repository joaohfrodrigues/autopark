/******************************************************************************
 * File Name: lecarro.c
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
#include <string.h>

#include "parque.h"

/******************************************************************************
 * lefilecarro()
 *
 * Arguments: FILE, double pointer to car
 * Returns: Empty Park
 *
 * Description: Leitura de instruções do ficheiro
 *
 *****************************************************************************/
int lefilecarro(FILE *fp, car** new)
{
  int x, y, z;
  char as;
  int ta = 0;
  char id[5];


  if( fscanf(fp, "%s", id) != 1)
    return 0;

  (*new) = (car*) malloc(sizeof(car));
  if(fscanf(fp, "%d %c", &ta, &as) != 2)
    exit(0);

  if(fscanf(fp, "%d %d %d\n", &x, &y, &z) == 3)
  {
    (*new)->x = x;
    (*new)->y = y;
    (*new)->z = z;
  }
  else
  {
    (*new)->x = -1;
    (*new)->y = -1;
    (*new)->z = -1;
  }

  strcpy((*new)->id, id);
  (*new)->access = as;
  (*new)->ta = ta;
  (*new)->tb = ta;

  return 1;
}

/******************************************************************************
 * vem_depois()
 *
 * Arguments: Item 1, Item 2
 * Returns: 1 se c1->ta > c2->ta
 *
 * Description: Função de comparação para a lista de espera
 *
 *****************************************************************************/

int vem_depois(Item c1, Item c2)
{
  if(((car*)c1)->ta > ((car*)c2)->ta)
    return 1;
  else
    return 0;
}