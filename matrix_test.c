#include <stdio.h>
#include "matrix.h"

int main() {
    matrix_type **A, **B, **C;
    int correct;

    A = matrix_new(2, 3);
    B = matrix_new(3, 4);
    C = matrix_new(2, 4);

    matrix_set_row(A, 0, 3, (matrix_type[]) { 3, -1, -2});
    matrix_set_row(A, 1, 3, (matrix_type[]) {-4,  0,  1});

    matrix_set_row(B, 0, 4, (matrix_type[]) {-7,  1,  4, -2});
    matrix_set_row(B, 1, 4, (matrix_type[]) { 3,  0,  1,  4});
    matrix_set_row(B, 2, 4, (matrix_type[]) { 1, -3,  2,  2});

    /* C esta inicializada en cero */

    printf("A =\n");
    matrix_print(A, 2, 3);
    printf("B =\n");
    matrix_print(B, 3, 4);
    printf("C before =\n");
    matrix_print(C, 2, 4);

    matrix_multiply_and_add(A, B, C, 2, 3, 4);

    printf("C after =\n");
    matrix_print(C, 2, 4);

    correct = C[0][0] == -26 && C[0][1] ==   9 && C[0][2] ==   7 && C[0][3] == -14 &&
              C[1][0] ==  29 && C[1][1] ==  -7 && C[1][2] == -14 && C[1][3] ==  10;

    matrix_free(A);
    matrix_free(B);
    matrix_free(C);

    if (correct) {
        printf("Test passed\n");
        return 0;
    } else {
        printf("Test failed\n");
        return -1;
    }
}

