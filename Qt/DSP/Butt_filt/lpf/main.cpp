
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "wave_gen.h"
#include "butt_filter.h"

#define SAMPLE_RATE 4800000
#define SIGNAL_FREQ 50000
#define FILTER_FREQ 46000
#define NUM_SAMPLES 2000

// Compile: gcc -lm -o bwlpf bwlpf.c

int main(int argc, char *argv[] ) {
//	if (argc < 4) {
//		printf("Usage: %s n s f\n", argv[0]);
//		printf("Butterworth lowpass filter.\n");
//		printf("  n = filter order 2,4,6,...\n");
//		printf("  s = sampling frequency\n");
//		printf("  f = half power frequency\n");
//		return(-1);
//	};

	TWaveGenerator * WG = new TWaveGenerator;
	WG->Init(SAMPLE_RATE, SIGNAL_FREQ, 0);

	TWaveGenerator_Par * WG_PAR = new TWaveGenerator_Par;
	WG_PAR->Init(SAMPLE_RATE, SIGNAL_FREQ, 0);

	QVector<double> Signal(NUM_SAMPLES);

	WG->GetWave(Signal, NUM_SAMPLES, 200);
	//WG_PAR->GetWave(Signal, NUM_SAMPLES, 5);

	TButtFilter * Filter;
	Filter = new TButtFilter;
	double * Buffer = (double *)malloc(NUM_SAMPLES*sizeof(double));
	for (int k = 0; k < NUM_SAMPLES; k++)
		Buffer[k] = Signal[k];

	Filter->InitHighPass(4, SAMPLE_RATE, FILTER_FREQ);
	Filter->Process(Buffer, NUM_SAMPLES);
	for (int k = 0; k < NUM_SAMPLES; k++)
		Signal[k] = Buffer[k];

	/* Default values */
	int n = 4;
	double s = SAMPLE_RATE;
	double f = FILTER_FREQ;
	if (argc == 4)
		f = strtod(argv[3], NULL);
	if (argc >= 3)
		s = strtod(argv[2], NULL);
	if (argc >= 2)
		n = (int)strtol(argv[1], NULL, 10);
	n = n/2;

	int i;

	/*  Low-pass Filter coeffs */
	double a = tan(M_PI*f/s);
	double a2 = a*a;
	double r;

	double *A = (double *)malloc(n*sizeof(double));
	double *d1 = (double *)malloc(n*sizeof(double));
	double *d2 = (double *)malloc(n*sizeof(double));
	double *w0 = (double *)calloc(n, sizeof(double));
	double *w1 = (double *)calloc(n, sizeof(double));
	double *w2 = (double *)calloc(n, sizeof(double));

//	for (i = 0; i < n; ++i) {
//		r = sin(M_PI*(2.0*i + 1.0)/(4.0*n));
//		s = a2 + 2.0*a*r + 1.0;
//		A[i] = a2/s;
//		d1[i] = 2.0*(1 - a2)/s;
//		d2[i] = -(a2 - 2.0*a*r + 1.0)/s;
//	};
	/*  Low-pass Filter coeffs */



	/* High-pass filter coeffs */
	for (i = 0; i < n; ++i) {
		r = sin(M_PI*(2.0*i + 1.0)/(4.0*n));
		s = a2 + 2.0*a*r + 1.0;
		A[i] = 1.0/s;
		d1[i] = 2.0*(1 - a2)/s;
		d2[i] = -(a2 - 2.0*a*r + 1.0)/s;
	};
	/* High-pass filter coeffs */


	/* LPF */
	int j;
//	for (j = 0; j < NUM_SAMPLES; j++) {
//		for (i = 0; i < n; ++i) {
//			w0[i] = d1[i]*w1[i] + d2[i]*w2[i] + Signal[j];
//			Signal[j] = A[i]*(w0[i] + 2.0*w1[i] + w2[i]);
//			w2[i] = w1[i];
//			w1[i] = w0[i];
//		};
//		printf("%lf\n", Signal[j]);
//	};
	/* LPF */

	/* HPF */
	for (j = 0; j < NUM_SAMPLES; j++) {
		for (i = 0; i < n; ++i) {
			w0[i] = d1[i]*w1[i] + d2[i]*w2[i] + Signal[j];
			Signal[j] = A[i]*(w0[i] - 2.0*w1[i] + w2[i]);
			w2[i] = w1[i];
			w1[i] = w0[i];
		};
		printf("%lf\n", Signal[j]);
	};
	/* HPF */


	return(0);
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
