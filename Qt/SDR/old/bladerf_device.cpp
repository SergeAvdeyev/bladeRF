
#include "bladerf_device.h"

TBladeRfDevice::TBladeRfDevice(QObject *parent) : QObject(parent) {
	FDevice = NULL;
	FConnected = false;
	FRxActive = false;
	FRxBuffer = NULL;
	FRxConfigured = false;
	FTxActive = false;
	FTxConfigured = false;
	//FTxDataList = new QList;
}

TBladeRfDevice::~TBladeRfDevice() {
	Disconnect();
	if (FRxBuffer != NULL)
		free(FRxBuffer);

	//if FTxBuffer <> nil then
	//  FreeMemory(FTxBuffer);

	while (FTxDataList.count() > 0) {
		free(FTxDataList.at(0));
		FTxDataList.removeFirst();
	};
	//FTxDataList.Free;

}

void TBladeRfDevice::Lock() {
	FMutex.lock();
}


void TBladeRfDevice::Unlock() {
   FMutex.unlock();
}


void TBladeRfDevice::Connect() {
	int Status;

	if (FDevice != NULL) return;

	Status = bladerf_open(&FDevice, NULL);
	if (Status != 0) {
		emit MessageFromDevice(FromDev_Connect, Status, 0);
		return;
	};
	Status = IsFpgaConfigured();
	FConnected = Status == 0;
	if (!FConnected) {
		bladerf_close(FDevice);
		FDevice = NULL;
	};
	emit MessageFromDevice(FromDev_Connect, Status, 0);
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

void TBladeRfDevice::Disconnect() {
	if ((!FConnected) || (FDevice == NULL)) return;

	RxStop();
	TxStop();

	bladerf_close(FDevice);
	FDevice = NULL;
	FConnected = false;
	FRxActive = false;
	FRxConfigured = false;
	FTxActive = false;
	FTxConfigured = false;

	emit MessageFromDevice(FromDev_Disconnect, 0, 0);
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







int TBladeRfDevice::RxInitSync() {
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
	if (Result == 0)
		FRxConfigured = true;
	return Result;
}



void TBladeRfDevice::RxStart() {
	int Status;
	//rx_size : UINT;

	if ((!FConnected) || (FDevice == NULL)) return;
	if (FRxActive) return;

	Status = RxInitSync();
	if (Status != 0) {
		emit MessageFromDevice(FromDev_RxStart, Status, 0);
		return;
	};
	Status = bladerf_enable_module(FDevice, BLADERF_MODULE_RX, true);
	if (Status != 0) {
		emit MessageFromDevice(FromDev_RxStart, Status, 0);
		return;
	};

	//FRxSamplesLen := 512*1000; // May be any (reasonable) size
	FRxSamplesLen = 48*1000; // May be any (reasonable) size
	FRxBufferSize = FRxSamplesLen*sizeof(int16_t)*2; // May be any (reasonable) size

	FRxBuffer = malloc(FRxBufferSize);

	FRxActive = true;

	emit MessageFromDevice(FromDev_RxStart, Status, 0);
}

/*
{
function TBladeRfDevice::RxStartSync: Integer;
begin
  Result := -1;
  if (not FConnected) OR (FDevice = nil) then Exit;
  Result := 0;
  if FRxActive then Exit;

  Result := RxInitSync;
  if Result <> 0 then Exit;

  if FRxBuffer = nil then begin
	FRxSamplesLen := 512*1000; // May be any (reasonable) size
	FRxBufferSize := FRxSamplesLen*SizeOf(SmallInt)*2; // May be any (reasonable) size
	GetMem(FRxBuffer, FRxBufferSize);
  end;

  Result := bladerf_enable_module(FDevice, BLADERF_MODULE_RX, True);
  if Result <> 0 then begin
	//FreeMemory(FRxBuffer);
	//FRxBuffer := nil;
	Exit;
  end;

  FRxActive := True;
end;
}

*/

void TBladeRfDevice::RxStop() {
	int Status;

	if (!FRxActive) return;
	Status = bladerf_enable_module(FDevice, BLADERF_MODULE_RX, false);
	if (Status != 0) {
		emit MessageFromDevice(FromDev_RxStop, Status, 0);
		return;
	};

	FRxActive = false;

	emit MessageFromDevice(FromDev_RxStop, 0, 0);
}

/*
{
function TBladeThread.RxStopSync: Integer;
begin
  Lock;
  FRxActive := False;
  //while True do
  //  if not RxProcess then break;
  Result := bladerf_enable_module(FDevice, BLADERF_MODULE_RX, False);
  Unlock;

  //FreeMemory(FRxBuffer);
  //FRxBuffer := nil;
end;
}
*/



int TBladeRfDevice::RxProcess() {
	int Status;
	char * FBufferTmp;

	int Result = 0;

	if ((!FConnected) || (FDevice == NULL)) return 0;
	if (!FRxActive) return 0;

	Status = bladerf_sync_rx(FDevice, FRxBuffer, FRxSamplesLen, NULL, 5000);
	if (Status != 0) {
		Disconnect();
		emit MessageFromDevice(FromDev_RxProcessError, Status, 0);
		return 0;
	};
	//FBufferTmp = (char *)malloc(FRxBufferSize);
	//memcpy(FBufferTmp, FRxBuffer, FRxBufferSize);
	//emit MessageFromDevice(FromDev_RxProcessData, (int64_t)FBufferTmp, FRxBufferSize);
	return Result;
}











int TBladeRfDevice::TxInitSync() {
	uint32_t num_buffers;
	uint32_t buffer_size;  // Must be a multiple of 1024
	uint32_t num_transfers;
	uint32_t timeout_ms;

	//Result := 0;
	//if FTxConfigured then Exit;
	num_buffers   = 4;     // 16
	buffer_size   = 8192;  // 4096
	num_transfers = 2;     // num_buffers / 2
	timeout_ms    = 100;

	int Result = bladerf_sync_config(FDevice,
									 BLADERF_MODULE_TX,
									 BLADERF_FORMAT_SC16_Q11,
									 num_buffers,
									 buffer_size,
									 num_transfers,
									 timeout_ms);
	if (Result == 0)
		FTxConfigured = true;
	return Result;
}



void TBladeRfDevice::TxStart() {
	int Status;
	//rx_size : UINT;

	if ((!FConnected) || (FDevice == NULL)) return;
	if (FTxActive) return;

	Status = TxInitSync();
	if (Status != 0) {
		emit MessageFromDevice(FromDev_TxStart, Status, 0);
		return;
	};
	Status = bladerf_enable_module(FDevice, BLADERF_MODULE_TX, true);
	if (Status != 0) {
		emit MessageFromDevice(FromDev_TxStart, Status, 0);
		return;
	};

	FTxSamplesLen = 512*1000; // May be any (reasonable) size
	FTxBufferSize = FTxSamplesLen*sizeof(int16_t)*2; // May be any (reasonable) size
	FTxBuffer = (int32_t *)malloc(FTxBufferSize);

	FTxActive = true;

	emit MessageFromDevice(FromDev_TxStart, Status, 0);
}

/*
{
function TBladeRfDevice::TxStartSync: Integer;
begin
  Result := -1;
  if (not FConnected) OR (FDevice = nil) then Exit;
  Result := 0;
  if FTxActive then Exit;

  Result := TxInitSync;
  if Result <> 0 then Exit;

  //if FTxBuffer = nil then begin
	FTxSamplesLen := 512*1000; // May be any (reasonable) size
	FTxBufferSize := FTxSamplesLen*SizeOf(SmallInt)*2; // May be any (reasonable) size
  //  GetMem(FTxBuffer, FTxBufferSize);
  //end;

  Result := bladerf_enable_module(FDevice, BLADERF_MODULE_TX, True);
  if Result <> 0 then
	Exit;

  FTxActive := True;
end;
}
*/


void TBladeRfDevice::TxStop() {
	int Status;

	if (!FTxActive) return;
	FTxActive = false;
	Status = bladerf_enable_module(FDevice, BLADERF_MODULE_TX, false);
	if (Status != 0) {
		emit MessageFromDevice(FromDev_TxStop, Status, 0);
		return;
	};

	emit MessageFromDevice(FromDev_TxStop, 0, 0);
}

/*
int TBladeRfDevice::TxStopSync() {
begin
  Result := -1;
  if (not FConnected) OR (FDevice = nil) then Exit;
  if not FTxActive then Exit;

  Lock;
  FTxActive := False;
  Result := bladerf_enable_module(FDevice, BLADERF_MODULE_TX, False);
  Unlock;
}
*/

bool TBladeRfDevice::CanSend() {

	bool Result = false;
	if ((!FConnected) || (FDevice == NULL)) return false;
	if (!FTxActive) return false;

	Lock();
	Result = FTxDataList.count() < 2;
	Unlock();
	return Result;
}

bool TBladeRfDevice::TxAddData(char *AData, int ADataSize) {
	PTxDataItem FDataItem;

	bool Result = false;
	FDataItem = (PTxDataItem)malloc(sizeof(TTxDataItem));
	FDataItem->Data = AData;
	FDataItem->DataSize = ADataSize;

	Lock();
	FTxDataList.append(FDataItem);
	Result = true;
	Unlock();

	return Result;
}

void TBladeRfDevice::TxProcess() {
	int Status;
	PTxDataItem FDataItem;
	//FDataSended : Integer;

	if ((!FConnected) || (FDevice == NULL)) return;
	if (!FTxActive) return;

	FDataItem = NULL;
	return;

	//FDataSended := 0;
	//while True do begin
	Lock();
	if (FTxDataList.count() == 0) {
		Unlock();
		//Break;
		return;
	};
	FDataItem = FTxDataList.takeFirst();
	Unlock();

	if (FDataItem == NULL) return;

	//Status := bladerf_sync_tx(FDevice, FDataItem^.Data, FDataItem^.DataSize div 4, nil, 5000);
	Status = bladerf_sync_tx(FDevice, FTxBuffer, FTxSamplesLen, NULL, 5000);
	if (Status != 0) {
		Disconnect();
		emit MessageFromDevice(FromDev_TxProcessError, Status, 0);
		//Break;
		return;
	};
	//FreeMem(FDataItem^.Data);
	//FreeMem(FDataItem);

	//inc(FDataSended, FDataItem^.DataSize);
	//if FDataSended >= FTxBufferSize then Break;
  //end;
}




// Handle messages from main thread
void TBladeRfDevice::MessageToDevice(uint16_t Msg, int64_t WParam, int64_t LParam) {
	Q_UNUSED(WParam);
	Q_UNUSED(LParam);
	switch (Msg) {
		case ToDevMsg_Connect :
			Connect();
			break;
		case ToDevMsg_Disconnect :
			Disconnect();
			break;

		case ToDevMsg_RxStart :
			RxStart();
			break;
		case ToDevMsg_RxStop :
			RxStop();
			break;

		case ToDevMsg_TxStart :
			TxStart();
			break;

		case ToDevMsg_TxStop :
			TxStop();
			break;

		default:
			break;
	}

}



void TBladeRfDevice::Execute() {

	while (1) {
		//RxProcess();

		QThread::msleep(100);
	};

}


/*
void TBladeRfDevice::Execute;
var msg : tagMSG;
	//P : PChar;
	//n : Cardinal;
	//ErrorBuf : PChar;
	//status, oldstatus : Integer;
	//S : String;
begin
  //oldstatus := 0;
  while not Terminated do
  begin
	 if PeekMessage(msg, 0, 0, 0, PM_REMOVE) then
	 begin
	   case msg.message of
		 ToDevMsg_Connect    : Connect;
		 ToDevMsg_Disconnect : Disconnect;

		 ToDevMsg_RxStart    : RxStart;
		 ToDevMsg_RxStop     : RxStop;

		 ToDevMsg_TxStart    : TxStart;
		 ToDevMsg_TxStop     : TxStop;
	   end;
	 end
	 else
	 begin
	   if (not FRxActive) AND (not FTxActive) then
	   begin
		 Suspend;
		 Continue;
	   end;
	 end;

	 //Lock;
	 RxProcess;
	 //Unlock;

	 try
	   TxProcess;
	 except
	 end;
	 //Sleep(2);
  end;
  if FConnected then
	Disconnect;
}
*/









/*
const


{
  ToDevMsg_TxSetFreq   = WM_USER + 5; // Set TX Frequency
  FromDev_TxSetFreq    = WM_USER + 5; // Set TX Frequency result
  ToDevMsg_TxGetFreq   = WM_USER + 6; // Get TX Frequency
  FromDev_TxGetFreq    = WM_USER + 6; // Get TX Frequency result

  ToDevMsg_TxSetSR     = WM_USER + 9; // Set TX Sample Rate
  FromDev_TxSetSR      = WM_USER + 9; // Set TX Sample Rate result
  ToDevMsg_TxGetSR     = WM_USER + 10; // Get TX Sample Rate
  FromDev_TxGetSR      = WM_USER + 10; // Get TX Sample Rate result

  ToDevMsg_TxSetBW     = WM_USER + 13; // Set TX BandWidth
  FromDev_TxSetBW      = WM_USER + 13; // Set TX BandWidth result
  ToDevMsg_TxGetBW     = WM_USER + 14; // Get TX BandWidth
  FromDev_TxGetBW      = WM_USER + 14; // Get TX BandWidth result

  ToDevMsg_TxVga1Gain  = WM_USER + 21; // Set TX Vga1 Gain
  ToDevMsg_TxVga2Gain  = WM_USER + 22; // Set TX Vga2 Gain
}

  ToDevMsg_RxStart     = WM_USER + 100; // Start receive
  FromDev_RxStart      = WM_USER + 101; // Start receive result

  ToDevMsg_RxStop      = WM_USER + 102; // Stop receive
  FromDev_RxStop       = WM_USER + 103; // Stop receive result

  ToDevMsg_TxStart     = WM_USER + 104; // Start transmit
  FromDev_TxStart      = WM_USER + 105; // Start transmit result

  ToDevMsg_TxStop      = WM_USER + 106; // Stop transmit
  FromDev_TxStop       = WM_USER + 107; // Stop transmit result

  FromDev_RxProcessError = WM_USER + 110; // Result of receiving
  FromDev_RxProcessData  = WM_USER + 111; // Result of receiving

  FromDev_TxProcessError = WM_USER + 112; // Result of receiving


type
  TTxDataItem = record
	Data     : Pointer;
	DataSize : Integer;
  end;
  PTxDataItem = ^TTxDataItem;

  TBladeThread = class(TThread)
  private
	hMainWnd         : THandle;
	FCriticalSection : TCriticalSection;

	FDevice       : Pointer;
	FConnected    : Boolean;


	FTxActive     : Boolean;
	FTxBuffer     : PSmallInt;
	FTxSamplesLen : UINT;
	FTxBufferSize : UINT;
	FTxConfigured : Boolean;

	FTxDataList : TList;

	procedure Lock;
	procedure Unlock;

	procedure Connect;
	procedure Disconnect;

	procedure RxStart;
	procedure RxStop;

	procedure TxStart;
	procedure TxStop;

	function RxInitSync: Integer;
	function RxProcess: Boolean;

	function TxInitSync: Integer;
	procedure TxProcess;

	function IsFpgaConfigured: Integer;

  protected
	procedure Execute; override;

  public
	destructor Destroy; override;

	function RxSetFreqSync(AFreq: Integer): Integer;
	function RxGetFreqSync(var AFreqActual: Integer): Integer;

	function RxSetSrSync(ASrFreq: Cardinal; var ASrFreqActual: Cardinal): Integer;
	function RxGetSrSync(var ASrFreqActual: Cardinal): Integer;

	function RxSetBwSync(ABandWidth: Cardinal; var ABandWidthActual: Cardinal): Integer;
	function RxGetBwSync(var ABandWidthActual: Cardinal): Integer;

	function RxSetLnaSync(ALna: Byte): Integer;
	function RxGetLnaSync(var ALnaActual: Cardinal): Integer;

	function RxSetVga1Sync(AVga1: Integer; var AVga1Actual: Integer): Integer;
	function RxGetVga1Sync(var AVga1Actual: Integer): Integer;

	function RxSetVga2Sync(AVga2: Integer; var AVga2Actual: Integer): Integer;
	function RxGetVga2Sync(var AVga2Actual: Integer): Integer;

	//function RxStartSync: Integer;
	//function RxStopSync: Integer;




	function TxSetFreqSync(AFreq: Integer): Integer;
	function TxGetFreqSync(var AFreqActual: Integer): Integer;

	function TxSetSrSync(ASrFreq: Cardinal; var ASrFreqActual: Cardinal): Integer;
	function TxGetSrSync(var ASrFreqActual: Cardinal): Integer;

	function TxSetBwSync(ABandWidth: Cardinal; var ABandWidthActual: Cardinal): Integer;
	function TxGetBwSync(var ABandWidthActual: Cardinal): Integer;

	function TxSetVga1Sync(AVga1: Integer; var AVga1Actual: Integer): Integer;
	function TxGetVga1Sync(var AVga1Actual: Integer): Integer;

	function TxSetVga2Sync(AVga2: Integer; var AVga2Actual: Integer): Integer;
	function TxGetVga2Sync(var AVga2Actual: Integer): Integer;

	//function TxStartSync: Integer;
	//function TxStopSync: Integer;

	function TxAddData(AData: Pointer; ADataSize: Integer): Boolean;

	function CanSend: Boolean;
  end;

implementation


*/
