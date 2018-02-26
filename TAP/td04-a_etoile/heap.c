#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

heap heap_create(int n, int (*f)(const void *, const void *)){
  heap h = malloc(sizeof(*h));
  h->nmax = n;
  h->size = 0;
  void * t =  malloc(sizeof(void *)*(h->nmax+1));
  h->array = t; 
  h->f=*f;
  return h;
}

void heap_destroy(heap h){
  free(h->array);
  free(h);
}


int heap_empty(heap h){
  return h->size;
}

int heap_add(heap h, void *object){
  if(h->nmax == h->size) return 1;
  h->array[h->size+1]=object;
  h->size+=1;

  int i = h->size;
  while(i>1){
    if((h->f(h->array[i], h->array[i/2]))<0){
      void *tmp=h->array[i/2];
      h->array[i/2] = h->array[i];
      h->array[i] = tmp;
      i=i/2;
    } else {
      break;
    }
  }
  return 0;
}


void *heap_top(heap h){
  if(h->size==0)
    return NULL;
  return h->array[1];
}


void *heap_pop(heap h){
  if(h->size==0)
    return NULL;
  
  int i=1;
  void *res = h->array[i];
  h->array[i] = h->array[h->size];
  h->size--;
  void *tmp;
    
  while(i<=h->size){//while
    
    if(2*i>h->size){ //si pas de fils
      return res;
    }else if(2*i==h->size){//si un seul fils
      if((h->f(h->array[i], h->array[2*i]))>0){
	tmp = h->array[2*i];
	h->array[2*i] = h->array[i];
	h->array[i] = tmp;
      }
      return res;
    }else{
      if(h->f(h->array[2*i], h->array[2*i+1])<0){
	if(h->f(h->array[i], h->array[2*i])<0) return res;
	tmp = h->array[2*i]; 
	h->array[2*i] = h->array[i];
	h->array[i] = tmp;
	i=i*2;
      }else{
	if(h->f(h->array[i], h->array[2*i+1])<0) return res;
	tmp = h->array[2*i+1];
	h->array[2*i+1] = h->array[i];
	h->array[i] = tmp;
	i=2*i+1;
      }
    }
  }
  return res;
}
