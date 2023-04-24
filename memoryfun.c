#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "memoryfun.h"

double *dvector(int nh)
/* Dimensiona y reserve espacio para un vector double de rango [n1..nh] */
{
    double *v;
    v=(double *) malloc((nh)*sizeof(double));
    if (v == NULL) {
        printf("Error: no se pudo asignar memoria.\n");
        exit(EXIT_FAILURE);
    }
    return v;
}

int *ivector(int nh)
/* Dimensiona y reserve espacio para un vector double de rango [n1..nh] */
{
    int *v;
    v=(int *) malloc((nh)*sizeof(int));
    if (v == NULL) {
        printf("Error: no se pudo asignar memoria.\n");
        exit(EXIT_FAILURE);
    }
    return v;
}

double *dmatrix (int nfh, int nch) 
/* Dimensiona y reserva espacio para una matriz double [nf1..nfh][nc1..nch] */
{
	double *m;
	m=(double *)malloc((nfh)*(nch)*sizeof(double));
	if (m == NULL) {
        printf("Error: no se pudo asignar memoria.\n");
        exit(EXIT_FAILURE);
    }
	return m;
}

int *imatrix (int nfh, int nch) 
/* Dimensiona y reserva espacio para una matriz int [nf1..nfh][nc1..nch] */
{
	int *m;
	m=(int *)malloc((nfh)*(nch)*sizeof(int));
	if (m == NULL) {
        printf("Error: no se pudo asignar memoria.\n");
        exit(EXIT_FAILURE);
    }
	return m;
}
double maxval(double* arr, int size) {
    double max = arr[0];
    for(int i = 1; i < size; i++) {
        if(arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

double minval(double* arr, int size) {
    double min = arr[0];
    for(int i = 1; i < size; i++) {
        if(arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}
