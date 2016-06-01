#include <iostream>
#include <stdlib.h>
#include <stdio.h>
//#include <vector>
#include <math.h>

#include <QVector>
#include "wave_gen.h"

using namespace std;

#define N 10 //The number of images which construct a time series for each pixel
//#define PI 3.14159


class FilterButterworth {
	public:
		enum PassType {
			Highpass,
			Lowpass,
		};

	/// <summary>
	/// rez amount, from sqrt(2) to ~ 0.1
	/// </summary>
	private:
		double _resonance;

		double _frequency;
		int _sampleRate;
		PassType _passType;
		double c, a1, a2, a3, b1, b2;

		/// <summary>
		/// Array of input values, latest are in front
		/// </summary>
		double inputHistory[2]; // = new float[2];

		/// <summary>
		/// Array of output values, latest are in front
		/// </summary>
		double outputHistory[3]; // = new float[3];

	public:
		FilterButterworth(double frequency, int sampleRate, PassType passType, double resonance) {
			_resonance = resonance;
			_frequency = frequency;
			_sampleRate = sampleRate;
			_passType = passType;

			switch (_passType) {
				case Lowpass:
					c = 1.0f / (double)tan(M_PI * frequency / sampleRate);
					a1 = 1.0f / (1.0f + resonance * c + c * c);
					a2 = 2.0f * a1;
					a3 = a1;
					b1 = 2.0f * (1.0f - c * c) * a1;
					b2 = (1.0f - resonance * c + c * c) * a1;
					break;
				case Highpass:
					c = (double)tan(M_PI * frequency / sampleRate);
					a1 = 1.0f / (1.0f + resonance * c + c * c);
					a2 = -2.0f * a1;
					a3 = a1;
					b1 = 2.0f * (c * c - 1.0f) * a1;
					b2 = (1.0f - resonance * c + c * c) * a1;
					break;
			};
		}

		double Update(double newInput) {
			double newOutput = a1 * newInput + a2 * inputHistory[0] + a3 * inputHistory[1] - b1 * outputHistory[0] - b2 * outputHistory[1];

			inputHistory[1] = inputHistory[0];
			inputHistory[0] = newInput;

			outputHistory[2] = outputHistory[1];
			outputHistory[1] = outputHistory[0];
			outputHistory[0] = newOutput;
			return newOutput;
		}

		double Value() {
			return outputHistory[0];
		}

		void Process(double * Buffer, int NumSamples) {
			int i;
			double * BufferPtr = Buffer;

			for (i = 0; i < NumSamples; i++) {
				*BufferPtr = Update(*BufferPtr);
				BufferPtr++;
			}
		}

		void Process(QVector<double> &Buffer, int NumSamples) {
			int i;

			for (i = 0; i < NumSamples; i++) {
				Buffer[i] = Update(Buffer[i]);
			};
		}
};

double *ComputeLP( int FilterOrder ) {
	double *NumCoeffs;
	int m;
	int i;

	NumCoeffs = (double *)calloc( FilterOrder+1, sizeof(double) );
	if( NumCoeffs == NULL ) return( NULL );

	NumCoeffs[0] = 1;
	NumCoeffs[1] = FilterOrder;
	m = FilterOrder/2;
	for( i=2; i <= m; ++i) 	{
		NumCoeffs[i] =(double) (FilterOrder-i+1)*NumCoeffs[i-1]/i;
		NumCoeffs[FilterOrder-i]= NumCoeffs[i];
	}
	NumCoeffs[FilterOrder-1] = FilterOrder;
	NumCoeffs[FilterOrder] = 1;

	return NumCoeffs;
}

double *ComputeHP( int FilterOrder ) {
	double *NumCoeffs;
	int i;

	NumCoeffs = ComputeLP(FilterOrder);
	if(NumCoeffs == NULL ) return( NULL );

	for( i = 0; i <= FilterOrder; ++i)
		if( i % 2 ) NumCoeffs[i] = -NumCoeffs[i];

	return NumCoeffs;
}

double *TrinomialMultiply( int FilterOrder, double *b, double *c ) {
	int i, j;
	double *RetVal;

	RetVal = (double *)calloc( 4 * FilterOrder, sizeof(double) );
	if( RetVal == NULL ) return( NULL );

	RetVal[2] = c[0];
	RetVal[3] = c[1];
	RetVal[0] = b[0];
	RetVal[1] = b[1];

	for( i = 1; i < FilterOrder; ++i ) 	{
		RetVal[2*(2*i+1)]   += c[2*i] * RetVal[2*(2*i-1)]   - c[2*i+1] * RetVal[2*(2*i-1)+1];
		RetVal[2*(2*i+1)+1] += c[2*i] * RetVal[2*(2*i-1)+1] + c[2*i+1] * RetVal[2*(2*i-1)];

		for( j = 2*i; j > 1; --j ) {
			RetVal[2*j]   += b[2*i] * RetVal[2*(j-1)]   - b[2*i+1] * RetVal[2*(j-1)+1] +
				c[2*i] * RetVal[2*(j-2)]   - c[2*i+1] * RetVal[2*(j-2)+1];
			RetVal[2*j+1] += b[2*i] * RetVal[2*(j-1)+1] + b[2*i+1] * RetVal[2*(j-1)] +
				c[2*i] * RetVal[2*(j-2)+1] + c[2*i+1] * RetVal[2*(j-2)];
		}

		RetVal[2] += b[2*i] * RetVal[0] - b[2*i+1] * RetVal[1] + c[2*i];
		RetVal[3] += b[2*i] * RetVal[1] + b[2*i+1] * RetVal[0] + c[2*i+1];
		RetVal[0] += b[2*i];
		RetVal[1] += b[2*i+1];
	}

	return RetVal;
}

