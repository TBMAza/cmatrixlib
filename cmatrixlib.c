#include "cmatrixlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

Matrix* matinit(size_t rows, size_t cols, float* cell, float initval) {
	Matrix* ret = malloc(sizeof(Matrix));
	ret->rows = rows;
	ret->cols = cols;
	if(cell) {
		ret->owns_cell = false;
		ret->cell = cell;
	}
	else {
		ret->owns_cell = true;
		ret->cell = malloc(sizeof(float) * rows * cols);
		for(size_t i = 0; i < rows*cols; ++i) ret->cell[i] = initval;
	}
	return ret;
}
void matprint(Matrix* m) {
	for(size_t i = 0; i < m->rows; ++i) {
		for(size_t j = 0; j < m->cols; ++j) {
			printf("%f\t", m->cell[i*m->cols + j]);
		}
		printf("\n");
	}
}
void matfree(Matrix* m) {
	if(m->owns_cell) free(m->cell);
	free(m);
}

/************************************************* SINGLE-THREADED API *************************************************/

Matrix* mattranspose(Matrix* m) {
	Matrix* ret = malloc(sizeof(Matrix));
	ret->rows = m->cols;
	ret->cols = m->rows;
	ret->owns_cell = true;
	ret->cell = malloc(sizeof(float) * ret->rows * ret->cols);
	for(size_t i = 0; i < m->rows; ++i) {
		for(size_t j = 0; j < m->cols; ++j) {
			ret->cell[j*ret->cols + i] = m->cell[i*m->cols + j];
		}
	}
	return ret;
}
Matrix* matsum(Matrix* m1, Matrix* m2) {
	if(m1->rows != m2->rows || m1->cols != m2->cols) return NULL;
	
	Matrix* ret = malloc(sizeof(Matrix));
	ret->rows = m1->rows;
	ret->cols = m1->cols;
	ret->owns_cell = true;
	ret->cell = malloc(sizeof(float) * ret->rows * ret->cols);
	
	for(size_t i = 0; i < ret->rows * ret->cols; ++i) ret->cell[i] = m1->cell[i] + m2->cell[i];
	
	return ret; 
}
Matrix* matscalarmult(float x, Matrix* m) {
	Matrix* ret = malloc(sizeof(Matrix));
	ret->rows = m->rows;
	ret->cols = m->cols;
	ret->owns_cell = true;
	ret->cell = malloc(sizeof(float) * ret->rows * ret->cols);
	
	for(size_t i = 0; i < ret->rows * ret->cols; ++i) ret->cell[i] = m->cell[i] * x;
	
	return ret;
}
Matrix* matmult(Matrix* m1, Matrix* m2) {
	if(m1->cols != m2->rows) return NULL;
	
	Matrix* ret = malloc(sizeof(Matrix));
	ret->rows = m1->rows;
	ret->cols = m2->cols;
	ret->owns_cell = true;
	ret->cell = calloc(ret->rows * ret->cols, sizeof(float));

	size_t m1_row_tracker = 0, m2_col_tracker = 0;
	for(size_t i = 0; i < ret->rows * ret->cols; ++i) {
		for(size_t j = 0; j < m1->cols; ++j) {
			ret->cell[i] += m1->cell[m1_row_tracker*m1->cols + j] * m2->cell[j*m2->cols + m2_col_tracker];
		}
		m2_col_tracker = (m2_col_tracker+1) % m2->cols;
		if(!m2_col_tracker) ++m1_row_tracker;
	}

	return ret;
}

/************************************************* MULTITHREADED API *************************************************/

