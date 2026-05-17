#include "cmatrixlib.h"
#include <stdio.h>
#include <time.h>

#define INITCLOCK struct timespec start, end; double elapsed
#define STARTCLOCK clock_gettime(CLOCK_MONOTONIC, &start)
#define STOPCLOCK clock_gettime(CLOCK_MONOTONIC, &end)
#define ELAPSED ( (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9 )

int main(void) {
	INITCLOCK;

	/*float m1cell[] = {
		0, 1,
		2, 3,
		4, 5,
		6, 7,
		8, 9
	};
	float m2cell[] = {
		9, 8, 7, 6,
		5, 4, 3, 2
	};

	Matrix* m1 = matinit(5, 2, m1cell, 0);
	Matrix* m2 = matinit(2, 4, m2cell, 0);

	matprint(matmult(m1, m2));
	printf("\n");

	Matrix* m3 = matinit(2, 4, NULL, 1);

	matprint(matsum(m2, m3));
	
	matfree(m1);
	matfree(m2);
	matfree(m3);

	Matrix* m1 = matinit(1000, 1000, NULL, 1);
	Matrix* m2 = matinit(1000, 1000, NULL, 1);
	Matrix* m3 = NULL;

	STARTCLOCK;
	m3 = matmult_m(m1, m2);
	STOPCLOCK;
	printf("elapsed: %fs\n", ELAPSED);

	matfree(m3);

	STARTCLOCK;
	m3 = matmult(m1, m2);
	STOPCLOCK;
	elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;
	printf("elapsed: %fs\n", ELAPSED);

	matfree(m1);
	matfree(m2);
	matfree(m3);

	Matrix* m = matinit(89999, 89999, NULL, 0);
	printf("initialzed\n");
	STARTCLOCK;
	Matrix* m2 = mattranspose(m);
	STOPCLOCK;
	printf("elapsed: %fs\n", ELAPSED);

	matfree(m);
	matfree(m2);

	Matrix* m1 = matinit(4000, 4000, NULL, 1);
	Matrix* m2 = matinit(4000, 4000, NULL, 1);
	printf("inited\n");
	Matrix* m2t = mattranspose(m2);
	printf("transposed\n");
	Matrix* hold = NULL;

	STARTCLOCK;
	hold = matmult_m_tr(m1, m2t);
	STOPCLOCK;
	printf("elapsed: %fs\n", ELAPSED);

	matfree(hold);
	
	STARTCLOCK;
	hold = matmult_m(m1, m2);
	STOPCLOCK;
	printf("elapsed: %fs\n", ELAPSED);

	float mcell1[] = {
		0, 1,
		2, 3,
		4, 5,
		6, 7,
		8, 9
	};
	float mcell2[] = {
		9, 8, 7, 6,
		5, 4, 3, 2
	};
	Matrix* m1 = matinit(5, 2, mcell1, IGNORE);
	Matrix* m2 = matinit(2, 4, mcell2, IGNORE);

	matprint(matmult_m_tr(m1, m2));

	float mcell[] = {
		1, 2,
		3, 4,
		5, 6
	};
	Matrix* m = matinit(3, 2, mcell, IGNORE);
	matprint(mattranspose(m));*/

	printf("initializing matrices...\n");
	Matrix* m1 = matinit(59000, 59000, NULL, 1);
	Matrix* m2 = matinit(59000, 59000, NULL, 2);
	Matrix* hold = NULL;
	printf("initialization done\n");

	printf("multithreaded method started...\n");
	STARTCLOCK;
	hold = matsum_m(m1, m2);
	STOPCLOCK;
	printf("multithreaded method done; elapsed: %fs\n", ELAPSED);

	matfree(hold);

	printf("single-threaded method started...\n");
	STARTCLOCK;
	hold = matsum(m1, m2);
	STOPCLOCK;
	printf("single-threaded method done; elapsed: %fs\n", ELAPSED);

	matfree(m1);
	matfree(m2);
	matfree(hold);

	return 0;
}
