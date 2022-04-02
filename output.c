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
double* func_choose(double* x1, double* x2, double* x3, double* x4) {
        if(x1[0]==0){
            return x2;
        }
        else if(x1[0] > 0){
            return x3;
        }
        else {
            return x4;
        }
}
double* func_tr(double* param, int dim1, int dim2) {
        int i, j;
        double* resArr = (double *) calloc(dim1*dim2, sizeof(double));
        for(i = 0; i < dim1; i++){
            for(j = 0; j < dim2; j++){
                resArr[i*dim2+j] = param[j*dim1+i];
            }
        }
        return resArr;
}
double* func_add(double* x1, double* x2, int d) {
    double* arr = (double *) calloc(d,sizeof (double ));
    for (int i = 0; i < d; i++) {
        arr[i] = x1[i] + x2[i];
    }
    return arr;
}
double* func_subtract(double* x1, double* x2, int d) {
    double* arr = (double *) calloc(d,sizeof (double ));
    for (int i = 0; i < d; i++) {
        arr[i] = x1[i] - x2[i];
    }
    return arr;
}
double* func_multiply(double* x1, double* x2, int d, int d2, int d3) {
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
int main() {
double * A  = (double *) calloc(16, sizeof(double));
double * count  = (double *) calloc(1, sizeof(double));
double * incr  = (double *) calloc(1, sizeof(double));
double * i  = (double *) calloc(1, sizeof(double));
double * j  = (double *) calloc(1, sizeof(double));
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
count[0]  = 0.000000;
for(i[0] = 1.000000; i[0] < 4.000000; i[0] += 1.000000) {
for(j[0] = 1.000000; j[0] < 4.000000; j[0] += 1.000000) {
incr  = func_choose(func_subtract(A[0], 4.000000, 1),1.000000,1.000000,0.000000);
count  = func_add(count, incr, 1);
}
}
printf("count[0]");
return 0; 
}