typedef struct {
	Matrix* m1;
	Matrix* m2;
	Matrix* ret;

	size_t start_slice;
	size_t end_slice;
} SWArgs;
static void* matsum_worker(void* args) {
	SWArgs* uargs = (SWArgs*)args;
	Matrix* m1 = uargs->m1;
	Matrix* m2 = uargs->m2;
	Matrix* ret = uargs->ret;
	size_t start_slice = uargs->start_slice;
	size_t end_slice = uargs->end_slice;

	for(size_t i = start_slice; i < end_slice; ++i) {
		ret->cell[i] = m1->cell[i] + m2->cell[i];
	}

	return NULL;
}
Matrix* matsum_m(Matrix* m1, Matrix* m2) {
	if(m1->rows != m2->rows || m1->cols != m2->cols) return NULL;
	
	Matrix* ret = malloc(sizeof(Matrix));
	ret->rows = m1->rows;
	ret->cols = m1->cols;
	ret->owns_cell = true;
	ret->cell = malloc(sizeof(float) * ret->rows * ret->cols);

	size_t elements = ret->rows * ret->cols;
	size_t threads_no = elements / MINELEMSXTHREAD;
	if(threads_no <= 1) return matsum(m1, m2);
	if(threads_no > MAXTHREADS) threads_no = MAXTHREADS;

	size_t elements_per_thread = elements / threads_no;
	size_t remaining_elements = elements % threads_no;

	pthread_t threadpool[threads_no];
	SWArgs argvec[threads_no];

	size_t current_pointer = 0;
	for(size_t i = 0; i < threads_no; ++i) {
		argvec[i].m1 = m1;
		argvec[i].m2 = m2;
		argvec[i].ret = ret;

		argvec[i].start_slice = current_pointer;
		argvec[i].end_slice = current_pointer + elements_per_thread + (i < remaining_elements ? 1 : 0);
		current_pointer = argvec[i].end_slice;

		pthread_create(threadpool+i, NULL, matsum_worker, argvec+i);
	}
	for(size_t i = 0; i < threads_no; ++i) pthread_join(threadpool[i], NULL);

	return ret;
}

typedef struct {
	float x;
	Matrix* m;
	Matrix* ret;

	size_t start_slice;
	size_t end_slice;
} SMWArgs;
static void* matscalarmult_worker(void* args) {
	SMWArgs* uargs = (SMWArgs*)args;
	float x = uargs->x;
	Matrix* m = uargs->m;
	Matrix* ret = uargs->ret;
	size_t start_slice = uargs->start_slice;
	size_t end_slice = uargs->end_slice;

	for(size_t i = start_slice; i < end_slice; ++i) {
		ret->cell[i] = m->cell[i] * x;
	}

	return NULL;
}
Matrix* matscalarmult_m(float x, Matrix* m) {
	Matrix* ret = malloc(sizeof(Matrix));
	ret->rows = m->rows;
	ret->cols = m->cols;
	ret->owns_cell = true;
	ret->cell = malloc(sizeof(float) * ret->rows * ret->cols);

	size_t elements = ret->rows * ret->cols;
	size_t threads_no = elements / MINELEMSXTHREAD;
	if(threads_no <= 1) return matscalarmult(x, m);
	if(threads_no > MAXTHREADS) threads_no = MAXTHREADS;

	size_t elements_per_thread = elements / threads_no;
	size_t remaining_elements = elements % threads_no;

	pthread_t threadpool[threads_no];
	SMWArgs argvec[threads_no];

	size_t current_pointer = 0;
	for(size_t i = 0; i < threads_no; ++i) {
		argvec[i].x = x;
		argvec[i].m = m;
		argvec[i].ret = ret;

		argvec[i].start_slice = current_pointer;
		argvec[i].end_slice = current_pointer + elements_per_thread + (i < remaining_elements ? 1 : 0);
		current_pointer = argvec[i].end_slice;

		pthread_create(threadpool+i, NULL, matscalarmult_worker, argvec+i);
	}
	for(size_t i = 0; i < threads_no; ++i) pthread_join(threadpool[i], NULL);

	return ret;
}

