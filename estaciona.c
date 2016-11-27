/******************************************************************************
* File Name: estaciona.c
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
#include <string.h>

#include "parque.h"


/******************************************************************************
 * estaciona()
 *
 * Arguments: FILE, Park configuration, Graph, Graph_cruz, novo carro, lista de restrições, lista dos estacionados
 * Returns: -
 *
 * Description: função que estaciona o carro, tendo em conta o menor custo (solução ótima)
 *
 *****************************************************************************/
void estaciona(FILE *fp, Parque *p, Graph *graph, car *new_car, LinkedList *l_rest, LinkedList **l_parked , LinkedList **l_espera)
{
	int i, j, k = 0;
	int **st_acesso, *st_entrada;
	int **wt_acesso, *wt_entrada;
	int lugar = 0;
  int *find;
	int entrance = 0;
  int xparked=0, yparked=0, zparked=0;
  int indice_acesso = 0, indice_lugar = 0;
  int custo = 50000;
  int t_estaciona = 0;
  int n_acessos = 0;
  int custo_total = new_car->tb - new_car->ta;

  /*LIMPA AS RESTRIÇÕES E VERIFICA A EXISTÊNCIA DE NOVAS RESTRIÇÕES*/
  p->NL += limpa_rest(graph, l_rest, new_car);
  p->NL -= v_rest(graph, l_rest, new_car);


  /*SE NÃO HOUVEREM LUGARES DISPONÍVEIS, O CARRO VAI PARA A LISTA DE ESPERA*/
  if(p->NL <= 0)  
  {
    (*l_espera) = insertSortedLinkedList((*l_espera), (Item) new_car, &vem_depois);
    return;
  }

  /*ALOCAÇÃO DOS VETORES AUXILIARES*/

  for(i=0; i<p->S; i++)
    if(p->access[i].type == new_car->access)
      n_acessos++;

	st_acesso = (int**) malloc((n_acessos)*sizeof(int*));
  wt_acesso = (int**) malloc((n_acessos)*sizeof(int*));
  st_entrada = (int*) malloc((graph->V)*sizeof(int));
  wt_entrada = (int*) malloc((graph->V)*sizeof(int));
  find = (int*) malloc((n_acessos)*sizeof(int));

  for(i=0; i<n_acessos; i++)
  {
    st_acesso[i] = (int*) malloc((graph->V)*sizeof(int));
    wt_acesso[i] = (int*) malloc((graph->V)*sizeof(int));
  }

  /*INICIALIZA A VARIÁVEL LUGAR COM UM LUGAR QUE CUMPRA AS RESTRIÇÕES APRESENTADAS*/
  for(j=0; j< graph->V; j++)
    if(graph->v_vert[j].tipo == LUGAR && graph->v_vert[j].estado == LIVRE && graph->v_vert[j].rest == LIVRE)
    {
      lugar = j;
      break;
    }

  for(j=0; j< p->E; j++)
    if(p->entry[j].x == new_car->x && p->entry[j].y == new_car->y && p->entry[j].z == new_car->z)
    {
      entrance = GRAPHfindV(graph, p->entry[j].x, p->entry[j].y, p->entry[j].z);
      break;
    }

  /*DETERMINA A DISTÂNCIA DE TODOS OS VÉRTICES AO LUGAR*/
  GRAPHpfs(graph, entrance, st_entrada, wt_entrada);

  /*VERIFICAÇÃO DO LUGAR ÓTIMO, TENDO EM CONTA OS VÁRIOS ACESSOS DO MESMO TIPO*/
  for(i=0, k=0; i<p->S; i++)
  {
  	if(p->access[i].type == new_car->access)
  	{
      indice_acesso = GRAPHfindV(graph, p->access[i].x, p->access[i].y, p->access[i].z);

 			/*DETERMINA AS DISTÂNCIAS DE TODOS OS VÉRTICES AO ACESSO*/
      GRAPHpfs(graph, indice_acesso, st_acesso[k], wt_acesso[k]);

      /*DETERMINAÇÃO DO LUGAR COM MENOR CUSTO, DE TODOS OS ANALISADOS*/
      for(j=0; j< graph->V; j++)
      {
        if(graph->v_vert[j].tipo == LUGAR && graph->v_vert[j].estado == LIVRE && graph->v_vert[j].rest == LIVRE && (3*wt_acesso[k][j] + wt_entrada[j])< custo)
        {
          lugar = j;
          custo = wt_entrada[j] + 3*wt_acesso[k][j];
          indice_lugar = k;
        }
      }

      find[k] = lugar;
      k++;
 		}
  }

  /*ESCRITA NO FICHEIRO DE SAÍDA DA DESLOCAÇÃO DO CARRO ATÉ AO LUGAR*/
  escreve_estaciona(fp, p, graph, st_entrada, lugar, new_car, lugar, lugar, lugar);

  GRAPHfindC(graph, lugar, &xparked, &yparked, &zparked);

  /*ATUALIZA O FICHEIRO DE SAÍDA COM O CARRO ESTACIONADO*/
  escreve_saida(fp, new_car->id, new_car->tb, xparked, yparked, zparked, 'e');

  /*ATUALIZAÇÃO DAS COORDENADAS DO CARRO ESTACIONADO E DAS INFORMAÇÕES RELATIVAS AOS LUGARES DISPONÍVEIS*/
  new_car->x = xparked;
  new_car->y = yparked;
  new_car->z = zparked;

  graph->v_vert[lugar].estado = OCUPADO;
  p->NL--;
  t_estaciona = new_car->tb;
  (new_car->tb)++;

  /*ESCRITA NO FICHEIRO DE SAÍDA DA DESLOCAÇÃO DO LUGAR ATÉ AO ACESSO*/
  escreve_acesso(fp, p, graph, st_acesso[indice_lugar], lugar, new_car, lugar);

  custo_total += custo;

  /*ESCRITA NO FICHEIRO DE SAÍDA O RESUMO DO PERCURSO DO CARRO*/
  escreve_saida(fp, new_car->id, new_car->ta, t_estaciona, new_car->tb, custo_total, 'x');

  /*ATUALIZAÇÃO DA LISTA DE CARROS ESTACIONADOS*/
  (*l_parked) = insertUnsortedLinkedList((*l_parked), (Item) new_car);

  /*LIBERTAÇÃO DOS VETORES AUXILIARES*/
  for(i=0; i<n_acessos; i++)
  {
    free(st_acesso[i]);
    free(wt_acesso[i]);
  }

  free(st_acesso);
  free(wt_acesso);
  free(st_entrada);
  free(wt_entrada);
  free(find);

}

