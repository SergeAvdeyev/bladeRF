/* All rights reserved.
*
*/
/**
*
* Author: Viktor Signaievkyi
*/

//#include "stdafx.h"
#include "iir_filter.h"

/**
  Class constructor.
 */
//template
CFilterIIRStream::CFilterIIRStream(void) {
	m_bIsInitialized = false;
	m_pFilteredData = nullptr;
	m_FilteredDataLength = 0;
	m_Latency = 0;
	m_a0 = 0;
	m_a1 = 0;
	m_a2 = 0;
	m_b0 = 0;
	m_b1 = 0;
	m_b2 = 0;
	m_x1 = 0;
	m_x2 = 0;
	m_y1 = 0;
	m_y2 = 0;
	m_ft = NONE;
	m_bIsBackProcessing = false;
}

/**
  Class destructor.
 */

CFilterIIRStream::~CFilterIIRStream(void) {
	Flush();
}

/**
  Initializing/ReInitializing IIR filter by necessary parameters.
  \param ft - filter type (Low-Pass, High-Pass, Band-Pass and Band-Stop...).
  \param f0 - decision frequency. Value between 0.0 and sampling frequency divided by 2.
  \param sr - sampling rate (frequency).
  \param Q - quality factor (f0/bandwidth).
  \param dBFilterGain - used only in case of PEAKING_EQ, LOW_SHELF and HIGH_SHELF filters.
  \Note:
  \LPF, HPF, BPF, BSF, NOTCH - use f0, sr, Q;
  \PEAKING_EQ - uses f0, sr, Q, dBFilterGain;
  \LOW_SHELF, HIGH_SHELF - use f0, sr, dBFilterGain.
 */

int CFilterIIRStream::Init(FILTERTYPE ft, float f0, float sr, float Q, float dBFilterGain) {
	float A, omega, sine, cosine, alpha, beta;

	m_ft = ft;

	A = pow(10, dBFilterGain /40);
	//= sqrt(pow(10, dBFilterGain /20));
	omega = (float)(2 * M_PI * f0 /sr);
	sine = sin(omega);
	cosine = cos(omega);
	alpha = sine/(2*Q);
	beta = sqrt(A + A);
	//beta = sqrt(A)/Q;

	//TODO : Check beta correctness

	switch (ft) {
	case LPF: {
			m_b0 = (1 - cosine)/2;
			m_b1 = 1 - cosine;
			m_b2 = (1 - cosine)/2;
			m_a0 = 1 + alpha;
			m_a1 = -2*cosine;
			m_a2 = 1 - alpha;
		}
		break;
	case HPF:
		{
			m_b0 = (1 + cosine)/2;
			m_b1 = -(1 + cosine);
			m_b2 = (1 + cosine)/2;
			m_a0 = 1 + alpha;
			m_a1 = -2*cosine;
			m_a2 = 1 - alpha;
		}
		break;
	case BPF:
		{
			m_b0 = alpha;
			m_b1 = 0;
			m_b2 = -alpha;
			m_a0 = 1 + alpha;
			m_a1 = -2*cosine;
			m_a2 = 1 - alpha;
		}
		break;
	case BSF:
	case NOTCH:
		{
			m_b0 = 1;
			m_b1 = -2 * cosine;
			m_b2 = 1;
			m_a0 = 1 + alpha;
			m_a1 = -2 * cosine;
			m_a2 = 1 - alpha;
		}
		break;
	case PEAKING_EQ:
		{
			m_b0 = 1 + (alpha * A);
			m_b1 = -2 * cosine;
			m_b2 = 1 - (alpha * A);
			m_a0 = 1 + (alpha /A);
			m_a1 = -2 * cosine;
			m_a2 = 1 - (alpha /A);
		}
		break;
	case LOW_SHELF:
		{
			m_b0 = A * ((A + 1) - (A - 1) * cosine + beta * sine);
			m_b1 = 2 * A * ((A - 1) - (A + 1) * cosine);
			m_b2 = A * ((A + 1) - (A - 1) * cosine - beta * sine);
			m_a0 = (A + 1) + (A - 1) * cosine + beta * sine;
			m_a1 = -2 * ((A - 1) + (A + 1) * cosine);
			m_a2 = (A + 1) + (A - 1) * cosine - beta * sine;
		}
		break;
	case HIGH_SHELF:
		{
			m_b0 = A * ((A + 1) + (A - 1) * cosine + beta * sine);
			m_b1 = -2 * A * ((A - 1) + (A + 1) * cosine);
			m_b2 = A * ((A + 1) + (A - 1) * cosine - beta * sine);
			m_a0 = (A + 1) - (A - 1) * cosine + beta * sine;
			m_a1 = 2 * ((A - 1) - (A + 1) * cosine);
			m_a2 = (A + 1) - (A - 1) * cosine - beta * sine;
		}
		break;
	default:
		{
		}
	}

	// precompute the coefficients
	m_b0 /= m_a0;
	m_b1 /= m_a0;
	m_b2 /= m_a0;
	m_a1 /= m_a0;
	m_a2 /= m_a0;

	// zero in/out initial samples
	m_x1 = 0;
	m_x2 = 0;
	m_y1 = 0;
	m_y2 = 0;

	m_bIsInitialized = true;

	return 0; //RET_OK;
}

