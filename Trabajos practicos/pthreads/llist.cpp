#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <pthread.h>
#include <vector>
#include <iostream>
#include <time.h>

#define siz 100
#define NUM_THREADS	1

using namespace std;


struct node {
  int data;
  node* next;
  node(int val){
    data = val;
    next = 0;
  }
  ~node(){
    delete next;
    next=0;
  }
};
struct llist{
  node* head;
  llist(){
    head=0;
  }
  bool insert(int num, int EXP_pos){
    int i = 0;
    node** temp = &head, **prev = &head;
    while (i <= EXP_pos) {
      if ((*temp) != 0 && (*temp) -> data > num) {
        (*prev) -> next = new node(num);
        (*prev) -> next -> next = (*temp);
        return 1;
      }
      if (i < EXP_pos) {
        while ((*temp) == 0) {
          sched_yield();
        }
        if ((*temp) -> data > num) {
          (*prev) -> next = new node(num);
          (*prev) -> next -> next = (*temp);
        }
        else {
          ++i;
          (*prev) = (*temp);
          (*temp) = (*temp) -> next;
        }
      }
      else if (i == EXP_pos){
        if ((*temp) == 0) {
          (*temp) = new node(num);
          return 1;
        }
      }
    }

  }
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
      cout << "//" << endl;
      return;
    }
    cout << head -> data << " -> ";
    print_l(head -> next);
  }
}milista;
int a[siz];
///
void *fun(void *rank) {
  long my_rank = (long) rank;
  for (int i = my_rank; i < siz; i += NUM_THREADS) {
    milista.insert(i, i);
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
  for (int i = 0; i < siz; i++) {
    a[i] = rand();
  }
  ///
  clock_t o=clock();
  ///
  for(t=0; t<NUM_THREADS; t++) {
     printf("Main: creating thread %ld\n", t);
     rc = pthread_create(&thread[t], &attr, fun, (void *)t);
     if (rc) {
        printf("ERROR; return   code from pthread_create() is %d\n", rc);
        exit(-1);
        }
     }

  /* Free attribute and wait for the other threads */
  pthread_attr_destroy(&attr);
  for(t=0; t<NUM_THREADS; t++) {
   rc = pthread_join(thread[t], &status);
   if (rc) {
    printf("ERROR; return code from pthread_join() is %d\n", rc);
    exit(-1);
    }
   //printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
  }
  milista.print_l();
}
