/* The following program computes the product of two matrices
 * using the Level-3 BLAS function DGEMM. The matrices are
 * stored in row major order, according to the C convention
 * for arrays.
 */

#include <stdio.h>
#include <gsl/gsl_blas.h>

int main(void)
{
    double a[] = { 0.11, 0.12, 0.13,
                   0.21, 0.22, 0.23 }; // 2x3

    double b[] = { 1011, 1012,
                   1021, 1022,
                   1031, 1032 }; // 3x2

    double c[] = { 0.00, 0.00,
                   0.00, 0.00 }; // 2x2

    gsl_matrix_view A = gsl_matrix_view_array(a, 2, 3);
    gsl_matrix_view B = gsl_matrix_view_array(b, 3, 2);
    gsl_matrix_view C = gsl_matrix_view_array(c, 2, 2);

    /* Compute C = AB */

    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans,
                   1.0, &A.matrix, &B.matrix,
                   0.0, &C.matrix);

    printf("[[ %g, %g ]\n",  c[0], c[1]);
    printf(" [ %g, %g ]]\n", c[2], c[3]);

    return 0;
}