/******************************************************************************
 * escreve_estaciona()
 *
 * Arguments: FILE, Park configuration, Graph, vetor de inteiros com as distancias, posicao, novo carro, lugar, proximo vertice e vertice anterior
 * Returns: -
 *
 * Description: escreve o caminho desde a entrada até ao estacionamento
 *
 *****************************************************************************/

void escreve_estaciona(FILE *fp, Parque *p, Graph *graph, int *vetor, int pos, car *carro, int lugar, int prox, int ant)
{
	int x=0, y=0, z=0;
  int tempo = 0;

	if(vetor[pos] != -1) /*ATÉ CHEGAR À ENTRADA CHAMA-SE A SI PRÓPRIA, PARA PERCORRER O VETOR ST[] PROVENIENTE DO DJIKSTRA*/
  {
    prox = pos;
    pos = vetor[pos];
    ant = pos;
    if(vetor[pos] != -1)
      ant = vetor[pos];

		escreve_estaciona(fp, p, graph, vetor, pos, carro, lugar, prox, ant);
  }
  else
    return;

	GRAPHfindC(graph, pos, &x, &y, &z);

  tempo = tempo_direcao(p, graph, x, y, z, ant, prox); /*INDICA SE MUDOU DE DIREÇÃO, ASSIM COMO O TEMPO DE DESLOCAÇÃO*/

	if(tempo != 0)
  {
    if(carro->tb == 0)
      carro->tb =1;
      
    escreve_saida(fp, carro->id, carro->tb, x, y, z, 'm'); /*ESCREVE O MOVIMENTO DO CARRO*/
  }

  if(graph->v_vert[prox].tipo != RAMPA && graph->v_vert[pos].tipo == RAMPA) /*QUANDO SOBE/DESCE UMA RAMPA, SÓ NUM DOS INSTANTES É QUE O TEMPO VALE 2*/
    (carro->tb)--;

  /*ATUALIZA O TEMPO TOTAL DE MOVIMENTO DO CARRO, EM TB*/
  if(tempo == 0)
    (carro->tb)++;
  else
    (carro->tb)+= tempo;

  return;
}

/******************************************************************************
 * escreve_acesso()
 *
 * Arguments: FILE, Park configuration, Graph, vetor de inteiros com as distancias, posicao, novo carro, lugar
 * Returns: -
 *
 * Description: escreve o caminho desde o lugar até ao acesso
 *
 *****************************************************************************/

void escreve_acesso(FILE *fp, Parque *p, Graph *graph, int *vetor, int pos, car *carro, int lugar)
{
  int ant = 0;
  int prox = 0;
  int x=0, y=0, z=0;
  int cntp = 0;
  int tempo = 0;

  while(vetor[pos] != -1)
  {
    ant = pos;
    pos = vetor[pos];
    if(vetor[pos] != -1)
      prox = vetor[pos];

    GRAPHfindC(graph, pos, &x, &y, &z);
    tempo = tempo_direcao(p, graph, x, y, z, ant, prox); /*INDICA SE MUDOU DE DIREÇÃO, ASSIM COMO O TEMPO DE DESLOCAÇÃO*/

    if(graph->v_vert[pos].tipo == ACESSO || tempo != 0)
    {
      if(vetor[pos] == -1)
      {
        escreve_saida(fp, carro->id, carro->tb, x, y, z, 'a'); /*INDICA QUE CHEGOU AO ACESSO*/
        (carro->tb)--;
      }
      else
      {
        escreve_saida(fp, carro->id, carro->tb, x, y, z, 'p'); /*ESCREVE O MOVIMENTO DO PEÃO*/
        cntp++;
      }
    }

    if(cntp == 0 &&  vetor[prox] == -1)
    {
      escreve_saida(fp, carro->id, carro->tb, x, y, z, 'p'); /*SE NÃO TIVER ESCRITO NENHUM MOVIMENTO 'P', FÁ-LO NESTE MOMENTO*/
      cntp++;
    }

    if(graph->v_vert[prox].tipo != RAMPA && graph->v_vert[pos].tipo == RAMPA) /*QUANDO SOBE/DESCE UMA RAMPA, SÓ NUM DOS INSTANTES É QUE O TEMPO VALE 2*/
    (carro->tb)--;

    /*ATUALIZA O TEMPO TOTAL DE MOVIMENTO DO CARRO, EM TB*/
    if(tempo == 0)
      (carro->tb)++;
    else
      carro->tb+=tempo;
  }

  return;
}

