#ifndef WAVE_GEN_H
#define WAVE_GEN_H

#include <QVector>
#include <math.h>

#define PI M_PI
#define TWOPI (PI*2)

class TWaveGenerator {
	public:
		TWaveGenerator();
		~TWaveGenerator() {}
		void Init(int SampleRate, int Frequency, int InitPhase);

	private:
		int FSampleRate;
		double FInitPhase;
		int FFrequency;
		long double FInterval;
		double FAngle;
	public:
		bool GetWave(QVector<double> * DataBufferX,
					 QVector<double> * DataBufferY,
					 int Offset,
					 int NumSamples,
					 int Amplitude);
		bool GetWave(QVector<double> &DataBuffer,
					 int NumSamples,
					 int Amplitude);
		bool GetWave(float * DataBuffer,
					 int NumSamples,
					 int Amplitude);

};

class TWaveGenerator_Par {  // paraboloc generator
	public:
		TWaveGenerator_Par();
		~TWaveGenerator_Par() {}
		void Init(int SampleRate, int Frequency, int Amplitude, int InitPhase);

	private:
		int FSampleRate;
		int FInitPhase;
		int FFrequency;

		long m; // = FSampleRate/(2.0*FFrequency);
		double a; // = -m;
		double b; // = 0;
		double A;
		int ph;
		//long double FInterval;
		//double FAngle;
	public:
		bool GetWave(QVector<double> & DataBufferX,
					 QVector<double> & DataBufferY,
					 int Offset,
					 int NumSamples);
		bool GetWave(QVector<double> &DataBuffer,
					 int NumSamples);
		bool GetWave(float * DataBuffer,
					 int NumSamples);

};

#endif // WAVE_GEN_H

