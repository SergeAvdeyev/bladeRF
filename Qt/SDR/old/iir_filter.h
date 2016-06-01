#ifndef IIR_FILTER_H
#define IIR_FILTER_H

#include <stdlib.h>
#include "math.h"
//#include "common.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif /* !M_PI */

#ifndef M_LN2
#define M_LN2 0.69314718055994530942
#endif /* !M_LN2 */

class CFilterIIRStream
{
public:
	CFilterIIRStream(void);
	~CFilterIIRStream(void);

	typedef enum {
		LPF,        //low-pass filter
		HPF,        //high-pass filter
		BPF,        //band-pass filter
		BSF,        //band-stop filter
		NOTCH,      //band-stop filter
		PEAKING_EQ, //equalizer filter
		LOW_SHELF,
		HIGH_SHELF,
		NONE
	} FILTERTYPE;

	int Init(
		FILTERTYPE ft,
		float f0,
		float sr,
		float Q,
		float dBFilterGain);

	int Init(
		FILTERTYPE ft,
		float w0,
		float bw = 0,
		float dBFilterGain = 0);

	// it is inplace filter!
	int Process(float * pBuffIn, long BuffInLength);
	int Flush();

	const float *    GetFilteredData(void);
	long        GetFilteredDataLength(void);
	int         GetFilteredDataLatency(void);

	// this option should be used if the blocks from
	// snd file passing in the reverse order (blockN,
	// block(N-1),..., Block1). This can be used if
	// we want to get linear phase response. To do
	// that we need to process sound in forward order,
	// then in reversed order.
	bool SetBackProcessing(bool bState) {
		return m_bIsBackProcessing = bState;
	}

	bool GetBackProcessing() {
		return m_bIsBackProcessing;
	}

protected:
	void ApplyFilter(float * indata, long numSampsToProcess);

	bool    m_bIsInitialized;
	bool    m_bIsBackProcessing;

	float*  m_pFilteredData;
	long    m_FilteredDataLength;
	int     m_Latency;

	float   m_a0,
			m_a1,
			m_a2,
			m_b0,
			m_b1,
			m_b2;

	float   m_x1,
			m_x2;

	float   m_y1,
			m_y2;

	FILTERTYPE  m_ft;
};
#endif // IIR_FILTER_H
