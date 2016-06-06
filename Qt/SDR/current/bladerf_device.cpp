
#include "bladerf_device.h"
#include "mainwindow.h"

TBladeRfDevice::TBladeRfDevice(QObject *parent) : QObject(parent) {
	FParent = (TMainWindow *)parent;
	FDevice = NULL;
	FConnected = false;
	//FRxActive = false;
	//FRxBuffer = NULL;
	FRxConfigured = false;
	//FTxActive = false;
	FTxConfigured = false;

	FRxThread = NULL;
	FTxThread = NULL;
	FRxContainer = NULL;
	FTxContainer = NULL;

	//FTxDataList = new QList;
}

TBladeRfDevice::~TBladeRfDevice() {
	Disconnect();
	//if (FRxBuffer != NULL)
	//	free(FRxBuffer);

	//if FTxBuffer <> nil then
	//  FreeMemory(FTxBuffer);

	//while (FTxDataList.count() > 0) {
	//	free(FTxDataList.at(0));
	//	FTxDataList.removeFirst();
	//};
	//FTxDataList.Free;

}




int TBladeRfDevice::Connect() {

	if (FDevice != NULL) return -1;

	int Status = bladerf_open(&FDevice, NULL);
	if (Status != 0) {
		return Status;
	};
	Status = IsFpgaConfigured();
	FConnected = Status == 0;
	if (!FConnected) {
		bladerf_close(FDevice);
		FDevice = NULL;
	};
	return Status;
}

int TBladeRfDevice::IsFpgaConfigured() {
	int Status = -1;

	if (FDevice == NULL) return Status;
	Status = bladerf_is_fpga_configured(FDevice);
	if (Status == 0)
		Status = bladerf_load_fpga(FDevice, "hostedx115.rbf");
	else if (Status == 1)
		Status = 0;
	return Status;
}

int TBladeRfDevice::Disconnect() {
	if ((!FConnected) || (FDevice == NULL)) return -1;

	RxStop();
	TxStop();

	bladerf_close(FDevice);
	FDevice = NULL;
	FConnected = false;
	//FRxActive = false;
	FRxConfigured = false;
	//FTxActive = false;
	FTxConfigured = false;

	return 0;
}

















///////////////////////////////////////////
////////                        ///////////
////////     RX                 ///////////
////////                        ///////////
///////////////////////////////////////////

int TBladeRfDevice::RxSetFreqSync(int AFreq) {
	uint32_t FFreqTmp;

	int Result = 0;
	if ((!FConnected) || (FDevice == NULL)) return Result;

	FFreqTmp = AFreq;
	FFreqTmp = FFreqTmp*1000;
	Result = bladerf_set_frequency(FDevice, BLADERF_MODULE_RX, FFreqTmp);

	return Result;
}

int TBladeRfDevice::RxGetFreqSync(int * AFreqActual) {
	uint32_t FFreqTmp;

	*AFreqActual = 0;
	int Result = 0;
	if ((!FConnected) || (FDevice == NULL)) return Result;

	Result = bladerf_get_frequency(FDevice, BLADERF_MODULE_RX, &FFreqTmp);
	*AFreqActual = FFreqTmp / 1000;
	return Result;
}

int TBladeRfDevice::RxSetSrSync(uint32_t ASrFreq, uint32_t *ASrFreqActual) {
	*ASrFreqActual = 0;
	if ((!FConnected) || (FDevice == NULL)) return 0;

	return bladerf_set_sample_rate(FDevice, BLADERF_MODULE_RX, ASrFreq, ASrFreqActual);
}



int TBladeRfDevice::RxGetSrSync(uint32_t * ASrFreqActual) {
	*ASrFreqActual = 0;
	if ((!FConnected) || (FDevice == NULL)) return 0;

	return bladerf_get_sample_rate(FDevice, BLADERF_MODULE_RX, ASrFreqActual);
}

int TBladeRfDevice::RxSetBwSync(uint32_t ABandWidth, uint32_t * ABandWidthActual) {
	if ((!FConnected) || (FDevice == NULL)) return 0;

	return bladerf_set_bandwidth(FDevice, BLADERF_MODULE_RX, ABandWidth, ABandWidthActual);
}

int TBladeRfDevice::RxGetBwSync(uint32_t * ABandWidthActual) {
	if ((!FConnected) || (FDevice == NULL)) return 0;

	return bladerf_get_bandwidth(FDevice, BLADERF_MODULE_RX, ABandWidthActual);
}


