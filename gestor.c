/******************************************************************************
 * File Name: gestor.c
 * Author:    João Rodrigues, Sara Vieira
 * Revision:  v1.0
 * NAME
 *     Projeto-Gestor de Parque-FUnção main
 * SYNOPSIS
 *		#include <stdlib.h>
 *		#include <stdio.h>
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parque.h"
#include "graph_pos.h"

int main(int argc, char **argv)
{

  FILE *fout;
  FILE *fcar;
  Parque *park;
  int n_vert = 0;
  int n_cruz = 0;
  Graph *graph = NULL;
  LinkedList *l_espera = initLinkedList();
  LinkedList *l_parked = initLinkedList();
  LinkedList *l_rest = initLinkedList();
  car *new_car = NULL;
  int tempo = 0;
  int i;
  car *aux_car = NULL;

  char extOut[] = ".pts";
  char * fileNameOut;

  char * fileparquecfg;
  char * fileparquecarro;
  char * fileparquerest = NULL;

  fileparquecfg = argv[1];
  fileparquecarro = argv[2];

  /******************************************************************************
 * LEITURA DO FICHEIRO DE RESTRIÇÕES
 *****************************************************************************/

  if(argc==4)
  {
    fileparquerest = argv[3];
    l_rest = lefilerest(fileparquerest);
  }

  /******************************************************************************
 * LEITURA E PREENCHIMENTO DO GRAFO E DA MATRIZ DO PARQUE
 *****************************************************************************/

  park = leconfig(fileparquecfg, &n_vert);

  graph = GRAPHinit(n_vert);
  preenche_graph(park, graph, &n_cruz);

  /******************************************************************************
 * PROCESSAMENTO DOS CARROS E RESTRIÇÕES
 *****************************************************************************/

  /* Determine output filename*/
  fileNameOut = (char *) malloc(sizeof(char) * (strlen(fileparquecfg)+1));

  strtok(fileparquecfg, ".");
  strcpy(fileNameOut, fileparquecfg);
  strcat(fileNameOut, extOut);

  fout = fopen(fileNameOut, "w");
  fcar = fopen(fileparquecarro, "r");


  /*LEITURA DE INSTRUÇÕES*/
  while(lefilecarro(fcar, &new_car))
  {
    if(new_car->access != 'S')
    {
      escreve_saida(fout, new_car->id, new_car->ta, new_car->x, new_car->y, new_car->z, 'i');
      
        /*ESTACIONA O CARRO*/
        estaciona(fout, park, graph, new_car, l_rest, &l_parked, &l_espera);
    }
    else{
      if(new_car->x != -1) /*RETIRAR UM CARRO DE UMA POSIÇÃO ESPECÍFICA*/
      {
        for(i=0; i<n_vert; i++)
          if(graph->v_vert[i].x == new_car->x && graph->v_vert[i].y == new_car->y && graph->v_vert[i].z == new_car->z)
          {
            graph->v_vert[i].estado = LIVRE;
            park->NL++;
          }

        /*VERIFICA LISTA DE ESPERA*/
        if(l_espera != NULL)
        {
          tempo = new_car->ta;
          tempo = temp_rest(graph, l_rest, new_car, tempo);
          if(tempo == new_car->ta)
          {
            escreve_saida(fout, new_car->id, new_car->ta, new_car->x, new_car->y, new_car->z, 's');
            free(new_car);
            new_car = (car *) getItemLinkedList(l_espera);
            new_car->tb = tempo;
            estaciona(fout, park, graph, new_car, l_rest, &l_parked, &l_espera);
            l_espera = nextHeadLinkedList(l_espera);
          }
          else{
            aux_car = (car *) getItemLinkedList(l_espera);
            aux_car->tb = tempo;
            estaciona(fout, park, graph, aux_car, l_rest, &l_parked, &l_espera);
            l_espera = nextHeadLinkedList(l_espera);
            escreve_saida(fout, new_car->id, new_car->ta, new_car->x, new_car->y, new_car->z, 's');
            free(new_car);
          }
        }
        else{
          escreve_saida(fout, new_car->id, new_car->ta, new_car->x, new_car->y, new_car->z, 's');
          free(new_car);
        }
      }    
      else{   /*RETIRA UM CARRO QUE ESTACIONAMOS*/
        /*VERIFICA LISTA DE ESPERA*/
        if(l_espera != NULL)
        {
          tempo = new_car->ta;
          tempo = temp_rest(graph, l_rest, new_car, tempo);
          if(tempo == new_car->ta)
          {
            retira_carro(fout, park, graph, new_car, l_parked);
            free(new_car);
            new_car = (car *) getItemLinkedList(l_espera);
            new_car->tb = tempo;
            estaciona(fout, park, graph, new_car, l_rest, &l_parked, &l_espera);
            l_espera = nextHeadLinkedList(l_espera);
          }
          else{
            aux_car = (car *) getItemLinkedList(l_espera);
            aux_car->tb = tempo;
            estaciona(fout, park, graph, aux_car, l_rest, &l_parked, &l_espera);
            l_espera = nextHeadLinkedList(l_espera);
            retira_carro(fout, park, graph, new_car, l_parked);
            free(new_car);
          }
        }
        else{
          retira_carro(fout, park, graph, new_car, l_parked);
          free(new_car);
        }
      }
    }
  }

  /******************************************************************************
 * LIBERTAÇÃO DE MEMÓRIA
 *****************************************************************************/
  fclose(fcar);
  fclose(fout);

  freeLinkedList(l_parked, &free_carro);
  freeLinkedList(l_espera, &free_carro);
  freeLinkedList(l_rest, &free_rest);

  GRAPHdestroy(graph);

  free(fileNameOut);

  freeparque(park);

  exit(0);
}