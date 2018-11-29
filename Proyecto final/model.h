#ifndef MODEL_H
#define MODEL_H

#include <algorithm>
#include <utility>
#include <iostream>
#include <string>
#include <cuda.h>

using namespace std;

struct nodeh {
  pair <string, int> *arr;
  nodeh *next;
  int last_pos;
  int size;
  __host__ __device__
  nodeh (int tam) {
    last_pos = 0;
    size = tam;
    arr = new pair <string, int> [tam];
    next = 0;
  }
  __host__ __device__
  float add (pair <string, int> to_add) {
    if (last_pos == size && next == 0) {
      return 1; // to transfer
    }
    else if (last_pos == size) {
      transfer();
    }
    arr[last_pos] = to_add;
    last_pos++;
    return 0; //all ok
  }
  __host__ __device__
  float transfer () {
    if (next == 0) {
      return 1; // updater
    }
    for (int i = 0; i < size; i++) {
      next -> add (arr[i]);
    }
    last_pos = 0;
    return 0;
  }
  __host__ __device__
  float search (string to_search) {
    for (int i = last_pos - 1; i >= 0; --i) {
      if (arr[i].first == to_search) {
        return 1.0;
      }
    }
    return 0.0;
  }
};


struct modelh {
  nodeh *head;
  __host__ __device__
  modelh () {
    int mean = 8;
    int tam = 5;
    int acum = 1;
    head = new nodeh(mean * acum);
    nodeh **temp = & (head -> next);
    --tam;
    acum *= 2;
    while (tam > 0) {
      (*temp) = new nodeh(mean * acum);
      temp = & ((*temp) -> next);
      acum *= 2;
      --tam;
    }
  }
};

#endif
