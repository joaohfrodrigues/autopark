/******************************************************************************
 *
 * File Name: parque.c
 * Author:    João Rodrigues e Sara Vieira
 * Revision:  v1.0
 * NAME
 *     parque.c - definition of Parque
 *
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parque.h"

 /******************************************************************************
* retira_carro()
*
* Arguments: File, Park, Graph, car, lista de estacionados 
* Returns: --
*
* Description: Função que retira um carro da lista de carros estacionados
*
*****************************************************************************/

void retira_carro(FILE *fp, Parque *park, Graph *graph, car *carro, LinkedList *l_parked)
{
  int lugar = 0;
  int tempo = carro->ta;

  carro = (car *)findItemLinkedList(l_parked, (Item)carro, &compara_carro);

  lugar = GRAPHfindV(graph, carro->x, carro->y, carro->z);

  graph->v_vert[lugar].estado = LIVRE;

  park->NL++;

  escreve_saida(fp, carro->id, tempo, carro->x, carro->y, carro->z, 's');
}

/******************************************************************************
* compara_carro()
*
* Arguments: Item carro 1, Item carro2
* Returns: --
*
* Description: Retorna 1 se forem o mesmo carro
*
*****************************************************************************/

int compara_carro(Item c1, Item c2)
{
  if(strcmp(((car *)c1)->id, ((car *)c2)->id) == 0)
    return 1;

  return 0;
}

/******************************************************************************
* free_carro()
*
* Arguments: Item
* Returns: --
*
* Description: Free used memory
*
*****************************************************************************/

void free_carro(Item carro)
{
  free((car *) carro);
}

/******************************************************************************
* free_rest()
*
* Arguments: Item
* Returns: --
*
* Description: Free used memory
*
*****************************************************************************/

void free_rest(Item res)
{
  free((rest *) res);
}

/******************************************************************************
* freeparque()
*
* Arguments: Park configuration
* Returns: --
*
* Description: Free used memory
*
*****************************************************************************/
void freeparque(Parque *park)
{
  int i, j;

  for(i=0; i<(park->N); i++)
  {
    for(j=0; j<(park->M); j++)
      free(park->cfg[i][j]);

    free(park->cfg[i]);
  }

  free(park->cfg);

  free(park->access);

  free(park->entry);

  free(park);
}