double *ComputeNumCoeffs(int FilterOrder) {
	double *TCoeffs;
	double *NumCoeffs;
	int i;

	NumCoeffs = (double *)calloc( 2*FilterOrder+1, sizeof(double) );
	if( NumCoeffs == NULL ) return( NULL );

	TCoeffs = ComputeHP(FilterOrder);
	if( TCoeffs == NULL ) return( NULL );

	for( i = 0; i < FilterOrder; ++i) {
		NumCoeffs[2*i] = TCoeffs[i];
		NumCoeffs[2*i+1] = 0.0;
	}
	NumCoeffs[2*FilterOrder] = TCoeffs[FilterOrder];

	free(TCoeffs);

	return NumCoeffs;
}

double *ComputeDenCoeffs( int FilterOrder, double Lcutoff, double Ucutoff ) {
	int k;            // loop variables
	double theta;     // PI * (Ucutoff - Lcutoff) / 2.0
	double cp;        // cosine of phi
	double st;        // sine of theta
	double ct;        // cosine of theta
	double s2t;       // sine of 2*theta
	double c2t;       // cosine 0f 2*theta
	double *RCoeffs;     // z^-2 coefficients
	double *TCoeffs;     // z^-1 coefficients
	double *DenomCoeffs;     // dk coefficients
	double PoleAngle;      // pole angle
	double SinPoleAngle;     // sine of pole angle
	double CosPoleAngle;     // cosine of pole angle
	double a;         // workspace variables

	cp = cos(M_PI * (Ucutoff + Lcutoff) / 2.0);
	theta = M_PI * (Ucutoff - Lcutoff) / 2.0;
	st = sin(theta);
	ct = cos(theta);
	s2t = 2.0*st*ct;        // sine of 2*theta
	c2t = 2.0*ct*ct - 1.0;  // cosine of 2*theta

	RCoeffs = (double *)calloc( 2 * FilterOrder, sizeof(double) );
	TCoeffs = (double *)calloc( 2 * FilterOrder, sizeof(double) );

	for( k = 0; k < FilterOrder; ++k ) {
		PoleAngle = M_PI * (double)(2*k+1)/(double)(2*FilterOrder);
		SinPoleAngle = sin(PoleAngle);
		CosPoleAngle = cos(PoleAngle);
		a = 1.0 + s2t*SinPoleAngle;
		RCoeffs[2*k] = c2t/a;
		RCoeffs[2*k+1] = s2t*CosPoleAngle/a;
		TCoeffs[2*k] = -2.0*cp*(ct+st*SinPoleAngle)/a;
		TCoeffs[2*k+1] = -2.0*cp*st*CosPoleAngle/a;
	}

	DenomCoeffs = TrinomialMultiply(FilterOrder, TCoeffs, RCoeffs );
	free(TCoeffs);
	free(RCoeffs);

	DenomCoeffs[1] = DenomCoeffs[0];
	DenomCoeffs[0] = 1.0;
	for( k = 3; k <= 2*FilterOrder; ++k )
		DenomCoeffs[k] = DenomCoeffs[2*k-2];


	return DenomCoeffs;
}

void filter(int ord, double *a, double *b, int np, double *x, double *y) {
	int i, j;
	y[0] = b[0] * x[0];
	for (i = 1; i < ord + 1; i++) {
		y[i] = 0.0;
		for (j = 0; j < i + 1; j++)
			y[i] = y[i] + b[j]*x[i-j];
		for (j = 0; j < i; j++)
			y[i]=y[i] - a[j + 1]*y[i - j - 1];
	};
	for (i = ord + 1; i < np + 1; i++) {
		y[i] = 0.0;
		for (j = 0; j < ord + 1; j++)
			y[i] = y[i] + b[j]*x[i - j];
		for (j = 0; j < ord; j++)
			y[i] = y[i] - a[j + 1]*y[i - j - 1];
	};
}




int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
//	//Frequency bands is a vector of values - Lower Frequency Band and Higher Frequency Band

//	//First value is lower cutoff and second value is higher cutoff
//	double FrequencyBands[2] = {0.25, 0.375};//these values are as a ratio of f/fs, where fs is sampling rate, and f is cutoff frequency
//	//and therefore should lie in the range [0 1]
//	//Filter Order

//	int FiltOrd = 5;

//	//Pixel Time Series
//	/*int PixelTimeSeries[N];
//	int outputSeries[N];
//	*/
//	//Create the variables for the numerator and denominator coefficients
//	double *DenC = 0;
//	double *NumC = 0;
//	//Pass Numerator Coefficients and Denominator Coefficients arrays into function, will return the same

//	NumC = ComputeNumCoeffs(FiltOrd);
//	for(int k = 0; k<11; k++) {
//		printf("NumC is: %lf\n", NumC[k]);
//	}
//	//is A in matlab function and the numbers are correct
//	DenC = ComputeDenCoeffs(FiltOrd, FrequencyBands[0], FrequencyBands[1]);
//	for(int k = 0; k<11; k++) {
//		printf("DenC is: %lf\n", DenC[k]);
//	}
//	double y[5];
//	double x[5]={1,2,3,4,5};
//	filter(5, DenC, NumC, 5, x, y);
//	return 1;

	TWaveGenerator * WG = new TWaveGenerator;
	WG->Init(44100, 500, 0);

	FilterButterworth * filter = new FilterButterworth(400, 44100, FilterButterworth::Lowpass, 20);

	QVector<double> Signal(1000);
	//float * Signal = (float *)malloc(200*sizeof(float));

	WG->GetWave(Signal, 1000, 5);

	filter->Process(Signal, 1000);

	printf("End\n");

	return 0;

}
