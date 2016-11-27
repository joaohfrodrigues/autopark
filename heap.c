/******************************************************************************
 *
 * File Name: heap.c
 * Author:    João Rodrigues, Sara Vieira
 * Revision:  v1.0
 *
 * DESCRIPTION
 *		funções do acervo
 *
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "heap.h"


/******************************************************************************
 * FixUp()
 *
 * Arguments: h - pointer to heap structure
 *            k - index of element to fixup
 * Returns:
 * Side-Effects: none
 *
 * Description: Performs fixup from index
 *
 *****************************************************************************/

void FixUp(Heap * h, int k)
{
  data t;
  while ((k > 0) && (h->less) ((h->heapdata)[(k - 1) / 2].dist, (h->heapdata)[k].dist)) 
  {
    t = (h->heapdata)[k];
    (h->heapdata)[k] = (h->heapdata)[(k - 1) / 2];
    (h->heapdata)[(k - 1) / 2] = t;
    k = (k - 1) / 2;
  }

  return;
}


/******************************************************************************
 * FixDown()
 *
 * Arguments: h - pointer to heap structure
 *            k - index of element to fixdown
 * Returns:
 * Side-Effects: none
 *
 * Description: performs fixdown from index
 *
 *****************************************************************************/

void FixDown(Heap * h, int k)
{
  int j;
  data t;

  while ((2 * k + 1) < h->n_elements) {
    j = 2 * k + 1;
    if (((j + 1) < h->n_elements) && (h->less) (h->heapdata[j].dist, h->heapdata[j + 1].dist)) 
    {
      /* second offspring is greater */
      j++;
    }
    if (!(h->less) (h->heapdata[k].dist, h->heapdata[j].dist))
   {
      /* Elements are in correct order. */
      break;
    }
    /* the 2 elements are not correctly sorted, it is
       necessary to exchange them */
    t = (h->heapdata)[k];
    (h->heapdata)[k] = (h->heapdata)[j];
    (h->heapdata)[j] = t;
    k = j;
  }

  return;
}


/******************************************************************************
 * NewHeap()
 *
 * Arguments: size - heap size
 *            less - item comparison function
 * Returns: Heap
 * Side-Effects: none
 *
 * Description: allocates space for a new heap of specified size
 *
 *****************************************************************************/

Heap *NewHeap(int size, int (*less) (int, int))
{
  Heap *h;

  h = (Heap *) malloc(sizeof(Heap));
  h->n_elements = 0;
  h->less = less;
  h->heapdata = (data *) malloc(size * sizeof(data));
  return (h);
}

/******************************************************************************
 * FreeHeap()
 *
 * Arguments: Heap
 * Returns: -
 * Side-Effects: none
 *
 * Description: frees the memory used
 *
 *****************************************************************************/

void FreeHeap(Heap *h)
{
  free(h->heapdata);
  free(h);

}

/******************************************************************************
 * Insert()
 *
 * Arguments: h - pointer to heap
 *            distance, indice do vertice
 * Returns: 1
 * Side-Effects: none
 *
 * Description: add element at the end of heap and do fixup
 *
 *****************************************************************************/

int Insert(Heap * h, int dist, int id)
{
  h->heapdata[h->n_elements].dist = dist;
  h->heapdata[h->n_elements].id = id;

  h->n_elements++;
  FixUp(h, h->n_elements - 1);

  return 1;
}

/******************************************************************************
 * Modify()
 *
 * Arguments: h - pointer to heap
 *            id- id do vertice modificado
 *            newvalue - new element value
 * Returns: void
 * Side-Effects: none
 *
 * Description: compares new element with antecedent, if larger do a fixup
 *              otherwise do fixdown
 *
 *****************************************************************************/

void Modify(Heap * h, int id, int newvalue)
{
  int index = GetIndex(h, id);
  h->heapdata[index].dist = newvalue;
  FixUp(h, index);
  return;
}


/******************************************************************************
 * RemoveMax()
 *
 * Arguments: h - pointer to heap
 * Returns: int
 * Side-Effects: none
 *
 * Description: exchange first and last element of heap, remove last element
 *              (max element) from heap and maintain heap order by performing
 *              a fixdown of the first element
 *
 *****************************************************************************/

int RemoveMax(Heap * h)
{
  data t;

  if (h->n_elements > 0) {
    t = (h->heapdata)[0];
    (h->heapdata)[0] = (h->heapdata)[h->n_elements - 1];
    (h->heapdata)[h->n_elements - 1] = t;
    h->n_elements--;
    FixDown(h, 0);
    return t.id;
  }

  return -1;
}


/******************************************************************************
 * GetIndex()
 *
 * Arguments: h - pointer to heap structure
 *            k - index of element to fixdown
 * Returns:
 * Side-Effects: none
 *
 * Description: performs fixdown from index
 *
 *****************************************************************************/

int GetIndex(Heap * h, int id)
{
  int j, k = 0;
  /*printf("n_elements: %d, id: %d\n", h->n_elements, id);*/

  if((h->heapdata)[0].id == id)
    return 0;

  while ((2 * k + 1) < h->n_elements) {
    j = 2 * k + 1;

    if (h->heapdata[j].id == id) 
      return j;

    if (h->heapdata[j+1].id == id)
      return j+1;
   
    k++;
  }

  return -1;
}
