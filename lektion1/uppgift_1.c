#include <stdio.h>
#include <stdlib.h>

double *create_double_array(int n);
double **create_pointer_vector(int n);

#define NUMROWS 3
#define NUMCOLS 4

int main(){
    double **ptr;
    ptr = create_pointer_vector(NUMROWS);
    for (int i=0; i<NUMROWS; i++){
        ptr[i] = create_double_array(NUMCOLS);
    }

    for (int i=0; i<NUMROWS; i++){
        for (int j=0; j<NUMCOLS; j++){
            fprintf(stderr, "%d,%d",i, j);
            printf("%f ", ptr[i][j]);
        }
        printf("\n");
    }

    //Free memory
    for (int i=0; i<NUMROWS; i++){
        free(ptr[i]);
    }
    free(ptr);

    return 0;
}

double *create_double_array(int n){
    return calloc(n, sizeof(double));
}

double **create_pointer_vector(int n){
    return calloc(n, sizeof(double*));
}
