#ifndef CFFT_H
#define CFFT_H

#include <stdlib.h>
#include <complex>
#include <math.h>

#ifndef PI
#define PI M_PI
#endif
#define TWOPI 2*M_PI
#define HALFPI 0.5*M_PI
#define QUARTPI 0.25*M_PI

// if defined USE_FLOAT, default to float type
#ifndef USE_FLOAT
typedef double float_type;
#else
typedef float float_type;
#endif

typedef std::complex<float_type> CPLX;

//! \file
//! \brief  General Purpose C++ complex FFT Transform
//
//! This is a general-purpose C++ complex FFT transform class.
//! it is defined as a template over a complex type. For instance,
//! if using gnu gcc, the complex type is
//!			complex<float_type>
//! And you declare the cfft class as
//!          cfft<complex<float_type>>
//!
//! The underlying CPLX type requires:
//!  CPLX()
//!  operator = , CPLX(CPLX const&)
//!	CPLX(float_type,float_type)	[used on cos/sin]
//!	CPLX operator*( CPLX , float_type )
//!  CPLX conj(CPLX const &);		[conjugate]
//!  ComPlex::operator @ (CPLX , CPLX )	[ where @ =* + - ]
//! \author Tony Kirke
//!  \ingroup classes misc
class cfft {
	int N, log2N;           //! these define size of FFT buffer
	CPLX *w;                //! array [N/2] of cos/sin values
	int *bitrev;            //! bit-reversing table, in 0..N
	float_type fscales[2];  //! f-transform scalings
	float_type iscales[2];  //! i-transform scales
	void fft_func(CPLX *buf, int iflag);

	public:
		cfft(int size,                                            // size is power of 2
			float_type scalef1 = 0.5, float_type scalef2 = 1.0,  // fwd transform scalings
			float_type scalei1 = 1.0, float_type scalei2 = 1.0   // rev xform
		);
	~cfft();
	inline void fft(CPLX *buf) { //! perform forward fft on buffer
		fft_func(buf, 0);
	}
	inline void ifft(CPLX *buf) { //! perform reverse fft on buffer
		fft_func(buf, 1);
	}
	inline int length() const { return N; }
	//! used to fill in last half of complex spectrum of real signal
	//! when the first half is already there.
	void hermitian(CPLX *buf);

};  // class cfft

#endif // CFFT_H

