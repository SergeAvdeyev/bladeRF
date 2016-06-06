#ifndef TX_THREAD_H
#define TX_THREAD_H

#include <QApplication>
#include <QMutex>
#include <QThread>

#include "libbladeRF.h"
#include "fft.h"
#include "butt_filter.h"
#include "liquid.h"

#define TxSamplesLen 20000

struct TTxDataItem {
	void * Buffer;
	int BufferSize;
};
typedef TTxDataItem * PTxDataItem;


class TTxThread : public QObject {
	Q_OBJECT

	private:
		// Properties
		struct bladerf * FDevice;

		bool FTxActive;
		TWCplx * FTxBuffer;
		double * FTxBufferRe;
		double * FTxBufferIm;
		//int FTxSamplesLen;
		int FTxBufferSize;

		TButtFilter * IIR_FILTER_I;
		TButtFilter * IIR_FILTER_Q;

		modem Modem;
		nco_crcf nco_tx;

		QMutex FMutex;
		bool FTerminated;
		QList<PTxDataItem> DataList;

		unsigned char sym_out;

		// Methods
		int TxInitSync();
		int Process();

		void Lock();
		void Unlock();

		bool IsTerminated();
		PTxDataItem GetBuffer();
	public:
		explicit TTxThread(struct bladerf * Device);
		~TTxThread();
		void Terminate();

		bool CanSend();
		int AddBuffer(void * Buffer, int BufferSize);

	public slots:
		void Execute();
		void Finished();

	signals:
		//void NewRxData(void * Buffer, int BufferSize);
		void TxStart(int Status);
		void TxStop();
		void TxFinished();
};



#endif // TX_THREAD_H