int TBladeRfDevice::RxSetLnaSync(bladerf_lna_gain ALna) {
	if ((!FConnected) || (FDevice == NULL)) return 0;

	return bladerf_set_lna_gain(FDevice, ALna);
}

int TBladeRfDevice::RxGetLnaSync(uint32_t * ALnaActual) {
	Q_UNUSED(ALnaActual);
	if ((!FConnected) || (FDevice == NULL)) return 0;

	//Result := bladerf_get_bandwidth(FDevice, BLADERF_MODULE_RX, ABandWidthActual);
	return 0;
}

int TBladeRfDevice::RxSetVga1Sync(int AVga1, int *AVga1Actual) {
	if ((!FConnected) || (FDevice == NULL)) return 0;

	int Result = bladerf_set_rxvga1(FDevice, AVga1);
	if (Result == 0)
		Result = RxGetVga1Sync(AVga1Actual);
	return Result;
}




int TBladeRfDevice::RxGetVga1Sync(int * AVga1Actual) {
	if ((!FConnected) || (FDevice == NULL)) return 0;

	return bladerf_get_rxvga1(FDevice, AVga1Actual);
}

int TBladeRfDevice::RxSetVga2Sync(int AVga2, int *AVga2Actual) {
	if ((!FConnected) || (FDevice == NULL)) return 0;

	int Result = bladerf_set_rxvga2(FDevice, AVga2);
	if (Result == 0)
		Result = RxGetVga2Sync(AVga2Actual);
	return Result;
}

int TBladeRfDevice::RxGetVga2Sync(int * AVga2Actual) {
	if ((!FConnected) || (FDevice == NULL)) return 0;

	return bladerf_get_rxvga2(FDevice, AVga2Actual);
}





///////////////////////////////////////////
////////                        ///////////
////////     TX                 ///////////
////////                        ///////////
///////////////////////////////////////////

int TBladeRfDevice::TxSetFreqSync(int AFreq) {
	if ((!FConnected) || (FDevice == NULL)) return 0;

	uint32_t FFreqTmp;

	FFreqTmp = AFreq;
	FFreqTmp = FFreqTmp*1000;
	return bladerf_set_frequency(FDevice, BLADERF_MODULE_TX, FFreqTmp);
}

int TBladeRfDevice::TxGetFreqSync(int * AFreqActual) {
	if ((!FConnected) || (FDevice == NULL)) return 0;

	uint32_t FFreqTmp;

	int Result = bladerf_get_frequency(FDevice, BLADERF_MODULE_TX, &FFreqTmp);
	*AFreqActual = FFreqTmp / 1000;
	return Result;
}


int TBladeRfDevice::TxSetSrSync(uint32_t ASrFreq, uint32_t *ASrFreqActual) {
	if ((!FConnected) || (FDevice == NULL)) return 0;

	return bladerf_set_sample_rate(FDevice, BLADERF_MODULE_TX, ASrFreq, ASrFreqActual);
}

int TBladeRfDevice::TxGetSrSync(uint32_t *ASrFreqActual) {
	if ((!FConnected) || (FDevice == NULL)) return 0;

	return bladerf_get_sample_rate(FDevice, BLADERF_MODULE_TX, ASrFreqActual);
}


int TBladeRfDevice::TxSetBwSync(uint32_t ABandWidth, uint32_t *ABandWidthActual) {
	if ((!FConnected) || (FDevice == NULL)) return 0;

	return bladerf_set_bandwidth(FDevice, BLADERF_MODULE_TX, ABandWidth, ABandWidthActual);
}

int TBladeRfDevice::TxGetBwSync(uint32_t *ABandWidthActual) {
	if ((!FConnected) || (FDevice == NULL)) return 0;

	return bladerf_get_bandwidth(FDevice, BLADERF_MODULE_TX, ABandWidthActual);
}

int TBladeRfDevice::TxSetVga1Sync(int AVga1, int *AVga1Actual) {
	if ((!FConnected) || (FDevice == NULL)) return 0;

	int Result = bladerf_set_txvga1(FDevice, AVga1);
	if (Result == 0)
		Result = TxGetVga1Sync(AVga1Actual);
	return Result;
}

int TBladeRfDevice::TxGetVga1Sync(int *AVga1Actual) {
	if ((!FConnected) || (FDevice == NULL)) return 0;

	return bladerf_get_txvga1(FDevice, AVga1Actual);
}

