
#include "fft.h"

/* *************************************************************************
fft - In-place radix 2 decimation in time inverse FFT

Requires pointer to complex array, x and power of 2 size of FFT, m
(size of FFT = 2**m).  Places inverse FFT output on top of input
frequency domain COMPLEX array.

complex<float_type> fft(complex<float_type> *x, int m)

*************************************************************************/
void fft(std::vector<std::complex<double> >& Signal, int WindowSize) {
	std::complex<double> u, temp, tm;
	int wptr;
	int i, j, k, l, le, windex;

	/*  n = 2**m = inverse fft length */
	//int n = 1 << WindowSize;
	int fft_size_tmp = 1;
	i = WindowSize;

	while (1) {
		i = i >> 1;
		if (i == 1) break;
		fft_size_tmp++;
	};

	le = WindowSize / 2;
	std::vector<std::complex<double> > w(WindowSize + 1);
	std::vector<std::complex<double> > x(WindowSize + 1);

	/*  calculate the w values recursively */
	std::complex<double> w_inc = expj(-TWOPI / WindowSize);
	std::complex<double> w_x = w_inc;

	for (j = 0; j < le; j++) {
		w[j] = w_x;
		w_x *= w_inc;
		x[j] = Signal[j];
		x[j + le] = Signal[j + le];
	};

	/*  start inverse fft */
	le = WindowSize;
	windex = 1;
	for (l = 0; l < fft_size_tmp; l++) {
		le = le / 2;
		/*  first iteration with no multiplies */
		for (i = 0; i < WindowSize; i += 2 * le) {
			temp = (x[i] + x[i + le]);
			x[i + le] = (x[i] - x[i + le]);
			x[i] = temp;
		};

		/*  remaining iterations use stored w */
		wptr = windex - 1;
		for (j = 1; j < le; j++) {
			for (i = j; i < WindowSize; i += 2 * le) {
				temp = x[i] + x[i + le];
				tm = x[i] - x[i + le];
				x[i + le] = tm * w[wptr];
				x[i] = temp;
			};
			wptr += windex;
		};
		windex = 2 * windex;
	};

	/*  rearrange data by bit reversing */
	j = 0;
	for (i = 1; i < (WindowSize - 1); i++) {
		k = WindowSize / 2;
		while (k <= j) {
			j = j - k;
			k = k / 2;
		};
		j = j + k;
		if (i < j) {
			temp = x[j];
			x[j] = x[i];
			x[i] = temp;
		};
	};

	for (i = 0; i < WindowSize; i++)
		Signal[i] = x[i];
}





void fft_nip(std::vector<std::complex<double> > Signal, std::vector<std::complex<double> >& Spectr, int WindowSize) {
	std::complex<double> u, temp, tm;
	int wptr;
	int i, j, k, l, le, windex;

	/*  n = 2**m = inverse fft length */
	//int n = 1 << WindowSize;
	int fft_size_tmp = 1;
	i = WindowSize;

	while (1) {
		i = i >> 1;
		if (i == 1) break;
		fft_size_tmp++;
	};

	le = WindowSize / 2;
	std::vector<std::complex<double> > w(WindowSize + 1);
	std::vector<std::complex<double> > x(WindowSize + 1);

	/*  calculate the w values recursively */
	std::complex<double> w_inc = expj(-TWOPI / WindowSize);
	std::complex<double> w_x = w_inc;

	for (j = 0; j < le; j++) {
		w[j] = w_x;
		w_x *= w_inc;
		x[j] = Signal[j];
		x[j + le] = Signal[j + le];
	};

	/*  start inverse fft */
	le = WindowSize;
	windex = 1;
	for (l = 0; l < fft_size_tmp; l++) {
		le = le / 2;
		/*  first iteration with no multiplies */
		for (i = 0; i < WindowSize; i += 2 * le) {
			temp = (x[i] + x[i + le]);
			x[i + le] = (x[i] - x[i + le]);
			x[i] = temp;
		};

		/*  remaining iterations use stored w */
		wptr = windex - 1;
		for (j = 1; j < le; j++) {
			for (i = j; i < WindowSize; i += 2 * le) {
				temp = x[i] + x[i + le];
				tm = x[i] - x[i + le];
				x[i + le] = tm * w[wptr];
				x[i] = temp;
			};
			wptr += windex;
		};
		windex = 2 * windex;
	};

	/*  rearrange data by bit reversing */
	j = 0;
	for (i = 1; i < (WindowSize - 1); i++) {
		k = WindowSize / 2;
		while (k <= j) {
			j = j - k;
			k = k / 2;
		};
		j = j + k;
		if (i < j) {
			temp = x[j];
			x[j] = x[i];
			x[i] = temp;
		};
	};

	for (i = 0; i < WindowSize; i++)
		Spectr[i] = x[i];
}