/**
  Initializing/ReInitializing IIR filter by necessary parameters.
  \param ft - filter type (Low-Pass, High-Pass, Band-Pass and Band-Stop...).
  \param w0 - decision frequency (in radians from 0 to pi).
  \param bw - bandwidth in radians (from 0 to pi). It should be choosen dep on ft and w0.
  \param dBFilterGain - used only in case of PEAKING_EQ, LOW_SHELF and HIGH_SHELF filters.
  \Note:
  \LPF, HPF - use only w0;
  \BPF, BSF, NOTCH - use w0 and bw;
  \PEAKING_EQ - uses w0, bw and dBFilterGain;
  \LOW_SHELF, HIGH_SHELF - use w0 and dBFilterGain.
 */
int CFilterIIRStream::Init(FILTERTYPE ft, float w0, float bw, float dBFilterGain) {
	m_ft = ft;

	// let assign to the sampling some dummy but fixed value
	float sr = (float)(2*M_PI);

	// exclude mistakes and exception situations
	w0 = abs(w0);
	bw = abs(bw);

	// analyzing and fixing improper coefficients, if possible
	switch(ft)
	{
	case LPF:
		{
			// automatically attenuating bandwidth
			bw = w0;
		}
		break;
	case HPF:
		{
			// automatically attenuating bandwidth
			bw = sr/2 - w0;
		}
		break;
	case BPF:
	case BSF:
	case NOTCH:
		{
			// we are out of upper possible frequency range
			if((w0 + bw/2) > sr/2)
			{
				// it is unpossible to set up bandwidth which is out of max frequency
				return -1; //RET_INVALID_ARG;
			}
			// we are out of lower possible frequency range
			else if((w0 - bw/2) < 0) {
				// it is unpossible to set up bandwidth which is out of min frequency (0)
				return -1; //RET_INVALID_ARG;
			}
		}
		break;
	case PEAKING_EQ:
		{
		}
		break;
	case LOW_SHELF:
		{
		}
		break;
	case HIGH_SHELF:
		{
		}
		break;
	default:
		{
		}
	}

	return Init(ft, w0, sr, w0/bw, dBFilterGain);
}

/**
  Exactly filtering method.
  \param pBuffIn - pointer to the buffer to apply filter.
  \param BuffInLength - length of the buffer to filter.
 */
int CFilterIIRStream::Process(float *pBuffIn, long BuffInLength) {
	// filter should be initialized before call of this method
	if(m_bIsInitialized == false)
		return -1; //RET_ERROR;

	if (pBuffIn == NULL)
		return -2; //RET_INVALID_ARG;

	if(BuffInLength < 1)
		return -2; //RET_INVALID_ARG;

	ApplyFilter(pBuffIn, BuffInLength);

	return 0; //RET_OK;
}

/**
  Flushing. Used to free memory and set up all settings of the class to default
 */
int CFilterIIRStream::Flush() {
	m_bIsInitialized = false;
	m_pFilteredData = NULL;
	m_FilteredDataLength = 0;
	m_Latency = 0;
	m_a0 = 0;
	m_a1 = 0;
	m_a2 = 0;
	m_b0 = 0;
	m_b1 = 0;
	m_b2 = 0;
	m_x1 = 0;
	m_x2 = 0;
	m_y1 = 0;
	m_y2 = 0;
	m_ft = NONE;
	m_bIsBackProcessing = false;

	return 0; //RET_OK;
}

/**
  Internal filtering method.
  \param indata - pointer to the buffer to apply filter.
  \param numSampsToProcess - length of the buffer to filter.
 */
void CFilterIIRStream::ApplyFilter(float *indata, long numSampsToProcess) {
   float out = 0, in = 0;

   for (int i = 0; i < numSampsToProcess; i++) {
	  if (m_bIsBackProcessing == true)
		in = indata[numSampsToProcess - i - 1];
	  else
		in = indata[i];

	  // this optimization is usefull for some king of compilers/processors
	  if(m_ft == LPF) {
		  out = m_b0*(in + 2*m_x1 + m_x2) - m_a1*m_y1 - m_a2*m_y2;
	  } else if(m_ft == HPF) {
		  out = m_b0*(in - 2*m_x1 + m_x2) - m_a1*m_y1 - m_a2*m_y2;
	  } else if(m_ft == BPF) {
		  out = m_b0*(in - m_x2) - m_a1*m_y1 - m_a2*m_y2;
	  } else if(m_ft == BSF) {
		  out = (in + m_x2)/m_a0 + m_a1*(m_x1 - m_y1) - m_a2*m_y2;
	  } else {
		  // applying IIR routine
		  out = (m_b0*in + m_b1*m_x1 + m_b2*m_x2 - m_a1*m_y1 - m_a2*m_y2);
	  };

	  m_x2 = m_x1;
	  m_x1 = in;
	  m_y2 = m_y1;
	  m_y1 = out;

	  if (m_bIsBackProcessing == true)
		indata[numSampsToProcess - i - 1] = out;
	  else
		indata[i] = out;
   }

   m_pFilteredData = indata;
   m_FilteredDataLength = numSampsToProcess;
   m_Latency = 0;
}

/**
  Interface for the getting processed data.
  Returns pointer to the internal data of the class.
 */
const float * CFilterIIRStream::GetFilteredData(void) {
	return m_pFilteredData;
}

/**
  Interface to get processed data length.
  Returns size of processed data in number of samples.
 */
long CFilterIIRStream::GetFilteredDataLength(void) {
	return m_FilteredDataLength;
}

/**
  Interface to get latency of real-time processing.
  It means how many samples should we process to get first processed sample (or data block).
  It is associated to the delay in analog circuits.
 */
int CFilterIIRStream::GetFilteredDataLatency(void) {
	return m_Latency;
}

//// instantiation
////template class CFilterIIRStream;
////template class CFilterIIRStream;