int TBladeRfDevice::TxSetVga2Sync(int AVga2, int *AVga2Actual) {
	if ((!FConnected) || (FDevice == NULL)) return 0;

	int Result = bladerf_set_txvga2(FDevice, AVga2);
	if (Result == 0)
		Result = TxGetVga2Sync(AVga2Actual);
	return Result;
}

int TBladeRfDevice::TxGetVga2Sync(int *AVga2Actual) {
	if ((!FConnected) || (FDevice == NULL)) return 0;

	return bladerf_get_txvga2(FDevice, AVga2Actual);
}







void TBladeRfDevice::RxStart() {

	if ((!FConnected) || (FDevice == NULL)) return;

	if (FRxContainer != NULL) return;

	FRxThread = new TRxThread(FDevice);
	FRxContainer = new QThread();
	connect(FRxThread, SIGNAL(NewRxData(void*,int)), FParent, SLOT(OnRxData(void*,int)));
	connect(FRxThread, SIGNAL(RxStart(int)), FParent, SLOT(OnRxStart(int)));
	connect(FRxThread, SIGNAL(RxStop()), FParent, SLOT(OnRxStop()));
	connect(FRxThread, SIGNAL(RxFinished()), this, SLOT(OnRxFinished()));

	connect(FRxContainer, SIGNAL(started()), FRxThread, SLOT(Execute()));
	connect(FRxContainer, SIGNAL(finished()), FRxThread, SLOT(Finished()));
	FRxThread->moveToThread(FRxContainer);
	FRxContainer->start();
}


void TBladeRfDevice::RxStop() {
	if (FRxThread == NULL) return;

	FRxThread->Terminate();
	FRxContainer->terminate();
	FRxContainer->wait();
}

void TBladeRfDevice::OnRxFinished() {

	disconnect(FRxThread, SIGNAL(NewRxData(void*,int)));
	disconnect(FRxThread, SIGNAL(RxStart(int)));
	disconnect(FRxThread, SIGNAL(RxStop()));

	disconnect(FRxContainer, SIGNAL(started()));
	disconnect(FRxContainer, SIGNAL(finished()));
	delete FRxThread;
	delete FRxContainer;
	FRxThread = NULL;
	FRxContainer = NULL;
}
















void TBladeRfDevice::TxStart() {
	if ((!FConnected) || (FDevice == NULL)) return;

	if (FTxContainer != NULL) return;

	FTxThread = new TTxThread(FDevice);
	FTxContainer = new QThread();
	//connect(FTxThread, SIGNAL(NewRxData(void*,int)), FParent, SLOT(OnRxData(void*,int)));
	connect(FTxThread, SIGNAL(TxStart(int)), FParent, SLOT(OnTxStart(int)));
	connect(FTxThread, SIGNAL(TxStop()), FParent, SLOT(OnTxStop()));
	connect(FTxThread, SIGNAL(TxFinished()), this, SLOT(OnTxFinished()));

	connect(FTxContainer, SIGNAL(started()), FTxThread, SLOT(Execute()));
	connect(FTxContainer, SIGNAL(finished()), FTxThread, SLOT(Finished()));
	FTxThread->moveToThread(FTxContainer);
	FTxContainer->start();
}


void TBladeRfDevice::TxStop() {
	if (FTxThread == NULL) return;

	FTxThread->Terminate();
	FTxContainer->terminate();
	FTxContainer->wait();
}


void TBladeRfDevice::OnTxFinished() {

	//disconnect(FTxThread, SIGNAL(NewRxData(void*,int)));
	disconnect(FTxThread, SIGNAL(TxStart(int)));
	disconnect(FTxThread, SIGNAL(TxStop()));
	disconnect(FTxThread, SIGNAL(TxFinished()));

	disconnect(FTxContainer, SIGNAL(started()));
	disconnect(FTxContainer, SIGNAL(finished()));
	delete FTxThread;
	delete FTxContainer;
	FTxThread = NULL;
	FTxContainer = NULL;
}

bool TBladeRfDevice::TxCanSend() {
	if ((!FConnected) || (FDevice == NULL)) return false;
	if (FTxThread == NULL) return false;

	return FTxThread->CanSend();
}

int TBladeRfDevice::TxData(PWCplx Buffer, int BufferSize) {
	if ((!FConnected) || (FDevice == NULL)) return -1;
	if (FTxThread == NULL) return -1;

	return FTxThread->AddBuffer(Buffer, BufferSize);
}



















