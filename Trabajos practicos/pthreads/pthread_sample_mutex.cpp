#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUM_THREADS	4
#define n 10000000
long acum = 0;
long a[n];
int flag = 0;
pthread_mutex_t mtx;

using namespace std;

///
void *BusyWork(void *rank) {
  long my_rank = (long) rank;
  long long i;
  long long my_n = n/NUM_THREADS;
  long long my_first_i = my_n*my_rank;
  long long my_last_i = my_first_i + my_n;

  double my_sum = 0.0;
  for (i = my_first_i; i < my_last_i; i++) {
    my_sum += a[i];
  }
  pthread_mutex_lock(&mtx);
  acum += my_sum;
  pthread_mutex_unlock(&mtx);
  return NULL;
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
  ///
  if (pthread_mutex_init(&mtx, NULL) != 0)
  {
      printf("\n mtx init failed\n");
      return 1;
  }
  ///
  for(t=0; t<NUM_THREADS; t++) {
     printf("Main: creating thread %ld\n", t);
     rc = pthread_create(&thread[t], &attr, BusyWork, (void *)t);
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
  printf("La suma es: %ld\n", acum);
  printf("Main: program completed. Exiting.\n");
  pthread_mutex_destroy(&mtx);
  pthread_exit(NULL);
}
