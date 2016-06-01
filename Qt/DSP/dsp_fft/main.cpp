#include <QCoreApplication>

#include <iostream>
#include <iomanip>
#include <complex>
#include <cmath>


#include "fft.h"
#include "cfft.h"

#define C(x,y) std::complex<double>((x),(y))

//using namespace std::complex_literals;

int main(int argc, char *argv[]) {
	Q_UNUSED(argc);
	Q_UNUSED(argv);
	//QCoreApplication a(argc, argv);
	int i;

	cfft * c_fft;

	c_fft = new cfft(128);

	PDCplx FSignal, FSignalPtr;
	PDCplx FSpectr, FSpectrPtr;

	FSignal = (PDCplx)malloc(128*sizeof(TDCplx));
	FSignalPtr = FSignal;
	FSpectr = (PDCplx)malloc(128*sizeof(TDCplx));
	FSpectrPtr = FSpectr;

	std::vector<std::complex<double> > A;
	std::vector<std::complex<double> > C;
	C.resize(128);
	std::complex<double> B;
	int m = 128;

	std::complex<double> X;
	std::complex<double> Y;
	X = C(2,4);
	Y = C(10, 20);
	X *= Y;

	for (i = 0; i < m; i++) {
		B = C(i, i + 1);
		std::cout << B << '\n';
		A.push_back(B);

		FSignalPtr->Re = i;
		FSignalPtr->Im = i + 1;
		FSignalPtr++;
	};
	std::cout << '\n';

//	fft(A, 128);
//	for (i = 0; i < m; i++)
//		std::cout << A[i] << '\n';

//	fft_nip(A, C, 128);
//	for (i = 0; i < m; i++)
//		std::cout << C[i] << '\n';

	fft_nip_1(FSignal, FSpectr, 128);
	for (i = 0; i < m; i++) {
		std::cout << "(" << FSpectrPtr->Re << "," << FSpectrPtr->Im << ")\n";
		FSpectrPtr++;
	};

//	c_fft->fft(&A);
//	for (i = 0; i < m; i++) {
//		std::cout << "(" << FSignal->Re << "," << FSignal->Im << ")\n";
//		FSpectrPtr++;
//	};

	std::cout << '\n';

//	ifft(A, 128);
//	for (i = 0; i < m; i++)
//		std::cout << A[i] << '\n';

//	ifft_nip(C, A, 128);
//	for (i = 0; i < m; i++)
//		std::cout << A[i] << '\n';

	ifft_nip_1(FSpectr, FSignal, 128);
	FSignalPtr = FSignal;
	for (i = 0; i < m; i++) {
		std::cout << "(" << FSignalPtr->Re << "," << FSignalPtr->Im << ")\n";
		FSignalPtr++;
	};

	return 0;

	std::cout << std::fixed << std::setprecision(1);

	std::complex<double> z1 = 1i * 1i;     // imaginary unit squared
	std::cout << "i * i = " << z1 << '\n';

	//std::complex<double> z2 = std::pow(1i, 2); // imaginary unit squared
	//std::cout << "pow(i, 2) = " << z2 << '\n';

	//double PI = std::acos(-1);
	//std::complex<double> z3 = std::exp(1i * PI); // Euler's formula
	//std::cout << "exp(i * pi) = " << z3 << '\n';

	std::complex<double> z4 = 1. + 2i, z5 = 1. - 2i; // conjugates
	std::cout << "(1+2i)*(1-2i) = " << z4*z5 << '\n';

	return 0;
}
