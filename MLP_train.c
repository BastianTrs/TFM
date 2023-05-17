#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "memoryfun.h"
#include <time.h>
// GENERAL VARIABLES
int i, j, k, ii, jj, kk, iii, jjj, kkk, t;
int ep, dum, err, x, xx;
int k1, k2;
double s;

// TIMING VARIABLES
clock_t start_time, stop_time;
//clock_t	start_time, stop_time;
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
double s, ReLu;

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
    start_time=clock();
    //OBTENEMOS TODOS LOS PARAMETROS DEL ARCHIVO "params.dat"
    printf("\n----NUMERO DE DATOS DE ENTRENAMIENTO----\n");
    printf("ntrain:%d\n",ntrain);
    
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

    //MOSTRAMOS PARAMETROS POR PANTALLA
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
	
	input = dmatrix(ntrain, ni);
	hidden = dmatrix(nh, nnh[0]);
	for(i=0;i<nh;i++)
     for(j=0;j<nnh[i];j++)hidden[i*nnh[i]+j]=-100.0;
	output = dmatrix(ntrain, no);
	target = dmatrix(ntrain, no);
	patron = ivector(ntrain);

	w_h = (double*) malloc(nh * nnh[0] * nnh[0] * sizeof(double));
	dw_h = (double*) malloc(nh * nnh[0] * nnh[0] * sizeof(double));
	
	b_h = dmatrix(nh, nnh[0]);
	db_h = dmatrix(nh, nnh[0]);
	w_o = dmatrix(nnh[nh-1], no);
	dw_o = dmatrix(nnh[nh-1], no);
	b_o = dvector(no);
	db_o = dvector(no);

	delta_h = dmatrix(nh, nnh[0]);
	for(i=0;i<nh;i++)
     for(j=0;j<nnh[i];j++)delta_h[i*nnh[i]+j]=-100.0;
	delta_o = dvector(no);
	
	//LECTURA DE DATOS DE ENTRENAMIENTO
	printf("\n----LECTURA DE DATOS DE ENTRENAMIENTO----\n");
	int k1, k2;
	FILE *file = fopen("train.txt", "r");
	if(file == NULL){
    	printf("Failed to open train.txt\n");
    	exit(EXIT_FAILURE);
	}
	
	for(t = 0; t < ntrain; t++){
		if(fscanf(file, "%d %d", &k1, &k2) != 2) {
		    printf("Failed to read input and target values from train.txt\n");
		    exit(EXIT_FAILURE);
		}
		for(i = 0; i < ni; i++){
		    if(fscanf(file, "%lf", &input[t*ni+i]) != 1) {
		        printf("Failed to read input values from train.txt\n");
		        exit(EXIT_FAILURE);
		    }
		}
		for(j = 0; j < no; j++){
		    if(fscanf(file, "%lf", &target[t*no+j]) != 1) {
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

	int trozo;
	if(percen == 0){
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
	//IMPRIMIR INPUT Y TARGET LEIDOS
	printf("INPUT:\n");
	for(i = 0; i < ntrain; i++) {
		for(j = 0; j < ni; j++) {
		    printf("%f ", input[i*ni + j]);
		}
		printf("\n");
	}
	printf("\nTARGET:\n");
	for(i = 0; i < ntrain; i++) {
		for(j = 0; j < no; j++) {
		    printf("%f ", target[i*no + j]);
		}
		printf("\n");
	}
	*/
	printf("\n----INICIALIZACION DE PESOS----\n");
	//Initialize weights between input layer and hidden layer
	//INICIALIZACION CON VALORES ENTRE [-0.5 , 0,5]
	srand(time(NULL));
	for(i = 0; i < nh; i++){
    	for(j = 0; j < nnh[i]; j++){
        	if(i == 0){
            	for(k = 0; k < ni; k++){
                	w_h[i*nnh[i]*ni + j*ni + k] = 2.0 * 1.0 / sqrt((double)ni) * (((double)rand()/(double)RAND_MAX) - 0.5);
            	}
        	} 
        	else{
            	for(k = 0; k < nnh[i - 1]; k++){
                	w_h[i*nnh[i-1]*ni + j*nnh[i-1] + k] = 2.0 * 1.0 / sqrt((double)nnh[i - 1]) * (((double)rand()/(double)RAND_MAX) - 0.5);
            	}
        	}
    	}
	}
	//Initialize weights between hidden layer and output layer
	for(i = 0; i < no; i++){
		for(j = 0; j < nnh[nh-1]; j++){
		    *(w_o + j*no + i) = 2.0*1.0/sqrt(nnh[nh-1])*(((double)rand()/(double)RAND_MAX) - 0.5);
		}
	}
	//Initialize biases
	for(i = 0; i < no; i++){
		b_o[i] = 0.0;
		db_o[i] = 0.0;
	}
	for(i = 0; i < nh; i++) {
		for(j = 0; j < nnh[i]; j++){
		    b_h[i*nnh[i] + j] = 0.0;
		    db_h[i*nnh[i] + j] = 0.0;
		}
	}
	printf("BIAS b_h INICIALIZADAS CAPA OCULTA\n");
	for(i = 0; i < nh; i++) {
   	for(j = 0; j < nnh[i]; j++){
        	printf("%f\n", b_h[i*nnh[i] + j]);
    	}
	}
	printf("DELTAS delta_h INICIALIZADAS CAPA OCULTA\n");
	for(i = 0; i < nh; i++) {
   	for(j = 0; j < nnh[i]; j++){
        	printf("%f\n", delta_h[i*nnh[i] + j]);
    	}
	}
	//Initialize error deltas
	for(i = 0; i < no; i++){
		delta_o[i] = 0.0;
	}
	for(i = 0; i < nh; i++){
		for(j = 0; j < nnh[i]; j++){
		    delta_h[i*nnh[i] + j] = 0.0;
		}
	}
	//IMPRESION DE PESOS INICIALIZADOS
	for(i = 0; i < nh; i++){
	    printf("Pesos de la capa oculta %d:\n", i + 1);
    	for(j = 0; j < nnh[i]; j++){
        	if(i == 0){
            	for(k = 0; k < ni; k++){
                	//printf("%lf ",w_h[i*nnh[i]*ni + j*ni + k]);
                	printf("w_h[%d]: %lf ", i*nnh[i]*ni + j*ni + k, w_h[i*nnh[i]*ni + j*ni + k]);
            	}
        	} 
        	else{
            	for(k = 0; k < nnh[i-1]; k++){
                	//printf("%lf ", ,w_h[i*nnh[i - 1]*nnh[i] + k*nnh[i] + j]);
						printf("w_h[%d]: %lf ", i*nnh[i-1]*ni + j*nnh[i-1] + k, w_h[i*nnh[i-1]*ni + j*nnh[i-1] + k]);
            	}
        	}
        	printf("\n");
    	}
    	printf("\n");
	}
	
	for(i = 0; i < no; i++){
		printf("Pesos de la capa oculta y la de salida\n");
		for(j = 0; j < nnh[nh-1]; j++){
		    printf("%lf ", *(w_o + j*no + i));
		}
		printf("\n");
	}
	
	max_min_weights(w_h, w_o, nnh, nh, ni, no);

	///////////////////////
/////////START TRAINING//////////////////////////////////////////
	///////////////////////
	srand(343343198);
	printf("rate = %lf\n", rate);
	printf("epochs = %d\n", epochs);
	//EPOCHS LOOP////////////////////////////////////////////////
	for(ep = 0; ep < epochs; ep++){
	
		//THE LEARNING SCHEDULE//
		if(ep == epochs/3) rate /= 2.0;
		if(ep == epochs/2) rate /= 2.0;
		if(ep == (int)(epochs/1.2)) rate /= 2.0;
		if(ep == (int)(epochs/1.4)) rate /= 2.0;
		//RANDOMIZE THE INPUTS USING FISHER-YATES
		for(i = 0; i < ntrain; i++) patron[i] = i;
		for(i = 0; i < ntrain; i++){
			ii = i + rand() % (ntrain - i);
			iii = patron[i];
			patron[i] = patron[ii];
			patron[ii] = iii;
		}
		// ITERATE OVER THE ENTIRE TRAINING SET
		// batchnumber = ntrain, trozo = 0, batchsize = 1
		// Para este caso el batchnumer es igual a ntrain, ya que todos los datos son de entrenamiento
		for(batch = 0; batch < batchnumber-trozo; batch++){ // TRAINING DATA IS SPLIT INTO TRAINING AND TESTING
    		for(dum = 0; dum < batchsize; dum++){ // EACH TIME WE DO THIS IS AN ITERATION
        		x = patron[dum+(batch)*batchsize]; // CASES LOOP
        		// GO THROUGH THE NETWORK FROM INPUT TO OUTPUT
        		// FIRST HIDDEN LAYER IS SPECIAL
        		for(k = 0; k < nh; k++){ // nh IS THE NUMBER OF HIDDEN LAYER
       			for(i = 0; i < nnh[k]; i++){ //nnh IS THE NUMBER OF NEURONS IN THE k HIDDEN LAYER
       				s = b_h[i + k*nnh[k]]; // THE BIAS
       				if(k == 0){ // THE FIRST HIDDEN LAYER IS SPECIAL
							for(j = 0; j < ni; j++){ // GO THROUGH ALL THE NEURONS IN THE INPUT LAYER
								s += *(input + j + x * ni) * *(w_h + j + i * ni + k * ni * nnh[k]); //SUM THE PRODUCT OF THE INPUT BY THE WEIGHT. THE BIAS HAS BEEN ADDED
							}
						} 
						else{ // i.e. IF IT IS NOT THE FIRST HIDDEN LAYER THEN
							for(j = 0; j < nnh[k-1]; j++){ // GO THROUGH ALL THE NEURONS IN THE PERVIOUS HIDDEN LAYER
								s += *(hidden + j + (k-1)*nnh[k-1]) * *(w_h + k*nnh[k]*nnh[k] + j*nnh[k] + i); // SUM THE PRODUCT OF THE PREVIOUS LAYER VALUE IN THE HIDDEN LAYER UNITS
							}
						}
						*(hidden + i + k *nnh[k]) = sigmoid(s); // APPLY THE ACTIVATION FUNCTION TO GET THE ACTUAL VALUE IN THE HIDDEN LAYER UNITS
        			}
        		}
        		// COMPUTE OUTPUT NEURON ACTIVATIONS FOR THE OUTPUT
        		for(k=0; k < no; k++){
					s = *(b_o + k); // THE BIAS
					for(j=0; j < nnh[nh-1]; j++){ // nh-1 is the LAST H LAYER, nnh(nh-1) IS THE no OF NEURONS in the last H LAYER
						s += *(hidden + (nh-1)*nnh[nh-1] + j) * *(w_o + j*no + k); // SUM THE PRODUCT OF THE VALUE IN THE LAST HIDDEN NEURONS BY THE WEIGHTS
					}
					*(output + k + x * no) = sigmoid(s); // APPLY THE ACTIVATION FUNCTION
				}
				//------------------------------------------------------------------ THE BACKPROP ALGORITHM
				// BACKPROP
				
				// WEIGHTS & BIAS CHANGES IN THE OUTPUT LAYER
				for(k=0; k < no; k++){
					*(delta_o + k) = *(output + k + x * no) - *(target + k + x * no);
					*(db_o + k) = *(delta_o + k);
					for(j=0; j < nnh[nh-1]; j++) 
						*(dw_o + j + k*nnh[nh-1]) = *(delta_o + k) * *(hidden + (nh-1)*nnh[nh-1] + j);
				}
				
				// WEIGHT & BIAS CHANGES IN THE HIDDEN LAYERS
				for(k = nh-1; k >= 0; k--){ // FOR ALL THE HIDDEN LAYERS BACKWARDS
					for(i = 0; i < nnh[k]; i++){ // FOR ALL THE NEURONS IN THOSE HIDDEN LAYERS
						// CALCULATE DELTAS IN ALL THE HIDDEN LAYERS BUT THE FIRST
						s = 0.0;
						if(k == nh-1){ // then THE LAST HIDDEN LAYERS
							for(j = 0; j < no; j++){ 
								s += w_o[i + j*no] * delta_o[j];
								//printf("Value of s LAST HIDDEN LAYER: %f\n", s);
								}
						}
						else{ // THE REST OF THE HIDDEN LAYERS
							for(j = 0; j < nnh[k+1]; j++){
								s += w_h[(k+1)*nnh[k+1]*nnh[k+1] + j*nnh[k+1] + i] * delta_h[(k+1)*nnh[k+1] + j];
								double wh_value = w_h[(k+1)*nnh[k+1]*nnh[k+1] + j*nnh[k+1] + i];
								double delta_value = delta_h[(k+1)*nnh[k+1] + j];
								//printf("w_h[%d]: %f\n", (k+1)*nnh[k+1]*nnh[k+1] + j*nnh[k+1] + i, wh_value);
								//printf("delta_h[%d]: %f\n", (k+1)*nnh[k+1] + j, delta_value);
								//printf("Value of s REST HIDDEN LAYERS: %f\n", s);
								}
						}
						delta_h[k*nnh[k] + i] = s * hidden[k*nnh[k] + i] * (1.0 - hidden[k*nnh[k] + i]);
						//printf("delta_h[%d]: %f\n", k*nnh[k] + i, delta_h[k*nnh[k] + i]);

						db_h[k*nnh[k] + i] = rate * delta_h[k*nnh[k] + i] + mo*db_h[k*nnh[k] + i];
						//printf("db_h[%d]: %f\n", k*nnh[k] + i, db_h[k*nnh[k] + i]);

						if(k == 0){ // THE FIRST HIDDEN LAYER
							for(j = 0; j < ni; j++)							
								dw_h[k*ni*nnh[k] + i*ni + j] = rate*input[x*ni + j]*delta_h[k*nnh[k] + i]*0.5-mo*dw_h[k*ni*nnh[k] + i*ni + j]; // THIS IS NOT STANDARD
						}
						else{
							for(j = 0; j < nnh[k-1]; j++){
								dw_h[k*nnh[k]*nnh[k] + i*nnh[k] + j] = rate*hidden[(k-1)*nnh[k-1] + j]*delta_h[k*nnh[k] + i]*0.5-mo*dw_h[k*ni*nnh[k] + i*ni + j]; // THIS IS NOT STANDARD
							}
						}
					}		
				}
				// -------------------------------------------------------STARTS OF UPDATING THE WEIGHTS AND BIASES
				// UPDATE THE WEIGHTS 6 BIASES
				// HIDDEN LAYERS
				for(k = 0; k < nh; k++){
					for(i = 0; i < nnh[k]; i++){
						b_h[k*nnh[k] + i] = b_h[k*nnh[k] + i] - db_h[k*nnh[k] + i];
						//printf("b_h[%d]: %f\n", k*nnh[k] + i, b_h[k*nnh[k] + i]);

						if(k==0){
							for(j = 0; j < ni; j++)
								w_h[j + i*ni + k*ni*nnh[k]];
						}
						else{
							for(j = 0; j < nnh[k]; j++)
								w_h[k*nnh[k]*nnh[k] + j*nnh[k] + i];
						}
					}
				}
				// OUTPUT LAYER
				for(k = 0;k<no;k++){
            	b_o[k] = b_o[k] - db_o[k];				
               	for (j=0;j<nnh[nh-1];j++) 
               		w_o[j + k*nnh[nh-1]] = w_o[j + k*nnh[nh-1]] - dw_o[j + k*nnh[nh-1]];  
            }			
			}	
   	}
   	// FIN DE LOS BUCLES DE ENTRENAMIENTO
   	/*
     !-----------------------------------------------------------------------------------------------------
     ! THE ERROR LOOPS ------------------------------------------------------------------------------------
     !-----------------------------------------------------------------------------------------------------
     ! CHECK MATCH WITH ALL THE DATA at EACH EPOCH - TRANSVERSE NET and CALCULATE sse1
     */
     /*
		sse1 = 0.0;
		for (xx = 0; xx < batchnumber; xx++) {
    		for (k = 0; k < nh; k++) {
        		for (i = 0; i < nnh[k]; i++) {
            	s = b_h[k*nnh[k] + i];
            	if (k == 0) {
               	for (j = 0; j < ni; j++)
                    s += input[xx*ni + j] * w_h[k*ni*nnh[k] + j*nnh[k] + i];
            	} 
            	else {
               	for (j = 0; j < nnh[k-1]; j++)
                    s += hidden[(k-1)*nnh[k-1] + j] * w_h[k*nnh[k-1]*nnh[k] + j*nnh[k] + i];
            	}
            	hidden[k*nnh[k] + i] = sigmoid(s);
        		}
    		}
    		for (k = 0; k < no; k++) {
        		s = b_o[k];
        		for (j = 0; j < nnh[nh-1]; j++)
            	s += hidden[(nh-1)*nnh[nh-1] + j] * w_o[j*no + k];
        		output[xx*no + k] = sigmoid(s);
        		sse1 += (output[xx*no + k] - target[xx*no + k]) * (output[xx*no + k] - target[xx*no + k]);
    		}
		}
		sse1 = sse1 / batchnumber;
		*/
		/*
     	!-----------------------------------------------------------------------------------------
     	! CHECK MATCH WITH JUST THE TESTING DATA at EACH EPOCH - TRANSVERSE NET and CALCULATE sse2
     	*/
     	/*
     	sse2 = 0.0;
		for (xx = batchnumber - trozo - 1; xx < batchnumber; xx++) {
			 for (k = 0; k < nh; k++) {
				  for (i = 0; i < nnh[k]; i++) {
				      s = b_h[k*nnh[k] + i];
				      if (k == 0) {
				          for (j = 0; j < ni; j++)
				              s += input[xx*ni + j] * w_h[k*ni*nnh[k] + j*nnh[k] + i];
				      } 
				      else {
				          for (j = 0; j < nnh[k-1]; j++)
				              s += hidden[(k-1)*nnh[k] + j] * w_h[k*nnh[k]*nnh[k] + j*nnh[k] + i];
				      }
				      hidden[k*nnh[k] + i] = sigmoid(s);
				  }
			 }
			 for (k = 0; k < no; k++) {
				  s = b_o[k];
				  for (j = 0; j < nnh[nh-1]; j++)
				      s += hidden[(nh-1)*nnh[nh-1] + j] * w_o[j*no + k];
				  output[xx*no + k] = sigmoid(s);
				  sse2 += (output[xx*no + k] - target[xx*no + k]) * (output[xx*no + k] - target[xx*no + k]);
			 }
		}
		sse2 = sse2 / trozo;
		*/
		/*
    	!-----------------------------------------------------------------------------------------
    	! ERROR COMPARISON -----------------------------------------------------------------------
    	! IF THE SSE1 IS BELOW THE THRESHOLD, STOP TRAINING. EXIT EPOCHS
    	*/
     	//if (sse1<erroradmisible) break;  //jjpm  Salgo del bucle. Cambiado el goto por este break goto 669
    	if (ep%10000==0)  printf("%8s %8i %11s %8.5f %12s %7.5f\n","Epochee: ",ep,"SSE(train):", sse1,"SSE(test):",sse2);
	}
	// END LOOP EPOCHs
	// ONCE FINISHED TRAINING, WRITE DOWN THE NN WEIGHTS FOR VALIDATION, ANALYSES AND REUSE
	
	FILE *fweights=fopen("./weights/weightsC.txt","w");
	//fprintf(fweights,"Pesos capas ocultas\n");
	for(i= 0; i<nh;i++){
	  	for(j= 0;j<nnh[i];j++){
		  	if(i==0){
		     	for(k= 0;k<ni;k++) fprintf(fweights, "%f\n", w_h[i*nnh[i]*ni + j*ni + k]);
		  	}
		  	else
		    	for(k= 0;k<nnh[i-1];k++) fprintf(fweights, "%f\n", w_h[i*nnh[i-1]*ni + j*nnh[i-1] + k]);			
	  	}
	}
	//fprintf(fweights,"BIAS capas ocultas\n");
	for(i = 0; i < nh; i++) {
   	for(j = 0; j < nnh[i]; j++){
        	fprintf(fweights, "%f\n", b_h[i*nnh[i] + j]);
    	}
	}
	//fprintf(fweights,"Pesos capas salida\n");
	for(i = 0; i < nnh[nh-1]; i++){
    	for (j = 0; j < no; j++){
        	fprintf(fweights, "%f\n", w_o[i*no + j]);
    	}
	}
	//fprintf(fweights,"BIAS capas salida\n");
	for(i = 0; i < no; i++){
    	fprintf(fweights, "%f\n", b_o[i]);
	}
	
	fclose(fweights);
	
	stop_time=clock();
	double time_taken = ((double)stop_time- start_time)/CLOCKS_PER_SEC; // in seconds
 	printf("train tooks %f seconds to execute \n", time_taken);
		
   return 0;
}
