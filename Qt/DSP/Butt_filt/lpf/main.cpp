
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "wave_gen.h"

#define SAMPLE_RATE 4800000
#define SIGNAL_FREQ 50000
#define FILTER_FREQ 16000
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

	WG->GetWave(Signal, NUM_SAMPLES, 5);

	WG_PAR->GetWave(Signal, NUM_SAMPLES, 5);

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

	double a = tan(M_PI*f/s);
	double a2 = a*a;
	double r;
	double *A = (double *)malloc(n*sizeof(double));
	double *d1 = (double *)malloc(n*sizeof(double));
	double *d2 = (double *)malloc(n*sizeof(double));
	double *w0 = (double *)calloc(n, sizeof(double));
	double *w1 = (double *)calloc(n, sizeof(double));
	double *w2 = (double *)calloc(n, sizeof(double));
	//double x;

	for (i = 0; i < n; ++i) {
		r = sin(M_PI*(2.0*i + 1.0)/(4.0*n));
		s = a2 + 2.0*a*r + 1.0;
		A[i] = a2/s;
		d1[i] = 2.0*(1 - a2)/s;
		d2[i] = -(a2 - 2.0*a*r + 1.0)/s;
	};

//	while (scanf("%lf", &x) != EOF) {
//		for (i = 0; i < n; ++i) {
//			w0[i] = d1[i]*w1[i] + d2[i]*w2[i] + x;
//			x = A[i]*(w0[i] + 2.0*w1[i] + w2[i]);
//			w2[i] = w1[i];
//			w1[i] = w0[i];}
//			printf("%lf\n", x);
//	};

	int j;
	for (j = 0; j < NUM_SAMPLES; j++) {
		for (i = 0; i < n; ++i) {
			w0[i] = d1[i]*w1[i] + d2[i]*w2[i] + Signal[j];
			Signal[j] = A[i]*(w0[i] + 2.0*w1[i] + w2[i]);
			w2[i] = w1[i];
			w1[i] = w0[i];
		};
		printf("%lf\n", Signal[j]);
	};

	return(0);
}
