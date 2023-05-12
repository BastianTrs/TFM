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
    if (v == NULL){
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
    if(v == NULL){
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
	if(m == NULL){
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
	if(m == NULL){
        printf("Error: no se pudo asignar memoria.\n");
        exit(EXIT_FAILURE);
    }
	return m;
}
double maxval(double* arr, int size){
    double max = arr[0];
    for(int i = 1; i < size; i++){
        if(arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

double minval(double* arr, int size){
    double min = arr[0];
    for(int i = 1; i < size; i++){
        if(arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}
void max_min_weights(double* w_h, double* w_o, int* nnh, int nh, int ni, int no){
    double max_w_h = w_h[0];
    double min_w_h = w_h[0];
    double max_w_o = w_o[0];
    double min_w_o = w_o[0];
	printf("initial min_w_h:%lf\n", min_w_h);
	printf("initial max_w_h:%lf\n", max_w_h);
	printf("initial min_w_o:%lf\n", min_w_o);
	printf("initial max_w_o:%lf\n", max_w_o);
    // Para los pesos de las capas ocultas
	for(int i = 0; i < nh; i++) {
	    max_w_h = w_h[i];
        min_w_h = w_h[i];
		for(int j = 0; j < nnh[i]; j++) {
		    if(i == 0) {
		        for(int k = 0; k < ni; k++) {  
		            if(*(w_h + i * nnh[i] * ni + j * ni + k) > max_w_h) {
		                max_w_h = *(w_h + i * nnh[i] * ni + j * ni + k);
		            }
		            if(*(w_h + i * nnh[i] * ni + j * ni + k) < min_w_h) {
		                min_w_h = *(w_h + i * nnh[i] * ni + j * ni + k);
		            }
		        }
		    } 
		    else{
		        for(int k = 0; k < nnh[i - 1]; k++) {       
		            if(*(w_h + i * nnh[i - 1] * nnh[i] + k * nnh[i] + j) > max_w_h) {
		                max_w_h = *(w_h + i * nnh[i - 1] * nnh[i] + k * nnh[i] + j);
		            }
		            if(*(w_h + i * nnh[i - 1] * nnh[i] + k * nnh[i] + j) < min_w_h) {
		                min_w_h = *(w_h + i * nnh[i - 1] * nnh[i] + k * nnh[i] + j);
		            }
		        }
		    }
		    //printf("W  I-H1: Max, Min: %lf %lf\n", max_w_h, min_w_h);
		}
		if(i==0)
		printf("W  I-H1: Max, Min: %lf %lf\n", max_w_h, min_w_h);
		if(i==1)
		printf("W H1-H2: Max, Min: %lf %lf\n", max_w_h, min_w_h);
	}
    // Para los pesos de la capa de salida
    for(int i = 0; i < no; i++){
        for(int j = 0; j < nnh[nh-1]; j++) {
            if(*(w_o + i + j*no ) > max_w_o){
                max_w_o = *(w_o + i + j*no );
            }
            if(*(w_o + i + j*no ) < min_w_o){
                min_w_o = *(w_o + i + j*no );
            }
        }
    }
    printf("W  H2-O: Max, Min: %lf %lf\n", max_w_o, min_w_o);
}

double sigmoid(double x){

	double sigmoid = (1.0/(1.0 + exp(-x)));
	return sigmoid;
}
double dsigmoid(double x){

	double dsigmoid = sigmoid(x) * (1.0 - sigmoid(x));
	return dsigmoid;
}