/******************************************************************************
 * tempo_direcao()
 *
 * Arguments: Park configuration, Graph, 3 coordenadas, vertice anterior e proximo vertice
 * Returns: 1 se mudar de direcao, 2 se mudar de piso, 0 se nenhuma das anteriores
 *
 * Description: verifica mudanças de direção e retorna o tempo demorado
 *
 *****************************************************************************/
int tempo_direcao(Parque* park, Graph *graph, int x, int y, int z, int ant, int prox)
{
  int xant=0, yant=0, zant=0;
  int xprox=0, yprox=0, zprox=0;

  GRAPHfindC(graph, prox, &xprox, &yprox, &zprox);
  GRAPHfindC(graph, ant, &xant, &yant, &zant);

  if(park->cfg[x][y][z] == 'u' || park->cfg[x][y][z] == 'd')
    return 2;

  if(xant != xprox && yant != yprox)
    return 1;

  return 0;
}

/******************************************************************************
 * limpa_rest()
 *
 * Arguments: Graph, lista de restrições
 * Returns: -
 *
 * Description: limpa as restrições
 *
 *****************************************************************************/

int limpa_rest(Graph *graph, LinkedList *l_rest, car* new_car)
{
  LinkedList *aux;
  rest *r;
  int i, v=0;
  int n_lug = 0;

  for(aux = l_rest; aux != NULL; aux=getNextNodeLinkedList(aux))
    if(((rest *)getItemLinkedList(aux))->tb < new_car->tb)
    {
      r = ((rest *)getItemLinkedList(aux));

      if(r->piso != -1)
      {
        for(i=0 ; i<graph->V; i++)
          if(graph->v_vert[i].z == r->piso && graph->v_vert[i].tipo == LUGAR)
          {
            graph->v_vert[i].rest = LIVRE;
            n_lug++;
          }
      }
      else{
        v= GRAPHfindV(graph, r->x, r->y, r->z);
        graph->v_vert[v].rest = LIVRE;
        n_lug = 1;
      }
    }

    return n_lug;
}

/******************************************************************************
 * v_rest()
 *
 * Arguments: Graph, lista de restrições
 * Returns: -
 *
 * Description: verifica novas restrições
 *
 *****************************************************************************/

int v_rest(Graph *graph, LinkedList *l_rest, car* new_car)
{
  LinkedList *aux;
  rest *r;
  int i, v=0;
  int n_lug = 0;

  for(aux = l_rest; aux != NULL; aux=getNextNodeLinkedList(aux))
  {
    r = ((rest *)getItemLinkedList(aux));
    if(r->ta <= new_car->tb && (r->tb > new_car->tb || r->tb == 0))
    {
      if(r->piso != -1)
      {
        for(i=0 ; i<graph->V; i++)
          if(graph->v_vert[i].z == r->piso && graph->v_vert[i].tipo == LUGAR)
          {
            n_lug ++;
            graph->v_vert[i].rest = OCUPADO;
          }
      }
      else{
        v= GRAPHfindV(graph, r->x, r->y, r->z);
        graph->v_vert[v].rest = OCUPADO;
        n_lug = 1;
      }
    }
  }

  return n_lug;
}

/******************************************************************************
 * temp_rest()
 *
 * Arguments: Graph, lista de restrições
 * Returns: -
 *
 * Description: verifica se alguma restrição caducou, aquando a saída de um carro
 *
 *****************************************************************************/

int temp_rest(Graph *graph, LinkedList *l_rest, car* new_car, int temp)
{
  LinkedList *aux;
  rest *r;
  int v=0;

  for(aux = l_rest; aux != NULL; aux=getNextNodeLinkedList(aux))
    if(((rest *)getItemLinkedList(aux))->tb < new_car->tb)
    {
      r = ((rest *)getItemLinkedList(aux));

      if(r->piso != -1)
      {
        if(r->tb < temp)
          temp = r->tb;
      }
      else{
        v= GRAPHfindV(graph, r->x, r->y, r->z);
        if(graph->v_vert[v].tipo == LUGAR)
          temp = r->tb;
      }
    }

    return temp;
}