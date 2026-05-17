#ifndef CMATRIXLIB_H
#define CMATRIXLIB_H

#include <stddef.h>
#include <stdbool.h>

#define MAXTHREADS 8
#define MINELEMSXTHREAD 262144
#define IGNORE 0.0

typedef struct {
	size_t rows;
	size_t cols;
	float* cell;
	bool owns_cell;
} Matrix;


Matrix* matinit(size_t rows, size_t cols, float* cell, float initval);
void matprint(Matrix* m);
void matfree(Matrix* m);

/* SINGLE-THREADED API */
Matrix* mattranspose(Matrix* m);
Matrix* matsum(Matrix* m1, Matrix* m2);
Matrix* matscalarmult(float x, Matrix* m);
Matrix* matmult(Matrix* m1, Matrix* m2);

/* MULTITHREADED API */
Matrix* matsum_m(Matrix* m1, Matrix* m2);
Matrix* matscalarmult_m(float x, Matrix* m);
Matrix* matmult_m(Matrix* m1, Matrix* m2);
Matrix* matmult_m_tr(Matrix* m1, Matrix* m2); // transposes m2 and operates row by row on it instead of col by col; better for cpu cache, thus faster

#endif
