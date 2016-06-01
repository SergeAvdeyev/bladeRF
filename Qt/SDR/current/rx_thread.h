#ifndef RX_THREAD_H
#define RX_THREAD_H

#include <QApplication>
#include <QMutex>

#include "libbladeRF.h"

class TRxThread : public QObject {
	Q_OBJECT

	private:
		struct bladerf * FDevice;

		bool FRxActive;
		void * FRxBuffer;
		int FRxSamplesLen;
		int FRxBufferSize;

		QMutex FMutex;
		bool FTerminated;

		int RxInitSync();

		int Process();

		void Lock();
		void Unlock();

		bool IsTerminated();

	public:
		explicit TRxThread(struct bladerf * Device);
		~TRxThread();
		void Terminate();

	public slots:
		void Execute();
		void Finished();

	signals:
		void NewRxData(void * Buffer, int BufferSize);
		void RxStart(int Status);
		void RxStop();
		void RxFinished();
};


#endif // RX_THREAD_H
