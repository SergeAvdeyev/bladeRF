
#include "tx_thread.h"

TTxThread::TTxThread(struct bladerf * Device) : QObject(0) {
	FDevice = Device;
	FTerminated = false;
}

TTxThread::~TTxThread() {

}

void TTxThread::Lock() {
	FMutex.lock();
}

void TTxThread::Unlock() {
	FMutex.unlock();
}

int TTxThread::TxInitSync() {
	uint32_t num_buffers;
	uint32_t buffer_size;  // Must be a multiple of 1024
	uint32_t num_transfers;
	uint32_t timeout_ms;

	num_buffers   = 32;     // 16
	buffer_size   = 32768;  // 4096
	num_transfers = 16;     // num_buffers / 2
	timeout_ms    = 3500;

	int Result = bladerf_sync_config(FDevice,
									 BLADERF_MODULE_TX,
									 BLADERF_FORMAT_SC16_Q11,
									 num_buffers,
									 buffer_size,
									 num_transfers,
									 timeout_ms);
	return Result;
}

int TTxThread::Process() {
	int Status;
	//char * FBufferTmp;

	Status = bladerf_sync_tx(FDevice, FTxBuffer, FTxSamplesLen, NULL, 5000);
	if (Status != 0) {
		return Status;
		//emit MessageFromDevice(FromDev_RxProcessError, Status, 0);
	};
	//FBufferTmp = (char *)malloc(FTxBufferSize);
	//memcpy(FBufferTmp, FTxBuffer, FTxBufferSize);
	//emit NewRxData(FBufferTmp, FRxBufferSize);
	//FBladeRfDevice->OnRxData(FBufferTmp, FRxBufferSize);
	return 0;
}

void TTxThread::Terminate() {
	Lock();
	FTerminated = true;
	Unlock();
}

bool TTxThread::IsTerminated() {
	bool Result;
	Lock();
	Result = FTerminated;
	Unlock();

	return Result;
}


void TTxThread::Execute() {

	int Status = TxInitSync();
	if (Status != 0) {
		emit TxStart(Status);
		return;
	};
	Status = bladerf_enable_module(FDevice, BLADERF_MODULE_TX, true);
	if (Status != 0) {
		emit TxStart(Status);
		return;
	};

	//FRxSamplesLen := 512*1000; // May be any (reasonable) size
	FTxSamplesLen = 48*1000; // May be any (reasonable) size
	FTxBufferSize = FTxSamplesLen*sizeof(int16_t)*2; // May be any (reasonable) size

	FTxBuffer = malloc(FTxBufferSize);

	FTxActive = true;
	emit TxStart(0);

	while (!IsTerminated()) {
		if (Process() != 0)
			break;
	};
	bladerf_enable_module(FDevice, BLADERF_MODULE_TX, false);
	free(FTxBuffer);
	emit TxStop();
}

void TTxThread::Finished() {
	emit TxFinished();
}

