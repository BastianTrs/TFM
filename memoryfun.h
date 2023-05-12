double *dvector(int nh);
int *ivector(int nh);
double *dmatrix(int nfh, int nch);
int *imatrix(int nfh, int nch);
double maxval(double* arr, int size);
double minval(double* arr, int size);
void max_min_weights(double* w_h, double* w_o, int* nnh, int nh, int ni, int no);
double sigmoid(double x);
double dsigmoid(double x);
