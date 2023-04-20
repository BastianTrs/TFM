#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include "memoryfun.h"


double *dvector(int nh)

/* Dimensiona y reserve espacio para un vector double de rango [n1..nh] */
{
        double *v;

        v=(double *) malloc((nh)*sizeof(double));
        return v;
}

int *ivector(int nh)

/* Dimensiona y reserve espacio para un vector double de rango [n1..nh] */
{
        int *v;

        v=(int *) malloc((nh)*sizeof(int));
        return v;
}

double *dmatrix (int nfh, int nch) 

/* Dimensiona y reserva espacio para una matriz double [nf1..nfh][nc1..nch] */
{
	double *m;
	
	m=(double *)malloc((nfh)*(nch)*sizeof(double));
	return m;
}

int *imatrix (int nfh, int nch) 

/* Dimensiona y reserva espacio para una matriz int [nf1..nfh][nc1..nch] */
{
	int *m;
	
	m=(int *)malloc((nfh)*(nch)*sizeof(int));
	return m;
}
