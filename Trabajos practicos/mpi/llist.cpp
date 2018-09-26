#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <time.h>

#define siz 100

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
        while ((*temp) == 0);
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
      else{
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
};

int main(int argc, char const *argv[]) {
  int comm_sz;
  int my_rank;
  int acum;
  int a[siz];
  llist mi_lista;
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  if (my_rank != 0) {
    for (int i = my_rank - 1; i < siz; i+=comm_sz) {
      mi_lista.insert(i,i);
    }
    mi_lista.print_l(mi_lista.head);
  }
  if (my_rank == 0) {
    mi_lista.print_l(mi_lista.head);
  }
  return 0;
}
