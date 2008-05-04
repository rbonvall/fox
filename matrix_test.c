#include <stdio.h>
#include "matrix.h"

int main() {
    matrix_type **A, **B, **C;
    int correct;

    A = matrix_new(2, 3);
    B = matrix_new(3, 4);
    C = matrix_new(2, 4);

    A[0][0] =  3;  A[0][1] = -1;  A[0][2] = -2;
    A[1][0] = -4;  A[1][1] =  0;  A[1][2] =  1;

    B[0][0] = -7;  B[0][1] =  1;  B[0][2] =  4;  B[0][3] = -2;
    B[1][0] =  3;  B[1][1] =  0;  B[1][2] =  1;  B[1][3] =  4;
    B[2][0] =  1;  B[2][1] = -3;  B[2][2] =  2;  B[2][3] =  2;

    matrix_multiply(A, B, C, 2, 3, 4);

    correct = C[0][0] == -26 && C[0][1] ==   9 && C[0][2] ==   7 && C[0][3] == -14 &&
              C[1][0] ==  29 && C[1][1] ==  -7 && C[1][2] == -14 && C[1][3] ==  10;

    printf("A =\n");
    matrix_print(A, 2, 3);
    printf("B =\n");
    matrix_print(B, 3, 4);
    printf("C =\n");
    matrix_print(C, 2, 4);

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

