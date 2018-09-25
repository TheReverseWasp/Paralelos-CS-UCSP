#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_THREADS	8
#define n 10000000
long acum = 0;
long a[n]{1};
int flag = 0;

void *BusyWork(void *rank)
{
  long my_rank = (long) rank;
  long long i;
  long long my_n = n/NUM_THREADS;
  long long my_first_i = my_n*my_rank;
  long long my_last_i = my_first_i + my_n;


  for (i = my_first_i; i < my_last_i; i++) {
    while (flag != my_rank);
    acum += a[i];
    flag = (flag+1) % NUM_THREADS;
    }
}

int main (int argc, char *argv[])
{
  pthread_t thread[NUM_THREADS];
  pthread_attr_t attr;
  int rc;
  long t;
  void *status;
  for (int i = 0; i < n; i++) {
    a[i] = 1;
  }
  /* Initialize and set thread detached attribute */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  clock_t o=clock();
  for(t=0; t<NUM_THREADS; t++) {
     printf("Main: creating thread %ld\n", t);
     rc = pthread_create(&thread[t], &attr, BusyWork, (void *)t);
     if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
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
  printf("La suma es: %ld\n", acum);
  printf("Main: program completed. Exiting.\n");
  o = clock() - o;
  printf ("BW took me %d clicks (%f seconds).\n",o,((float)o)/CLOCKS_PER_SEC);
  pthread_exit(NULL);
}
