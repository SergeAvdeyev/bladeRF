
#include "wave_gen.h"

TWaveGenerator::TWaveGenerator() {
	FSampleRate = 0;
	FInitPhase = 0;
}

void TWaveGenerator::Init(int SampleRate, int Frequency, int InitPhase) {
	FSampleRate = SampleRate;
	if (InitPhase == 0)
		FInitPhase = 0;
	else
		FInitPhase = (TWOPI)/(360/InitPhase);
	FFrequency = Frequency;

	FAngle = 0;

	FInterval = round(((TWOPI)/(FSampleRate/FFrequency))*10000)/10000;
	//FInterval = (2*M_PI)/(FSampleRate/FFrequency);
}

bool TWaveGenerator::GetWave(QVector<double> * DataBufferX,
							 QVector<double> * DataBufferY,
							 int Offset,
							 int NumSamples,
							 int Amplitude) {
	if (FSampleRate == 0) return false;
	if (Offset > DataBufferX->size()) return false;
	int i, to;

	to = Offset + NumSamples;
	if (to > DataBufferX->size())
		to = DataBufferX->size();

//	for (i = Offset; i < to; i++) {
//		(*DataBufferX)[i] = i;

//		(*DataBufferY)[i] = Amplitude*sin((FInterval*FAngle) + FInitPhase);
//		FAngle++;
//	};
//	FAngle = FAngle % FSampleRate;

	for (i = Offset; i < to; i++) {
		(*DataBufferX)[i] = i;

		(*DataBufferY)[i] = Amplitude*sin((FAngle) + FInitPhase);
		FAngle += FInterval;
		//FAngle = FAngle % 360;
	};
	if (FAngle >= TWOPI)
		FAngle = FAngle - round(FAngle/TWOPI)*TWOPI;

	return true;
}


bool TWaveGenerator::GetWave(QVector<double> & DataBuffer,
							 int NumSamples,
							 int Amplitude) {
	if (FSampleRate == 0) return false;
	int i, to;

	to = NumSamples;
	if (to > DataBuffer.size())
		to = DataBuffer.size();

	for (i = 0; i < to; i++) {
		DataBuffer[i] = Amplitude*sin((FAngle) + FInitPhase);
		FAngle += FInterval;
	};
	if (FAngle >= TWOPI)
		FAngle = FAngle - round(FAngle/TWOPI)*TWOPI;

	return true;
}

bool TWaveGenerator::GetWave(float * DataBuffer,
							 int NumSamples,
							 int Amplitude) {
	if (FSampleRate == 0) return false;
	int i;
	float * DataBufferPtr = DataBuffer;

	for (i = 0; i < NumSamples; i++) {
		*DataBufferPtr = Amplitude*sin((FAngle) + FInitPhase);
		FAngle += FInterval;
		DataBufferPtr++;
	};
	if (FAngle >= TWOPI)
		FAngle = FAngle - round(FAngle/TWOPI)*TWOPI;

	return true;
}














TWaveGenerator_Par::TWaveGenerator_Par() {
	FSampleRate = 0;
	FInitPhase = 0;
}

void TWaveGenerator_Par::Init(int SampleRate, int Frequency, int InitPhase) {
	FSampleRate = SampleRate;
	if (InitPhase == 0)
		FInitPhase = 0;
	else
		FInitPhase = (TWOPI)/(360/InitPhase);
	FFrequency = Frequency;

	//FAngle = 0;

	//FInterval = round(((TWOPI)/(FSampleRate/FFrequency))*10000)/10000;
	//FInterval = (2*M_PI)/(FSampleRate/FFrequency);



//	printf("usage: %s s f a n\n", argv[0]);
//	printf("  Generates a parabolic waveform.\n");
//	printf("  s = sampling rate\n");
//	printf("  f = parabolic wave frequency\n");
//	printf("  a = amplitude\n");
//	printf("  n = number of samples to generate\n");

//	double FSampleRate = strtod(argv[1], NULL);
//	double f = strtod(argv[2], NULL);
//	double A = strtod(argv[3], NULL);
//	long n = strtol(argv[4], NULL, 10);


}

bool TWaveGenerator_Par::GetWave(QVector<double> & DataBufferX,
							 QVector<double> & DataBufferY,
							 int Offset,
							 int NumSamples,
							 int Amplitude) {
	if (FSampleRate == 0) return false;
	if (Offset > DataBufferX.size()) return false;
	long i, to;

	to = Offset + NumSamples;
	if (to > DataBufferX.size())
		to = DataBufferX.size();

	long m = FSampleRate/(2.0*FFrequency);
	long a = -m;
	long b = 0;

	double A = Amplitude * 4.0/(double)(m*m);
	for (i = Offset; i < to; i++) {
		DataBufferX[i] = i;
		if (i % m == 0) {
			a += m;
			b += m;
			A = -A;
		};
		DataBufferY[i] = A*(i - a)*(i - b);
	};

	return true;
}


bool TWaveGenerator_Par::GetWave(QVector<double> & DataBuffer,
							 int NumSamples,
							 int Amplitude) {
	if (FSampleRate == 0) return false;
	int i, to;

	to = NumSamples;
	if (to > DataBuffer.size())
		to = DataBuffer.size();

	long m = FSampleRate/(2.0*FFrequency);
	double a = -m;
	double b = 0;

	double A = Amplitude * 4.0/(double)(m*m);
	for (i = 0; i < to; i++) {
		if (i % m == 0) {
			a += m;
			b += m;
			A = -A;
		};
		DataBuffer[i] = A*(i - a)*(i - b);
	};

	return true;
}

bool TWaveGenerator_Par::GetWave(float * DataBuffer,
							 int NumSamples,
							 int Amplitude) {
	if (FSampleRate == 0) return false;
	int i;
	float * DataBufferPtr = DataBuffer;

	long m = FSampleRate/(2.0*FFrequency);
	long a = -m;
	long b = 0;

	double A = Amplitude * 4.0/(double)(m*m);
	for (i = 0; i < NumSamples; i++) {
		if (i % m == 0) {
			a += m;
			b += m;
			A = -A;
		};
		*DataBufferPtr = A*(i - a)*(i - b);
		DataBufferPtr++;
	};

	return true;
}
