#include <stdio.h>
#include <stdlib.h>

// GENERAL VARIABLES
int i, j, k, ii, jj, kk, iii, jjj, kkk, t;
int ep, dum, err, x, xx;
int k1, k2;

// TIMING VARIABLES
double start_time, stop_time;

// EXPERIMENT AND DATASET STRUCTURE
int epochs; // number of epochs
int ntrain; // number of training samples

// NET ARCHITECTURE
int ni; // number of input neurons
int no; // number of output neurons
int nh; // number of hidden layers; cannot exceed hgs
int n1, n2, n3, n4; // number of neurons in hidden layer. Add here if more are required

// NEURONS (UNITS)
double *input; //2 dimensions array
double *hidden; //2 dimensions array
double *output; //2 dimensions array
double *target; //2 dimensions array
int *nnh; //1 dimensions array
int *patron; //1 dimensions array

// WEIGHTS AND SUMS FOR THE HIDDEN LAYER
double* w_h; //3 dimensions array
double* dw_h; //3 dimensions array
double* b_h; //2 dimensions array
double* db_h; //2 dimensions array

// WEIGHTS AND SUMS FOR THE OUTPUT LAYER
double* w_o; //2 dimensions array
double* dw_o; //2 dimensions array
double* b_o; //1 dimension array
double* db_o; //1 dimension array

// ERROR DELTAS
double* delta_h; //2 dimension array
double* delta_o; //1 dimension array

// FOR BATCH TRAINING
int batchnumber, batchsize, batch;

// SPLIT BETWEEN TRAINING ANT TESTING
float percen, trozo;

// EPSILON, TRAINING AND TESTING ERRORS
double erroradmisible, sse1, sse2;

// TRAINING PARAMETERS
double rate, mo;

// ACTIVATION FUNCTIONS, DERIVATIVES
double s, sigmoid, dsigmoid, ReLu;

// CHARACTERS FOR INPUT
char tamano[12];
char etiqueta[40];

int main(int argc, char* argv[]) {

    char command[100];
    sprintf(command, "wc -l train.txt | awk '{print $1}'");
    ntrain = atoi(system(command));
    // Aquí se podría realizar la asignación de memoria correspondiente para los datos de entrenamiento
    return 0;

}