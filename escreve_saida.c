/********************************************************************
* File: escreve_saida.c
*
* Date: 30-04-2009
*
* Description: funcao auxiliar fornecida pelo corpo docente
*              de AED 2008/2009 para o projecto final "Parking Garage".
*
* Nota: a funcao nao fecha o ficheiro!!
*
* History:
*
* Bug-report:  lms@inesc-id.pt
*
* Copyright (c) 2008 IST/DEEC/AED - Algoritmos e Estruturas de Dados
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID_SIZE 128


/******************************************************************************
 * escreve_saida ()
 *
 * Arguments: fp - descriptor do ficheiro de saida
 *            vid - identificador da viatura
 *            tk - instante de tempo em que ocorre o movimento
 *            pX, pY, pZ - coordenadas (X,Y,Z) da viatura em movimento
 *            tm - tipo de movimento
 * Returns: 0 - se nao houver qualquer erro, ou seja se as coordenadas
 *              corresponderem a uma posicao valida
 *          -1 - se houver algum erro
 * Side-Effects: nenhum
 *
 * Description: escreve no ficheiro de saida um tuplo de valores do tipo
 *                          Vid T X Y Z M
 *              sendo Vid o identificador da viatura, T o instante de tempo,
 *              X, Y e Z  indicam a posicao da viatura no parque de
 *              estacionamento e M e' o tipo de movimento podendo apenas ter
 *              os caracteres 'i', 'm', 'e', 'p', 'a', 'x' ou 's'.
 *
 *****************************************************************************/

int escreve_saida(FILE *fp, char *vid, int tk, int pX, int pY, int pZ, char tm)
{
  static char pvid[MAX_ID_SIZE];
  static int ptk = -1, ppx, ppy, ppz; /*savep = 1;*/

  int retval = 0;    /* valor retornado quando n�o h� erro;
                      * se houver erro retval = -1
                      */
  char *bogus = "??";

  if(fp == NULL) {
    fprintf(stderr, "Ficheiro invalido.\n");
    exit(1);
  }

  /* check for valid range of values and valid type of move */
  if (vid == NULL) {
    vid = bogus;
    fprintf(stderr, "Argumentos invalidos: Identificador de viatura nulo!\n");
    fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
            vid, tk, pX, pY, pZ, tm);
    /*savep = 0;*/
    retval = -1;
  } else if (tk < 0) {
    fprintf(stderr, "Argumentos invalidos: tempo negativo!\n");
    fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
            vid, tk, pX, pY, pZ, tm);
    retval = -1;
  } else if (pX <0 || pY <0 || pZ <0) {
    fprintf(stderr, "Argumentos invalidos: coordenadas erradas!\n");
    fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
            vid, tk, pX, pY, pZ, tm);
    retval = -1;
  } else if (tm != 'i' && tm != 'x' && tm != 'e' && tm != 'm' && tm != 'p'
             && tm != 's' && tm != 'a') {
    fprintf(stderr, "Argumentos invalidos: tipo de movimento!\n");
    fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
            vid, tk, pX, pY, pZ, tm);
    retval = -1;
  } else {
    if (ptk != -1) { /* 2nd execution */
      /* Note: this is abusive; this assumes that identifiers for different
       * are in different memory locations.
       * If this is not the case, comment out this test!
       */
      if (!strcmp(vid, pvid)) {
        /* same vehicle as in last call; exclude summary and exit lines */
        if ((tm == 'i') || (tm == 'm') || (tm == 'e') ||
            (tm == 'p') || (tm == 'a')) {
          if (tk <= ptk) {
            /* invalid tk */
            fprintf(stderr, "tk deve ser maior que %d.\n", ptk);
            fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
                    vid, tk, pX, pY, pZ, tm);
            retval = -1;
          } else  if (pZ == ppz && pX == ppx && pY == ppy) {
            /* it did not move at all */
            fprintf(stderr, "Viatura %s n�o se moveu.\n", vid);
            fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
                    vid, tk, pX, pY, pZ, tm);
            retval = -1;
          } else if (pZ == ppz && pX != ppx && pY != ppy) {
              /* diagonal move on same floor*/
              fprintf(stderr, "Movimento invalido: ");
              fprintf(stderr,
                      "linha e coluna nao podem mudar em simultaneo.\n");
              fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
                      vid, tk, pX, pY, pZ, tm);
              retval = -1;
            } else if (pZ != ppz && (pX != ppx || pY != ppy)) {
              /* level changes with some lateral move*/
              fprintf(stderr, "Movimento invalido: ");
              fprintf(stderr,
                      "linha e coluna devem ser iguais ao nivel anterior.\n");
              fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
                      vid, tk, pX, pY, pZ, tm);
              retval = -1;
            }
        }
      }
    }
  }
  if (retval == 0) {
    /* keep info from this run through */
    strncpy(pvid, vid, MAX_ID_SIZE);
    ptk = tk;
    ppx = pX; ppy = pY; ppz = pZ;

    /* generate output */
    fprintf(fp, "%s %d %d %d %d %c\n", vid, tk, pX, pY, pZ, tm);
  }

  return(retval);
}
/*end of function */


