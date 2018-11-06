#include <stdio.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <omp.h>
using namespace std;
void amax(int vrand[], int x){
    for(int i=0;i<x;i++){
        int nrand=1;
        vrand[i]=nrand;
    }
}

void fillmatrix(int** vrand, int x){
    for(int i = 0; i <x; i++){
        amax(vrand[i],x);
    }
}


int main(int argc,char* argv[]){
    int thread_count=strtol(argv[1],NULL,10);
    string sz=argv[2];
    int size=atoi( sz.c_str());;
    int **matrix;
    int vect[size],answer[size];
    amax(vect,size);
    matrix = new int *[size];
    for(int i = 0; i <size; i++){
        matrix[i] = new int[size];
    }
    fillmatrix(matrix,size);

    int i,j;
    clock_t tStart = clock();

    # pragma omp parallel for num_threads(thread_count) default(none) private(i,j) shared(matrix,vect,answer,size)
    for(i=0;i<size;i++){
        answer[i]=0.0;
        for(j=0;j<size;j++){
            answer[i]+=matrix[i][j]*vect[j];
        }
    }
    cout<<"matrix time obrilion masta of"<< (double)(clock() - tStart)/CLOCKS_PER_SEC<<endl;
    return 0;
}
