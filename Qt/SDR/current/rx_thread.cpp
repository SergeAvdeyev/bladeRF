
#include "rx_thread.h"

TRxThread::TRxThread(struct bladerf * Device) : QObject(0) {
	FDevice = Device;
	FTerminated = false;
}

TRxThread::~TRxThread() {

}

void TRxThread::Lock() {
	FMutex.lock();
}

void TRxThread::Unlock() {
	FMutex.unlock();
}

int TRxThread::RxInitSync() {
	uint32_t num_buffers;
	uint32_t buffer_size;  // Must be a multiple of 1024
	uint32_t num_transfers;
	uint32_t timeout_ms;

	num_buffers   = 32;     // 16
	buffer_size   = 32768;  // 4096
	num_transfers = 16;     // num_buffers / 2
	timeout_ms    = 3500;

	int Result = bladerf_sync_config(FDevice,
									 BLADERF_MODULE_RX,
									 BLADERF_FORMAT_SC16_Q11,
									 num_buffers,
									 buffer_size,
									 num_transfers,
									 timeout_ms);
	return Result;
}

int TRxThread::Process() {
	int Status;
	char * FBufferTmp;

	Status = bladerf_sync_rx(FDevice, FRxBuffer, FRxSamplesLen, NULL, 5000);
	if (Status != 0) {
		return Status;
		//emit MessageFromDevice(FromDev_RxProcessError, Status, 0);
	};
	FBufferTmp = (char *)malloc(FRxBufferSize);
	memcpy(FBufferTmp, FRxBuffer, FRxBufferSize);
	emit NewRxData(FBufferTmp, FRxBufferSize);
	//FBladeRfDevice->OnRxData(FBufferTmp, FRxBufferSize);
	return 0;
}

void TRxThread::Terminate() {
	Lock();
	FTerminated = true;
	Unlock();
}

bool TRxThread::IsTerminated() {
	bool Result;
	Lock();
	Result = FTerminated;
	Unlock();

	return Result;
}


void TRxThread::Execute() {

	int Status = RxInitSync();
	if (Status != 0) {
		emit RxStart(Status);
		return;
	};
	Status = bladerf_enable_module(FDevice, BLADERF_MODULE_RX, true);
	if (Status != 0) {
		emit RxStart(Status);
		return;
	};

	//FRxSamplesLen := 512*1000; // May be any (reasonable) size
	FRxSamplesLen = 48*1000; // May be any (reasonable) size
	FRxBufferSize = FRxSamplesLen*sizeof(int16_t)*2; // May be any (reasonable) size

	FRxBuffer = malloc(FRxBufferSize);

	FRxActive = true;
	emit RxStart(0);

	while (!IsTerminated()) {
		if (Process() != 0)
			break;
	};
	bladerf_enable_module(FDevice, BLADERF_MODULE_RX, false);
	free(FRxBuffer);
	emit RxStop();
}

void TRxThread::Finished() {
	emit RxFinished();
}

