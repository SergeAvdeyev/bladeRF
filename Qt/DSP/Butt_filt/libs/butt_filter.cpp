
#include "butt_filter.h"


TButtFilter::TButtFilter() {
	FFilterType = ftNone;

	A = NULL;
	d1 = NULL;
	d2 = NULL;
	w0 = NULL;
	w1 = NULL;
	w2 = NULL;
}

TButtFilter::~TButtFilter() {
	FreeMem();
}

void TButtFilter::AllocMem(int Size) {
	if (A != NULL) return;

	A  = (double *)malloc(Size*sizeof(double));
	d1 = (double *)malloc(Size*sizeof(double));
	d2 = (double *)malloc(Size*sizeof(double));
	w0 = (double *)calloc(Size, sizeof(double));
	w1 = (double *)calloc(Size, sizeof(double));
	w2 = (double *)calloc(Size, sizeof(double));
}

void TButtFilter::FreeMem() {
	if (A == NULL) return;
	free(A);
	A = NULL;
	free(d1);
	free(d2);
	free(w0);
	free(w1);
	free(w2);
}

bool TButtFilter::InitLowPass(int Order, double SampleRate, double CutoffFreq) {
	if ((FFilterType == ftLPF) &&
		(FOrder*2 == Order) &&
		(FSampleRate == SampleRate) &&
		(FCutoffFreq == CutoffFreq)) return true; // already initialized

	if ((Order < 2) || (Order > 20) || (Order % 2 != 0)) return false; // Bad param
	if ((SampleRate < 100) || (SampleRate > 50000000)) return false;
	if (CutoffFreq > (SampleRate / 2)) return false;

	/* Now initialize */
	FSampleRate = SampleRate;
	FOrder = Order / 2;
	FCutoffFreq = CutoffFreq;

	FFilterType = ftLPF;
	double a = tan(M_PI*FCutoffFreq/FSampleRate);
	double a2 = a*a;
	double r;
	int i;
	double s;

	FreeMem(); /* Just in case */
	AllocMem(FOrder);

	/* Calc coeffs */
	for (i = 0; i < FOrder; ++i) {
		r = sin(M_PI*(2.0*i + 1.0)/(4.0*FOrder));
		s = a2 + 2.0*a*r + 1.0;
		A[i] = a2/s;
		d1[i] = 2.0*(1 - a2)/s;
		d2[i] = -(a2 - 2.0*a*r + 1.0)/s;
	};


	return true;
}

bool TButtFilter::InitHighPass(int Order, double SampleRate, double CutoffFreq) {
	if ((FFilterType == ftHPF) &&
		(FOrder*2 == Order) &&
		(FSampleRate == SampleRate) &&
		(FCutoffFreq == CutoffFreq)) return true; // already initialized

	if ((Order < 2) || (Order > 20) || (Order % 2 != 0)) return false; // Bad param
	if ((SampleRate < 100) || (SampleRate > 50000000)) return false;
	if (CutoffFreq > (SampleRate / 2)) return false;

	/* Now initialize */
	FFilterType = ftHPF;
	FSampleRate = SampleRate;
	FOrder = Order / 2;
	FCutoffFreq = CutoffFreq;

	double a = tan(M_PI*FCutoffFreq/FSampleRate);
	double a2 = a*a;
	double r;
	int i;
	double s;

	FreeMem(); /* Just in case */
	AllocMem(FOrder);

	for (i = 0; i < FOrder; ++i) {
		r = sin(M_PI*(2.0*i + 1.0)/(4.0*FOrder));
		s = a2 + 2.0*a*r + 1.0;
		A[i] = 1.0/s;
		d1[i] = 2.0*(1 - a2)/s;
		d2[i] = -(a2 - 2.0*a*r + 1.0)/s;
	};


	return true;
}

void TButtFilter::InitBandPass() {

}

void TButtFilter::InitBandStop() {

}

void TButtFilter::Process(double * Buffer, int NumSamples) {
	if (FFilterType == ftLPF)
		ProcessLPF(Buffer, NumSamples);
	else if (FFilterType == ftHPF)
		ProcessHPF(Buffer, NumSamples);
}

