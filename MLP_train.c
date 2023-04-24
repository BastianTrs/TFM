#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "memoryfun.h"
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

	//OBTENEMOS LA CANTIDAD DE DATOS DE ENTRENAMIENTO
	int lines = 0;
    FILE *trainFile = fopen(argv[1], "r");
    if (!trainFile) {
        printf("Error: no se pudo abrir el archivo.\n");
        return 1;
    }
    while ((lines = fgetc(trainFile)) != EOF) {
        if (lines == '\n') {
            ntrain++;
        }
    }
    fclose(trainFile);
    printf("\n----NUMERO DE DATOS DE ENTRENAMIENTO----\n");
    printf("ntrain:%d\n",ntrain);
    
    //LEEMOS LOS PARAMETROS DEL ARCHIVO "params.dat"
    FILE *fp = fopen("params.dat", "r");
    if (fp == NULL) {
        printf("Error opening params.dat file.\n");
        return 1;
    }

    fscanf(fp, "%d %d %d %d %d %d %d %s", &ni, &no, &nh, &n1, &n2, &n3, &n4, etiqueta);
    fscanf(fp, "%d %s", &epochs, etiqueta);
    fscanf(fp, "%lf %s", &rate, etiqueta);
    fscanf(fp, "%lf %s", &mo, etiqueta);
    fscanf(fp, "%lf %s", &erroradmisible, etiqueta);
    fscanf(fp, "%d %s", &batchsize, etiqueta);
    fscanf(fp, "%f %s", &percen, etiqueta);

    fclose(fp);

    // Imprime los valores leídos
    printf("\n----PARAMETROS Y ESTRUCTURA DE----\n");
    printf("ni = %d\n", ni);
    printf("no = %d\n", no);
    printf("nh = %d\n", nh);
    printf("n1 = %d\n", n1);
    printf("n2 = %d\n", n2);
    printf("n3 = %d\n", n3);
    printf("n4 = %d\n", n4);
    printf("epochs = %d\n", epochs);
    printf("rate = %lf\n", rate);
    printf("mo = %lf\n", mo);
    printf("erroradmisible = %lf\n", erroradmisible);
    printf("batchsize = %d\n", batchsize);
    printf("percen = %f\n", percen);

    //ASIGNAMOS MEMORIA DINAMICA
    nnh = ivector(4); 
	nnh[0] = n1;
	nnh[1] = n2;
	nnh[2] = n3;
	nnh[3] = n4;
	printf("nnh[0]:%d\n", nnh[0]);
	printf("nnh[1]:%d\n", nnh[1]);
	printf("nnh[2]:%d\n", nnh[2]);
	printf("nnh[3]:%d\n", nnh[3]);
	
	//input = (double*) malloc(ntrain * ni * sizeof(double));
	input = dmatrix(ntrain, ni);
	//hidden = (double*) malloc(nh * nnh[0] * sizeof(double));
	hidden = dmatrix(nh, nnh[0]);
	//output = (double*) malloc(ntrain * no * sizeof(double));
	output = dmatrix(ntrain, no);
	//target = (double*) malloc(ntrain * no * sizeof(double));
	target = dmatrix(ntrain, no);
	//patron = (int*) malloc(ntrain * sizeof(int));
	patron = ivector(ntrain);

	w_h = (double*) malloc(nh * nnh[0] * nnh[0] * sizeof(double));
	dw_h = (double*) malloc(nh * nnh[0] * nnh[0] * sizeof(double));
	
	//b_h = (double*) malloc(nh * nnh[0] * sizeof(double));
	b_h = dmatrix(nh, nnh[0]);
	//db_h = (double*) malloc(nh * nnh[0] * sizeof(double));
	db_h = dmatrix(nh, nnh[0]);
	//w_o = (double*) malloc(nnh[nh-1] * no * sizeof(double));
	w_o = dmatrix(nnh[nh-1], no);
	//dw_o = (double*) malloc(nnh[nh-1] * no * sizeof(double));
	dw_o = dmatrix(nnh[nh-1], no);
	//b_o = (double*) malloc(no * sizeof(double));
	b_o = dvector(no);
	//db_o = (double*) malloc(no * sizeof(double));
	db_o = dvector(no);

	//delta_h = (double*) malloc(nh * nnh[0] * sizeof(double));
	delta_h = dmatrix(nh, nnh[0]);
	//delta_o = (double*) malloc(no * sizeof(double));
	delta_o = dvector(no);
	
	//LECTURA DE DATOS DE ENTRENAMIENTO
	printf("\n----LECTURA DE DATOS DE ENTRENAMIENTO----\n");
	int k1, k2;
	FILE *file = fopen("train.txt", "r");
	if (file == NULL) {
    	printf("Failed to open train.txt\n");
    	exit(EXIT_FAILURE);
	}
	
	for (int t = 0; t < ntrain; t++) {
		if (fscanf(file, "%d %d", &k1, &k2) != 2) {
		    printf("Failed to read input and target values from train.txt\n");
		    exit(EXIT_FAILURE);
		}
		for (int i = 0; i < ni; i++) {
		    if (fscanf(file, "%lf", &input[t*ni+i]) != 1) {
		        printf("Failed to read input values from train.txt\n");
		        exit(EXIT_FAILURE);
		    }
		}
		for (int j = 0; j < no; j++) {
		    if (fscanf(file, "%lf", &target[t*no+j]) != 1) {
		        printf("Failed to read target values from train.txt\n");
		        exit(EXIT_FAILURE);
		    }
		}
	}

	fclose(file);

	printf("INPUT : Maximum, minimum: %lf %lf\n", maxval(input, ntrain*ni), minval(input, ntrain*ni));
	printf("OUTPUT: Maximum, minimum: %lf %lf\n", maxval(target, ntrain*no), minval(target, ntrain*no));
	printf("\n");

	int batchnumber = ntrain / batchsize;
	printf("Number of batches: %d of %d (batchsize) cases each\n", batchnumber, batchsize);
	printf("\n");

	int trozo;
	if (percen == 0) {
		trozo = 0;
		printf("WARNING: NO VALIDATION DATA. REPORTED SSE IS BOGUS.\n");
		printf("\n");
	}

	trozo = percen * ntrain / 100;

	printf("%lf of data are for independent testing\n", percen);
	printf("\n");
	printf("Training: 1 to %d cases (shuffled each epoch)\n", batchnumber-trozo);
	printf("Testing: %d to %d cases (shuffled each epoch)\n", batchnumber-trozo, batchnumber);
	printf("\n");
	/*
	printf("INPUT:\n");
	for (int i = 0; i < ntrain; i++) {
		for (int j = 0; j < ni; j++) {
		    printf("%f ", input[i*ni + j]);
		}
		printf("\n");
	}
	printf("\nTARGET:\n");
	for (int i = 0; i < ntrain; i++) {
		for (int j = 0; j < no; j++) {
		    printf("%f ", target[i*no + j]);
		}
		printf("\n");
	}
	*/
	
	for(int i = 0; i < nh; i++){
    	for(int j = 0; j < nnh[i]; j++){
        	if(i == 0){
            	for(int k = 0; k < ni; k++){
                	w_h[i * nnh[i] * ni + j * ni + k] = 2.0 * 1.0 / sqrt((double)ni) * (rand() - 0.5);
            	}
        	} 
        	else{
            	for(int k = 0; k < nnh[i - 1]; k++){
                	w_h[i * nnh[i - 1] * nnh[i] + k * nnh[i] + j] = 2.0 * 1.0 / sqrt((double)nnh[i - 1]) * (rand() - 0.5);
            	}
        	}
    	}
    	b_h[i * nnh[i]] = 0.0;
    	db_h[i * nnh[i]] = 0.0;
	}

	for(int k = 0; k < no; k++){
		for(int j = 0; j < nnh[nh - 1]; j++) {
		    w_o[j * no + k] = 2.0 * 1.0 / sqrt((double)nnh[nh - 1]) * (rand() - 0.5);
		}
		b_o[k] = 0.0;
		db_o[k] = 0.0;
	}
	
    return 0;

}
