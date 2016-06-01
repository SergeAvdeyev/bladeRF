#ifndef DEVICE_THREAD_H
#define DEVICE_THREAD_H

#include <QApplication>
#include <QMutex>
#include <QThread>

#include "libbladeRF.h"

#define WM_USER  1001
#define ToDevMsg_Connect     WM_USER + 1
#define FromDev_Connect      WM_USER + 2 // result

#define ToDevMsg_Disconnect  WM_USER + 3
#define FromDev_Disconnect   WM_USER + 4 // disconnected from device event



#define ToDevMsg_RxStart     WM_USER + 100		// Start receive
#define FromDev_RxStart      WM_USER + 101		// Start receive result

#define ToDevMsg_RxStop      WM_USER + 102		// Stop receive
#define FromDev_RxStop       WM_USER + 103		// Stop receive result

#define ToDevMsg_TxStart     WM_USER + 104		// Start transmit
#define FromDev_TxStart      WM_USER + 105		// Start transmit result

#define ToDevMsg_TxStop      WM_USER + 106		// Stop transmit
#define FromDev_TxStop       WM_USER + 107		// Stop transmit result


#define FromDev_RxProcessError WM_USER + 110	// Result of receiving
#define FromDev_RxProcessData  WM_USER + 111	// Result of receiving

#define FromDev_TxProcessError WM_USER + 112	// Result of receiving

struct TTxDataItem {
	char * Data;
	int DataSize;
};
typedef TTxDataItem * PTxDataItem;


class TBladeRfDevice : public QObject {
	Q_OBJECT
	private:
		struct bladerf * FDevice;
		QMutex FMutex;
		bool FConnected;
		bool FRxActive;
		void * FRxBuffer;
		bool FRxConfigured;

		uint32_t FRxBufferSize;
		uint32_t FRxSamplesLen;

		bool		FTxActive;
		int32_t *	FTxBuffer;
		uint32_t	FTxSamplesLen;
		uint32_t	FTxBufferSize;
		bool		FTxConfigured;
		QList<PTxDataItem> FTxDataList;

		void Lock();
		void Unlock();
		void Connect();
		int IsFpgaConfigured();
		void Disconnect();


		void RxStart();
		void RxStop();

		void TxStart();
		void TxStop();

		int RxInitSync();
		int RxProcess();

		int TxInitSync();
		void TxProcess();

	public:
		explicit TBladeRfDevice(QObject *parent = 0);
		~TBladeRfDevice();

		int RxSetFreqSync(int AFreq);
		int RxGetFreqSync(int *AFreqActual);

		int RxSetSrSync(uint32_t ASrFreq, uint32_t *ASrFreqActual);
		int RxGetSrSync(uint32_t * ASrFreqActual);

		int RxSetBwSync(uint32_t ABandWidth, uint32_t * ABandWidthActual);
		int RxGetBwSync(uint32_t * ABandWidthActual);

		int RxSetLnaSync(bladerf_lna_gain ALna);
		int RxGetLnaSync(uint32_t * ALnaActual);

		int RxSetVga1Sync(int AVga1, int * AVga1Actual);
		int RxGetVga1Sync(int * AVga1Actual);

		int RxSetVga2Sync(int AVga2, int * AVga2Actual);
		int RxGetVga2Sync(int * AVga2Actual);

		//function RxStartSync: Integer;
		//function RxStopSync: Integer;




		int TxSetFreqSync(int AFreq);
		int TxGetFreqSync(int * AFreqActual);

		int TxSetSrSync(uint32_t ASrFreq, uint32_t * ASrFreqActual);
		int TxGetSrSync(uint32_t * ASrFreqActual);

		int TxSetBwSync(uint32_t ABandWidth, uint32_t *ABandWidthActual);
		int TxGetBwSync(uint32_t *ABandWidthActual);

		int TxSetVga1Sync(int AVga1, int * AVga1Actual);
		int TxGetVga1Sync(int * AVga1Actual);

		int TxSetVga2Sync(int AVga2, int * AVga2Actual);
		int TxGetVga2Sync(int * AVga2Actual);

		//function TxStartSync: Integer;
		//function TxStopSync: Integer;

		bool TxAddData(char *AData, int ADataSize);

		bool CanSend();





	public slots:
		void Execute();
		void MessageToDevice(uint16_t Msg, int64_t WParam, int64_t LParam);

	signals:
		void MessageFromDevice(uint16_t Msg, int64_t WParam, int64_t LParam);
};


#endif // DEVICE_THREAD_H
