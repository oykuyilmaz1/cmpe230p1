#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "t.h"
#include <ctype.h>
double* func_sqrt(double* param, int dim) {
        double* res = (double*) calloc(dim, sizeof(double));
        for(int j = 0; j < dim; j++){
            res[j] = sqrt(param[j]);
        }
        return res;
}
double func_choose(double x1, double x2, double x3, double x4) {
        if(x1==0){
            return x2;
        }
        else if(x1 > 0){
            return x3;
        }
        else {
            return x4;
        }
}
double* func_tr_matrix(double* param, int dim1, int dim2) {
        int i, j;
        double* resArr = (double *) calloc(dim1*dim2, sizeof(double));
        for(i = 0; i < dim1; i++){
            for(j = 0; j < dim2; j++){
                resArr[i*dim2+j] = param[j*dim1+i];
            }
        }
        return resArr;
}
double func_tr_scalar(double param, int dim1, int dim2) {
        return param;}
double* func_add_matrix(double* x1, double* x2, int d) {
    double* arr = (double *) calloc(d,sizeof (double ));
    for (int i = 0; i < d; i++) {
        arr[i] = x1[i] + x2[i];
    }
    return arr;
}
double func_add_scalar(double x1, double x2, int d) {
    return x1 + x2;
}
double* func_subtract_matrix(double* x1, double* x2, int d) {
    double* arr = (double *) calloc(d,sizeof (double ));
    for (int i = 0; i < d; i++) {
        arr[i] = x1[i] - x2[i];
    }
    return arr;
}
double func_subtract_scalar(double x1, double x2, int d) {
    return x1 - x2;
}
double* func_multiply_matrix(double* x1, double* x2, int d, int d2, int d3) {
    int i,j,k;
    double* arr = (double *) calloc(d,sizeof (double ));
    for(i = 0; i <d; i++){
          for(j = 0; j < d2; j++){
               for(k = 0; k < d3; k++){
                    arr[i*d2+j] += x1[i*d3+k] * x2[k*d2+j];
               }
          }
     }    return arr;
}
double func_multiply_scalar(double x1, double x2, int d, int d2, int d3) {
    return x1*x2;
}
int main() {
double * A  = (double *) calloc(16, sizeof(double));
double count  = 0;
double incr  = 0;
double i  = 0;
double j  = 0;
A[0]  = 0.000000;
A[1]  = 1.000000;
A[2]  = 2.000000;
A[3]  = 3.000000;
A[4]  = 4.000000;
A[5]  = 5.000000;
A[6]  = 6.000000;
A[7]  = 7.000000;
A[8]  = 8.000000;
A[9]  = 9.000000;
A[10]  = 1.000000;
A[11]  = 1.000000;
A[12]  = 1.000000;
A[13]  = 2.000000;
A[14]  = 3.000000;
A[15]  = 4.000000;
count  = 0.000000;
for(i = 1.000000; i <= 4.000000; i += 1.000000) {
for(j = 1.000000; j <= 4.000000; j += 1.000000) {
incr  = func_choose(func_subtract_scalar(A[(int)((i - 1) * 4 + j - 1)], 4.000000, 1),1.000000,1.000000,0.000000);
count  = func_add_scalar(count, incr, 1);
}
}
printf("%.0f", count);
return 0; 
}