void TButtFilter::ProcessLPF(double * Buffer, int NumSamples) {
	int i, j;
	double * BufferPtr = Buffer;

	for (j = 0; j < NumSamples; j++) {
		for (i = 0; i < FOrder; ++i) {
			w0[i] = d1[i]*w1[i] + d2[i]*w2[i] + *BufferPtr;
			*BufferPtr = A[i]*(w0[i] + 2.0*w1[i] + w2[i]);
			w2[i] = w1[i];
			w1[i] = w0[i];
		};
		BufferPtr++;
	};
}

void TButtFilter::ProcessHPF(double * Buffer, int NumSamples) {
	int i, j;
	double * BufferPtr = Buffer;

	for (j = 0; j < NumSamples; j++) {
		for (i = 0; i < FOrder; ++i) {
			w0[i] = d1[i]*w1[i] + d2[i]*w2[i] + *BufferPtr;
			*BufferPtr = A[i]*(w0[i] - 2.0*w1[i] + w2[i]);
			w2[i] = w1[i];
			w1[i] = w0[i];
		};
		BufferPtr++;
	};
}





/* Band-pass Filter */
// The order of the filter must be a multiple of 4.

//int main(int argc, char *argv[]) {
//	if (argc < 5) {
//		printf("Usage: %s n s f1 f2\n", argv[0]);
//		printf("Butterworth bandpass filter.\n");
//		printf("  n = filter order 4,8,12,...\n");
//		printf("  s = sampling frequency\n");
//		printf("  f1 = upper half power frequency\n");
//		printf("  f2 = lower half power frequency\n");
//		return(-1);
//	};

//	int i, n = (int)strtol(argv[1], NULL, 10);
//	if (n % 4){
//		printf("Order must be 4,8,12,16,...\n");
//		return(-1);
//	};

//	double s = strtod(argv[2], NULL);
//	double f1 = strtod(argv[3], NULL);
//	double f2 = strtod(argv[4], NULL);
//	double a = cos(M_PI*(f1 + f2)/s)/cos(M_PI*(f1 - f2)/s);
//	double a2 = a*a;
//	double b = tan(M_PI*(f1 - f2)/s);
//	double b2 = b*b;
//	double r;

//	n = n/4;
//	double *A = (double *)malloc(n*sizeof(double));
//	double *d1 = (double *)malloc(n*sizeof(double));
//	double *d2 = (double *)malloc(n*sizeof(double));
//	double *d3 = (double *)malloc(n*sizeof(double));
//	double *d4 = (double *)malloc(n*sizeof(double));
//	double *w0 = (double *)calloc(n, sizeof(double));
//	double *w1 = (double *)calloc(n, sizeof(double));
//	double *w2 = (double *)calloc(n, sizeof(double));
//	double *w3 = (double *)calloc(n, sizeof(double));
//	double *w4 = (double *)calloc(n, sizeof(double));
//	double x;

//	for(i=0; i<n; ++i){
//		r = sin(M_PI*(2.0*i + 1.0)/(4.0*n));
//		s = b2 + 2.0*b*r + 1.0;
//		A[i] = b2/s;
//		d1[i] = 4.0*a*(1.0 + b*r)/s;
//		d2[i] = 2.0*(b2 - 2.0*a2-1.0)/s;
//		d3[i] = 4.0*a*(1.0 - b*r)/s;
//		d4[i] = -(b2 - 2.0*b*r + 1.0)/s;
//	};

//	while(scanf("%lf", &x)!=EOF) {
//		for(i=0; i<n; ++i){
//			w0[i] = d1[i]*w1[i] + d2[i]*w2[i] + d3[i]*w3[i] + d4[i]*w4[i] + x;
//			x = A[i]*(w0[i] - 2.0*w2[i] + w4[i]);
//			w4[i] = w3[i];
//			w3[i] = w2[i];
//			w2[i] = w1[i];
//			w1[i] = w0[i];
//		};
//		printf("%lf\n", x);
//	};

//	return(0);
//}
/* Band-pass Filter */


