#include "../include/cmatrixutils.h"
#include <math.h>
#include <stdlib.h>

mtype randunif(void* args) {
	RUArgs* uargs = (RUArgs*)args;
	return uargs->lower + ((mtype)rand() / (mtype)RAND_MAX) * (uargs->upper - uargs->lower);
}
mtype randgauss(void* args) {
	RGArgs* uargs = (RGArgs*)args;

	mtype u1 = (mtype)rand() / (mtype)RAND_MAX;
	mtype u2 = (mtype)rand() / (mtype)RAND_MAX;

	mtype z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);

	return z0 * uargs->variance + uargs->mean;
}
