/******************************************************************************
 * File Name: lerest.c
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
 * lefilerest()
 *
 * Arguments: name of file
 * Returns: Lista de restrições
 *
 * Description: Leitura do ficheiro de restrições e inicialização da lista de restrições
 *
 *****************************************************************************/

LinkedList * lefilerest(char *file_r)
{
  int x, y, z, p;
  int ta, tb;
  char r;
  rest *new_rest;
  FILE *fr;
  LinkedList *l_rest = initLinkedList();

  fr = fopen(file_r, "r");

  while(fscanf(fr, "%c", &r) == 1)
  {
  	if(fscanf(fr, "%d %d", &ta, &tb) != 2)
      exit(0);

  	if(fscanf(fr, "%d %d %d\n", &x, &y, &z) == 3)
  		p = -1; /*esta interdito um LUGAR*/
  	else{
  		p = x; /*esta interdito um PISO*/
  		x = -1;
  		y = -1;
  		z = -1;
  	}

  	new_rest = (rest *) malloc(sizeof(rest));
  	new_rest->ta = ta;
  	new_rest->tb = tb;
  	new_rest->piso = p;
  	new_rest->x = x;
  	new_rest->y = y;
  	new_rest->z = z;
 	
 	  l_rest = insertSortedLinkedList(l_rest, (Item *)new_rest, &(acontece_depois));
  }
  fclose(fr);

  return l_rest;
}

/******************************************************************************
 * acontece_depois()
 *
 * Arguments: Item 1, Item 2
 * Returns: 1 se a retrição c1 começar depois da restrição c2
 *
 * Description: Função de comparação para a lista de restrições
 *
 *****************************************************************************/

int acontece_depois(Item c1, Item c2)
{
  if(((rest*)c1)->ta > ((rest*)c2)->ta)
    return 1;
  else
    return 0;
}