void fft_nip_1(PWCplx Signal, PDCplx Spectr, int WindowSize) {
	std::complex<double> u, temp, tm;
	int wptr;
	int i, j, k, l, le, windex;

	//PDCplx SignalPtr = Signal;
	//PDCplx SpectrPtr = Spectr;

	/*  n = 2**m = inverse fft length */
	//int n = 1 << WindowSize;
	int fft_size_tmp = 1;
	i = WindowSize;

	while (1) {
		i = i >> 1;
		if (i == 1) break;
		fft_size_tmp++;
	};

	le = WindowSize / 2;
	std::vector<std::complex<double> > w(WindowSize + 1);
	std::vector<std::complex<double> > x(WindowSize + 1);

	/*  calculate the w values recursively */
	std::complex<double> w_inc = expj(-TWOPI / WindowSize);
	std::complex<double> w_x = w_inc;

	for (j = 0; j < le; j++) {
		w[j] = w_x;
		w_x *= w_inc;
		x[j] = Cplx(Signal[j].Re, Signal[j].Im);
		x[j + le] = Cplx(Signal[j + le].Re, Signal[j + le].Im);

		//x[j] = Cplx(SignalPtr->Re, SignalPtr->Im);
		//x[j + le] = Cplx(PDCplx(SignalPtr + le)->Re, PDCplx(SignalPtr + le)->Im);
		//SignalPtr++;
	};

	/*  start inverse fft */
	le = WindowSize;
	windex = 1;
	for (l = 0; l < fft_size_tmp; l++) {
		le = le / 2;
		/*  first iteration with no multiplies */
		for (i = 0; i < WindowSize; i += 2 * le) {
			temp = (x[i] + x[i + le]);
			x[i + le] = (x[i] - x[i + le]);
			x[i] = temp;
		};

		/*  remaining iterations use stored w */
		wptr = windex - 1;
		for (j = 1; j < le; j++) {
			for (i = j; i < WindowSize; i += 2 * le) {
				temp = x[i] + x[i + le];
				tm = x[i] - x[i + le];
				x[i + le] = tm * w[wptr];
				x[i] = temp;
			};
			wptr += windex;
		};
		windex = 2 * windex;
	};

	/*  rearrange data by bit reversing */
	j = 0;
	for (i = 1; i < (WindowSize - 1); i++) {
		k = WindowSize / 2;
		while (k <= j) {
			j = j - k;
			k = k / 2;
		};
		j = j + k;
		if (i < j) {
			temp = x[j];
			x[j] = x[i];
			x[i] = temp;
		};
	};

	for (i = 0; i < WindowSize; i++) {
		u = x[i];
		Spectr[i].Re = u.real();
		Spectr[i].Im = u.imag();
		//SpectrPtr++;
	};
}

void fft_nip_2(PFCplx Signal, PDCplx Spectr, int WindowSize) {
	std::complex<double> u, temp, tm;
	int wptr;
	int i, j, k, l, le, windex;

	/*  n = 2**m = inverse fft length */
	//int n = 1 << WindowSize;
	int fft_size_tmp = 1;
	i = WindowSize;

	while (1) {
		i = i >> 1;
		if (i == 1) break;
		fft_size_tmp++;
	};

	le = WindowSize / 2;
	std::vector<std::complex<double> > w(WindowSize + 1);
	std::vector<std::complex<double> > x(WindowSize + 1);

	/*  calculate the w values recursively */
	std::complex<double> w_inc = expj(-TWOPI / WindowSize);
	std::complex<double> w_x = w_inc;

	for (j = 0; j < le; j++) {
		w[j] = w_x;
		w_x *= w_inc;
		x[j] = Cplx(Signal[j].Re, Signal[j].Im);
		x[j + le] = Cplx(Signal[j + le].Re, Signal[j + le].Im);
	};

	/*  start inverse fft */
	le = WindowSize;
	windex = 1;
	for (l = 0; l < fft_size_tmp; l++) {
		le = le / 2;
		/*  first iteration with no multiplies */
		for (i = 0; i < WindowSize; i += 2 * le) {
			temp = (x[i] + x[i + le]);
			x[i + le] = (x[i] - x[i + le]);
			x[i] = temp;
		};

		/*  remaining iterations use stored w */
		wptr = windex - 1;
		for (j = 1; j < le; j++) {
			for (i = j; i < WindowSize; i += 2 * le) {
				temp = x[i] + x[i + le];
				tm = x[i] - x[i + le];
				x[i + le] = tm * w[wptr];
				x[i] = temp;
			};
			wptr += windex;
		};
		windex = 2 * windex;
	};

	/*  rearrange data by bit reversing */
	j = 0;
	for (i = 1; i < (WindowSize - 1); i++) {
		k = WindowSize / 2;
		while (k <= j) {
			j = j - k;
			k = k / 2;
		};
		j = j + k;
		if (i < j) {
			temp = x[j];
			x[j] = x[i];
			x[i] = temp;
		};
	};

	for (i = 0; i < WindowSize; i++) {
		u = x[i];
		Spectr[i].Re = u.real();
		Spectr[i].Im = u.imag();
	};
}






