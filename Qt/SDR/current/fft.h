#ifndef FFT_H
#define FFT_H

#include <stdlib.h>
#include <complex>
#include <vector>
#include <chrono>

#define TWOPI 2*M_PI
#define HALFPI 0.5*M_PI
#define QUARTPI 0.25*M_PI

#define Cplx(x,y) std::complex<double>((x),(y))

struct TWCplx {
	int16_t Re;
	int16_t Im;
};
typedef TWCplx * PWCplx;

struct TDCplx {
	double Re;
	double Im;
};
typedef TDCplx * PDCplx;

struct TFCplx {
	float Re;
	float Im;
};
typedef TFCplx * PFCplx;


void fft(std::vector<std::complex<double> >& Signal, int WindowSize);
void ifft(std::vector<std::complex<double> >& Spectr, int WindowSize);

void fft_nip(std::vector<std::complex<double> > Signal, std::vector<std::complex<double> >& Spectr, int WindowSize);
void ifft_nip(std::vector<std::complex<double> > Spectr, std::vector<std::complex<double> >& Signal, int WindowSize);

void fft_nip_1(PWCplx Signal, PDCplx Spectr, int WindowSize);
void fft_nip_2(PFCplx Signal, PDCplx Spectr, int WindowSize);

void ifft_nip_1(PDCplx Spectr, PWCplx Signal, int WindowSize);


inline std::complex<double> expj(double x) { return (std::complex<double>(std::cos(x), std::sin(x))); }



class TDCBlocker {
	public:
		TDCBlocker();
		~TDCBlocker();

		void Process(int16_t &in_out);

	private:
		long acc, A, prev_x, prev_y;
		double pole;

		float gain;
		float lastinput;
		float lastoutput;
};


#endif // FFT_H
