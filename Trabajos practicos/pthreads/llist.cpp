#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <pthread.h>
#include <vector>
#include <iostream>
#include <time.h>

#define siz 100000
#define NUM_THREADS	4

using namespace std;

pthread_mutex_t mtx;

struct node {
  int data;
  node* next;
  node(int val){
    data = val;
    next = 0;
  }
  ~node(){
    delete next;
    next = 0;
  }
};
struct llist{
  node* head;
  ///
  llist(){
    head = 0;
  }
  bool insert(int num){
    node *temp, **temp2;
    bool ok = 0;
    temp2 = &head;
    while (ok == 0) {
      pthread_mutex_lock(&mtx);
      if ((*temp2) == 0) {
        (*temp2) = new node(num);
        ok = 1;
      }
      else if ((*temp2) -> data > num) {
        temp = (*temp2);
        (*temp2)= new node(num);
        (*temp2) -> next = temp;
        ok = 1;
      }
      pthread_mutex_unlock(&mtx);
      if (ok == 0) {
        temp2 = &((*temp2) -> next);
      }
    }
    return 0;
  }
  ///
  void print_l(node* runner){
    if (runner == 0) {
      cout << "//" << endl;
      return;
    }
    cout << runner -> data << " -> ";
    print_l(runner -> next);
  }
  void print_l(){
    if (head == 0) {
      cout << "vacio" << endl;
      return;
    }
    cout << head -> data << " -> ";
    print_l(head -> next);
  }
}milista;
///
void *fun(void *rank) {
  long my_rank = (long) rank;
  for (int i = my_rank; i < siz; i += NUM_THREADS) {
    //cout << i << " ";
    milista.insert(i);
  }
  return NULL;
}

int main(int argc, char const *argv[]) {
  pthread_t thread[NUM_THREADS];
  pthread_attr_t attr;
  int rc;
  long t;
  void *status;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  srand(time(NULL));
  ///
  clock_t o=clock();
  ///
  if (pthread_mutex_init(&mtx, NULL) != 0)
  {
      printf("\n mtx init failed\n");
      return 1;
  }
  ///
  for(t = 0; t < NUM_THREADS; t++) {
     printf("Main: creating thread %ld\n", t);
     rc = pthread_create(&thread[t], &attr, fun, (void *)t);
     if (rc) {
        printf("ERROR; return   code from pthread_create() is %d\n", rc);
        exit(-1);
        }
     }

  /* Free attribute and wait for the other threads */
  pthread_attr_destroy(&attr);
  for(t = 0; t<NUM_THREADS; t++) {
   rc = pthread_join(thread[t], &status);
   if (rc) {
    printf("ERROR; return code from pthread_join() is %d\n", rc);
    exit(-1);
    }
   //printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
  }
  milista.print_l();
  pthread_mutex_destroy(&mtx);
}
