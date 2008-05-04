#include "matrix.h"


matrix_type **matrix_new(int m, int n) {
    int i;
    matrix_type **rows;

    rows    = calloc(m,     sizeof(*rows));
    rows[0] = calloc(m * n, sizeof(*rows[0]));
    for (i = 1; i < m; ++i)
        rows[i] = rows[0] + n * i;
    return rows;
}


void matrix_free(matrix_type **A) {
    free(*A);
    free(A);
}


void matrix_multiply(matrix_type **A, matrix_type **B, matrix_type **C, int m, int n, int p) {
    int i, j, k;
    for (i = 0; i < m; ++i)
        for (j = 0; j < p; ++j) {
            C[i][j] = 0;
            for (k = 0; k < n; ++k)
                C[i][j] += A[i][k] * B[k][j];
        }
}


void matrix_print(matrix_type **A, int m, int n) {
    int i, j;
    for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j)
            printf("\t%g", A[i][j]);
        printf("\n");
    }

}

