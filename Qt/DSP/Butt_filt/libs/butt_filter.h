#ifndef BUTT_FILTER_H
#define BUTT_FILTER_H

#include <stdlib.h>
#include <math.h>

class TButtFilter {
	private:
		enum FilterType {
			ftNone,
			ftLPF,
			ftHPF,
			ftBPF,
			ftBSF
		};

		int FOrder;
		double FSampleRate;
		double FCutoffFreq;

		FilterType FFilterType;
		//double a; // = tan(M_PI*f/s);
		//double a2; // = a*a;
		//double r;

		double * A;	// = (double *)malloc(n*sizeof(double));
		double * d1; // = (double *)malloc(n*sizeof(double));
		double * d2; // = (double *)malloc(n*sizeof(double));
		double * w0; // = (double *)calloc(n, sizeof(double));
		double * w1; // = (double *)calloc(n, sizeof(double));
		double * w2; // = (double *)calloc(n, sizeof(double));

		void AllocMem(int Size);
		void FreeMem();
		void ProcessLPF(double *Buffer, int NumSamples);
		void ProcessHPF(double *Buffer, int NumSamples);
	public:
		TButtFilter();
		~TButtFilter();
		bool InitLowPass(int Order, double SampleRate, double CutoffFreq);
		bool InitHighPass(int Order, double SampleRate, double CutoffFreq);
		void InitBandPass();
		void InitBandStop();

		void Process(double *Buffer, int NumSamples);

};


#endif // BUTT_FILTER_H
