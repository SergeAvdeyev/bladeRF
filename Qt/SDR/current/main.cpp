#include <QApplication>
#include "mainwindow.h"

//#include "DspFilters/Dsp.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	// Create a Chebyshev type I Band Stop filter of order 3
	// with state for processing 2 channels of audio.
//	Dsp::SimpleFilter <Dsp::ChebyshevI::BandStop <3>, 2> f;

//	f.setup (3,    // order
//		   44100,// sample rate
//		   4000, // center frequency
//		   880,  // band width
//		   1);   // ripple dB
//	f.process(numSamples, arrayOfChannels);


	TMainWindow w;
	w.show();

	return a.exec();
}
