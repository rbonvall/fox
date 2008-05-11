#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdio.h>
#include <stdlib.h>

typedef float matrix_type;

/* crear una nueva matriz de m x n */
matrix_type **matrix_new(int m, int n);

/* liberar la memoria de una matrix */
void matrix_free(matrix_type **A);

/* sumar las matrices A y B y poner el resultado en C, todas de m x n */
void matrix_add(matrix_type **A, matrix_type **B, matrix_type **C,
                     int m, int n);

/* multiplicar la matriz A, de m x n, por la matriz B, de n x p,
 * y suma el resultado a la matriz C de m x p;  para obtener el
 * producto AB, usar una matriz C inicializada con ceros */
void matrix_multiply_and_add(matrix_type **A, matrix_type **B, matrix_type **C,
                     int m, int n, int p);

/* imprimir la matriz por pantalla */
void matrix_print(matrix_type **A, int m, int n);

/* asignar a la i-esima fila de la matriz A, de n columnas, los
 * valores en el arreglo row. */
void matrix_set_row(matrix_type **A, int i, int n, matrix_type row[]); 


/* crear una nueva matriz de m x n, y leer sus elementos
 * del archivo de texto de nombre filename;  si no se puede
 * leer el archivo, retornar NULL. */
matrix_type **matrix_new_from_file(int m, int n, char filename[]);

#endif
