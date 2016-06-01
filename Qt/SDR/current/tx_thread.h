#ifndef TX_THREAD_H
#define TX_THREAD_H

#include <QApplication>
#include <QMutex>

#include "libbladeRF.h"

class TTxThread : public QObject {
	Q_OBJECT

	private:
		struct bladerf * FDevice;

		bool FTxActive;
		void * FTxBuffer;
		int FTxSamplesLen;
		int FTxBufferSize;

		QMutex FMutex;
		bool FTerminated;

		int TxInitSync();

		int Process();

		void Lock();
		void Unlock();

		bool IsTerminated();

	public:
		explicit TTxThread(struct bladerf * Device);
		~TTxThread();
		void Terminate();

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
