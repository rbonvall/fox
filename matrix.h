#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdlib.h>

typedef float matrix_type;

/* crear una nueva matriz de m x n */
matrix_type **matrix_new(int m, int n);

/* liberar la memoria de una matrix */
void matrix_free(matrix_type **A);

/* multiplicar la matriz A, de m x n, por la matriz B, de n x p,
 * y poner el resultado en C de m x p */
void matrix_multiply(matrix_type **A, matrix_type **B, matrix_type **C,
                     int m, int n, int p);

/* imprimir la matriz por pantalla */
void matrix_print(matrix_type **A, int m, int n);

#endif