/* *************************************************************************
ifft - In-place radix 2 decimation in time inverse FFT

Requires pointer to complex array, x and power of 2 size of FFT, m
(size of FFT = 2**m).  Places inverse FFT output on top of input
frequency domain COMPLEX array.

complex<float_type> ifft(complex<float_type> *x, int m)

*************************************************************************/
void ifft(std::vector<std::complex<double> >& Spectr, int WindowSize) {
	std::complex<double> u, temp, tm;
	int wptr;
	int i, j, k, l, le, windex;
	double scale;

	int fft_size_tmp = 1;
	i = WindowSize;

	while (1) {
		i = i >> 1;
		if (i == 1) break;
		fft_size_tmp++;
	};

	/*  n = 2**m = inverse fft length */
	//int n = 1 << m;

	le = WindowSize / 2;
	std::vector<std::complex<double> > w(WindowSize + 1);
	std::vector<std::complex<double> > x(WindowSize + 1);

	/*  calculate the w values recursively */
	std::complex<double> w_inc = expj(TWOPI / WindowSize);
	std::complex<double> w_x = w_inc;

	for (j = 0; j < le; j++) {
		w[j] = w_x;
		w_x *= w_inc;
		x[j] = Spectr[j];
		x[j + le] = Spectr[j + le];
	};

	/*  start inverse fft */
	le = WindowSize;
	windex = 1;
	for (l = 0; l < fft_size_tmp; l++) {
		le = le / 2;
		/*  first iteration with no multiplies */
		for (i = 0; i < WindowSize; i += 2 * le) {
			temp = (x[i] + x[i + le]);
			x[i + le] = (x[i] - x[i + le]);
			x[i] = temp;
		};

		/*  remaining iterations use stored w */
		wptr = windex - 1;
		for (j = 1; j < le; j++) {
			for (i = j; i < WindowSize; i += 2 * le) {
				temp = x[i] + x[i + le];
				tm = x[i] - x[i + le];
				x[i + le] = tm * w[wptr];
				x[i] = temp;
			};
			wptr += windex;
		};
		windex = 2 * windex;
	};

	/*  rearrange data by bit reversing */
	j = 0;
	for (i = 1; i < (WindowSize - 1); i++) {
		k = WindowSize / 2;
		while (k <= j) {
			j = j - k;
			k = k / 2;
		};

		j = j + k;
		if (i < j) {
			temp = x[j];
			x[j] = x[i];
			x[i] = temp;
		};
	};

	/*  scale all results by 1/n */
	scale = (double)(1.0 / WindowSize);
	for (i = 0; i < WindowSize; i++)
		Spectr[i] = scale * x[i];
}