typedef struct {
	Matrix* m1;
	Matrix* m2;
	Matrix* ret;

	size_t start_row;
	size_t end_row;
} MWArgs;
static void* matmult_worker(void* args) {
	MWArgs* uargs = (MWArgs*)args;
	Matrix* m1 = uargs->m1;
	Matrix* m2 = uargs->m2;
	Matrix* ret = uargs->ret;
	size_t start_row = uargs->start_row;
	size_t end_row = uargs->end_row;

	for(size_t i = start_row; i < end_row; ++i) {
		for(size_t j = 0; j < ret->cols; ++j) {
			for(size_t k = 0; k < m1->cols; ++k) {
				ret->cell[i*ret->cols + j] += m1->cell[i*m1->cols + k] * m2->cell[k*m2->cols + j];
			}
		}
	}

	return NULL;
}
static void* matmult_worker_tr(void* args) {
	MWArgs* uargs = (MWArgs*)args;
	Matrix* m1 = uargs->m1;
	Matrix* m2 = uargs->m2;
	Matrix* ret = uargs->ret;
	size_t start_row = uargs->start_row;
	size_t end_row = uargs->end_row;

	for(size_t i = start_row; i < end_row; ++i) {
		for(size_t j = 0; j < ret->cols; ++j) {
			for(size_t k = 0; k < m1->cols; ++k) {
				ret->cell[i*ret->cols + j] += m1->cell[i*m1->cols + k] * m2->cell[j*m2->cols + k];
			}
		}
	}

	return NULL;
}
Matrix* matmult_m(Matrix* m1, Matrix* m2) {
	if(m1->cols != m2->rows) return NULL;

	Matrix* ret = malloc(sizeof(Matrix));
	ret->rows = m1->rows;
	ret->cols = m2->cols;
	ret->owns_cell = true;
	ret->cell = calloc(ret->rows * ret->cols, sizeof(float));

	size_t threads_no = ret->rows < MAXTHREADS ? ret->rows : MAXTHREADS;
	size_t rows_per_thread = ret->rows / threads_no;
	size_t remaining_rows = ret->rows % threads_no;

	pthread_t threadpool[threads_no];
	MWArgs argvec[threads_no];

	size_t current_row = 0;
	for(size_t i = 0; i < threads_no; ++i) {
		argvec[i].m1 = m1;
		argvec[i].m2 = m2;
		argvec[i].ret = ret;

		argvec[i].start_row = current_row;
		argvec[i].end_row = current_row + rows_per_thread + (i < remaining_rows ? 1 : 0);
		current_row = argvec[i].end_row;

		pthread_create(threadpool+i, NULL, matmult_worker, argvec+i);
	}
	for(size_t i = 0; i < threads_no; ++i) pthread_join(threadpool[i], NULL);

	return ret;
}
Matrix* matmult_m_tr(Matrix* m1, Matrix* m2) {
	if(m1->cols != m2->rows) return NULL;

	Matrix* ret = malloc(sizeof(Matrix));
	ret->rows = m1->rows;
	ret->cols = m2->cols;
	ret->owns_cell = true;
	ret->cell = calloc(ret->rows * ret->cols, sizeof(float));

	size_t threads_no = ret->rows < MAXTHREADS ? ret->rows : MAXTHREADS;
	size_t rows_per_thread = ret->rows / threads_no;
	size_t remaining_rows = ret->rows % threads_no;

	pthread_t threadpool[threads_no];
	MWArgs argvec[threads_no];

	Matrix* m2t = mattranspose(m2);

	size_t current_row = 0;
	for(size_t i = 0; i < threads_no; ++i) {
		argvec[i].m1 = m1;
		argvec[i].m2 = m2t;
		argvec[i].ret = ret;

		argvec[i].start_row = current_row;
		argvec[i].end_row = current_row + rows_per_thread + (i < remaining_rows ? 1 : 0);
		current_row = argvec[i].end_row;

		pthread_create(threadpool+i, NULL, matmult_worker_tr, argvec+i);
	}
	for(size_t i = 0; i < threads_no; ++i) pthread_join(threadpool[i], NULL);

	return ret;
}