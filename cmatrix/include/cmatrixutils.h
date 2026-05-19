#ifndef CMATRIXUTILS_H
#define CMATRIXUTILS_H

#include "cmatrixlib.h"

typedef struct {
	mtype lower;
	mtype upper;
} RUArgs;

typedef struct {
	mtype mean;
	mtype variance;
} RGArgs;

mtype randunif(void* args);
mtype randgauss(void* args);

#endif