void ifft_nip(std::vector<std::complex<double> > Spectr, std::vector<std::complex<double> >& Signal, int WindowSize) {
	std::complex<double> u, temp, tm;
	int wptr;
	int i, j, k, l, le, windex;
	double scale;

	int fft_size_tmp = 1;
	i = WindowSize;

	while (1) {
		i = i >> 1;
		if (i == 1) break;
		fft_size_tmp++;
	};

	/*  n = 2**m = inverse fft length */
	//int n = 1 << m;

	le = WindowSize / 2;
	std::vector<std::complex<double> > w(WindowSize + 1);
	std::vector<std::complex<double> > x(WindowSize + 1);

	/*  calculate the w values recursively */
	std::complex<double> w_inc = expj(TWOPI / WindowSize);
	std::complex<double> w_x = w_inc;

	for (j = 0; j < le; j++) {
		w[j] = w_x;
		w_x *= w_inc;
		x[j] = Spectr[j];
		x[j + le] = Spectr[j + le];
	};

	/*  start inverse fft */
	le = WindowSize;
	windex = 1;
	for (l = 0; l < fft_size_tmp; l++) {
		le = le / 2;
		/*  first iteration with no multiplies */
		for (i = 0; i < WindowSize; i += 2 * le) {
			temp = (x[i] + x[i + le]);
			x[i + le] = (x[i] - x[i + le]);
			x[i] = temp;
		};

		/*  remaining iterations use stored w */
		wptr = windex - 1;
		for (j = 1; j < le; j++) {
			for (i = j; i < WindowSize; i += 2 * le) {
				temp = x[i] + x[i + le];
				tm = x[i] - x[i + le];
				x[i + le] = tm * w[wptr];
				x[i] = temp;
			};
			wptr += windex;
		};
		windex = 2 * windex;
	};

	/*  rearrange data by bit reversing */
	j = 0;
	for (i = 1; i < (WindowSize - 1); i++) {
		k = WindowSize / 2;
		while (k <= j) {
			j = j - k;
			k = k / 2;
		};

		j = j + k;
		if (i < j) {
			temp = x[j];
			x[j] = x[i];
			x[i] = temp;
		};
	};

	/*  scale all results by 1/n */
	scale = (double)(1.0 / WindowSize);
	for (i = 0; i < WindowSize; i++)
		Signal[i] = scale * x[i];
}

void ifft_nip_1(PDCplx Spectr, PWCplx Signal, int WindowSize) {
	std::complex<double> u, temp, tm;
	int wptr;
	int i, j, k, l, le, windex;
	double scale;

	int fft_size_tmp = 1;
	i = WindowSize;

	while (1) {
		i = i >> 1;
		if (i == 1) break;
		fft_size_tmp++;
	};

	/*  n = 2**m = inverse fft length */
	//int n = 1 << m;

	le = WindowSize / 2;
	std::vector<std::complex<double> > w(WindowSize + 1);
	std::vector<std::complex<double> > x(WindowSize + 1);

	/*  calculate the w values recursively */
	std::complex<double> w_inc = expj(TWOPI / WindowSize);
	std::complex<double> w_x = w_inc;

	for (j = 0; j < le; j++) {
		w[j] = w_x;
		w_x *= w_inc;
		x[j] = Cplx(Spectr[j].Re, Spectr[j].Im);
		x[j + le] = Cplx(Spectr[j + le].Re, Spectr[j + le].Im);
	};

	/*  start inverse fft */
	le = WindowSize;
	windex = 1;
	for (l = 0; l < fft_size_tmp; l++) {
		le = le / 2;
		/*  first iteration with no multiplies */
		for (i = 0; i < WindowSize; i += 2 * le) {
			temp = (x[i] + x[i + le]);
			x[i + le] = (x[i] - x[i + le]);
			x[i] = temp;
		};

		/*  remaining iterations use stored w */
		wptr = windex - 1;
		for (j = 1; j < le; j++) {
			for (i = j; i < WindowSize; i += 2 * le) {
				temp = x[i] + x[i + le];
				tm = x[i] - x[i + le];
				x[i + le] = tm * w[wptr];
				x[i] = temp;
			};
			wptr += windex;
		};
		windex = 2 * windex;
	};

	/*  rearrange data by bit reversing */
	j = 0;
	for (i = 1; i < (WindowSize - 1); i++) {
		k = WindowSize / 2;
		while (k <= j) {
			j = j - k;
			k = k / 2;
		};

		j = j + k;
		if (i < j) {
			temp = x[j];
			x[j] = x[i];
			x[i] = temp;
		};
	};

	/*  scale all results by 1/n */
	scale = (double)(1.0 / WindowSize);
	for (i = 0; i < WindowSize; i++) {
		u = scale * x[i];
		Signal[i].Re = u.real();
		Signal[i].Im = u.imag();
	};
}













TDCBlocker::TDCBlocker() {
	pole = 0.9999;
	A = (long)(32768.0*(1.0 - pole));
	acc = 0;
	prev_x = 0;
	prev_y = 0;

	gain = 0.9999;
}

TDCBlocker::~TDCBlocker() {

}



void TDCBlocker::Process(int16_t &in_out) {

	float inp = (float)in_out;
	float outp;

	outp = inp - lastinput + gain * lastoutput;
	lastinput  = inp;
	lastoutput = outp;

	in_out = outp;

	return;

	acc   -= prev_x;
	prev_x = (long)in_out << 15;
	acc   += prev_x;
	acc   -= A*prev_y;
	prev_y = acc >> 15;               // quantization happens here
	in_out = (short)prev_y;
}


