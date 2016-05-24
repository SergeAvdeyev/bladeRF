unit BladeRFThread;

interface

uses Windows, Sysutils, Messages, syncobjs,
  Classes, BladeRFIntf;

const
  ToDevMsg_Connect     = WM_USER + 1;
  FromDev_Connect      = WM_USER + 1; // result

  ToDevMsg_Disconnect  = WM_USER + 2;
  FromDev_Disconnect   = WM_USER + 2; // disconnected from device event


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

    FRxActive     : Boolean;
    FRxBuffer     : PSmallInt;
    FRxBufferSize : UINT;
    FRxSamplesLen : UINT;
    FRxConfigured : Boolean;

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
    constructor Create(HWND: THandle);
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

constructor TBladeThread.Create(HWND: THandle);
begin
  FCriticalSection := TCriticalSection.Create;
  FreeOnTerminate := False;
  hMainWnd := HWND;
  FConnected := False;
  FRxActive := False;
  FRxBuffer := nil;
  FRxConfigured := False;
  FTxActive := False;
  //FTxBuffer := nil;
  FTxConfigured := False;
  FTxDataList := TList.Create;
  inherited Create(True);
end;

destructor TBladeThread.Destroy;
begin
  Disconnect;
  if FRxBuffer <> nil then
    FreeMemory(FRxBuffer);

  //if FTxBuffer <> nil then
  //  FreeMemory(FTxBuffer);

  while FTxDataList.Count > 0 do begin
    FreeMem(FTxDataList.Items[0]);
    FTxDataList.Delete(0);
  end;
  FTxDataList.Free;

  FCriticalSection.Free;
  inherited;
end;

procedure TBladeThread.Lock;
begin
   FCriticalSection.Enter;
end;


procedure TBladeThread.Unlock;
begin
   FCriticalSection.Leave;
end;




procedure TBladeThread.Connect;
var Status : Integer;
begin
  if FDevice <> nil then Exit;

  Status := bladerf_open(FDevice, nil);
  if Status <> 0 then begin
    SendMessage(hMainWnd, FromDev_Connect, Status, 0);
    Exit;
  end;
  Status := IsFpgaConfigured ;
  FConnected := Status = 0;
  SendMessage(hMainWnd, FromDev_Connect, Status, 0);
end;

function TBladeThread.IsFpgaConfigured: Integer;
var Status : Integer;
begin
  if FDevice = nil then Exit;
  Status := bladerf_is_fpga_configured(FDevice);
  if Status = 0 then
    Status := bladerf_load_fpga(FDevice, 'hostedx115.rbf')
  else if Status = 1 then
    Status := 0;
end;

procedure TBladeThread.Disconnect;
begin
  if (not FConnected) OR (FDevice = nil) then Exit;

  RxStop;
  TxStop;

  bladerf_close(FDevice);
  FDevice := nil;
  FConnected := False;
  FRxActive := False;
  FRxConfigured := False;
  FTxActive := False;
  FTxConfigured := False;
  SendMessage(hMainWnd, FromDev_Disconnect, 0, 0);
end;




///////////////////////////////////////////
////////                        ///////////
////////     RX                 ///////////
////////                        ///////////
///////////////////////////////////////////

function TBladeThread.RxSetFreqSync(AFreq: Integer): Integer;
var FFreqTmp : Cardinal;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  FFreqTmp := AFreq;
  FFreqTmp := FFreqTmp*1000;
  Result := bladerf_set_frequency(FDevice, BLADERF_MODULE_RX, FFreqTmp);
end;

function TBladeThread.RxGetFreqSync(var AFreqActual: Integer): Integer;
var FFreqTmp : Cardinal;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_get_frequency(FDevice, BLADERF_MODULE_RX, FFreqTmp);
  AFreqActual := FFreqTmp div 1000;
end;

function TBladeThread.RxSetSrSync(ASrFreq: Cardinal; var ASrFreqActual: Cardinal): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_set_sample_rate(FDevice, BLADERF_MODULE_RX, ASrFreq, ASrFreqActual);
end;

function TBladeThread.RxGetSrSync(var ASrFreqActual: Cardinal): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_get_sample_rate(FDevice, BLADERF_MODULE_RX, ASrFreqActual);
end;

function TBladeThread.RxSetBwSync(ABandWidth: Cardinal; var ABandWidthActual: Cardinal): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_set_bandwidth(FDevice, BLADERF_MODULE_RX, ABandWidth, ABandWidthActual);
end;

function TBladeThread.RxGetBwSync(var ABandWidthActual: Cardinal): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_get_bandwidth(FDevice, BLADERF_MODULE_RX, ABandWidthActual);
end;


function TBladeThread.RxSetLnaSync(ALna: Byte): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  //Result := bladerf_set_lna_gain(FDevice, bladerf_lna_gain(ALna));
  Result := bladerf_set_lna_gain(FDevice, Integer(ALna));
end;

function TBladeThread.RxGetLnaSync(var ALnaActual: Cardinal): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  //Result := bladerf_get_bandwidth(FDevice, BLADERF_MODULE_RX, ABandWidthActual);
end;

function TBladeThread.RxSetVga1Sync(AVga1: Integer; var AVga1Actual: Integer): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_set_rxvga1(FDevice, AVga1);
  if Result = 0 then
    Result := RxGetVga1Sync(AVga1Actual);
end;

function TBladeThread.RxGetVga1Sync(var AVga1Actual: Integer): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_get_rxvga1(FDevice, AVga1Actual);
end;

function TBladeThread.RxSetVga2Sync(AVga2: Integer; var AVga2Actual: Integer): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_set_rxvga2(FDevice, AVga2);
  if Result = 0 then
    Result := RxGetVga2Sync(AVga2Actual);
end;

function TBladeThread.RxGetVga2Sync(var AVga2Actual: Integer): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_get_rxvga2(FDevice, AVga2Actual);
end;







///////////////////////////////////////////
////////                        ///////////
////////     TX                 ///////////
////////                        ///////////
///////////////////////////////////////////

function TBladeThread.TxSetFreqSync(AFreq: Integer): Integer;
var FFreqTmp : Cardinal;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  FFreqTmp := AFreq;
  FFreqTmp := FFreqTmp*1000;
  Result := bladerf_set_frequency(FDevice, BLADERF_MODULE_TX, FFreqTmp);
end;

function TBladeThread.TxGetFreqSync(var AFreqActual: Integer): Integer;
var FFreqTmp : Cardinal;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_get_frequency(FDevice, BLADERF_MODULE_TX, FFreqTmp);
  AFreqActual := FFreqTmp div 1000;
end;


function TBladeThread.TxSetSrSync(ASrFreq: Cardinal; var ASrFreqActual: Cardinal): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_set_sample_rate(FDevice, BLADERF_MODULE_TX, ASrFreq, ASrFreqActual);
end;

function TBladeThread.TxGetSrSync(var ASrFreqActual: Cardinal): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_get_sample_rate(FDevice, BLADERF_MODULE_TX, ASrFreqActual);
end;


function TBladeThread.TxSetBwSync(ABandWidth: Cardinal; var ABandWidthActual: Cardinal): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_set_bandwidth(FDevice, BLADERF_MODULE_TX, ABandWidth, ABandWidthActual);
end;

function TBladeThread.TxGetBwSync(var ABandWidthActual: Cardinal): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_get_bandwidth(FDevice, BLADERF_MODULE_TX, ABandWidthActual);
end;



function TBladeThread.TxSetVga1Sync(AVga1: Integer; var AVga1Actual: Integer): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_set_txvga1(FDevice, AVga1);
  if Result = 0 then
    Result := TxGetVga1Sync(AVga1Actual);
end;

function TBladeThread.TxGetVga1Sync(var AVga1Actual: Integer): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_get_txvga1(FDevice, AVga1Actual);
end;

function TBladeThread.TxSetVga2Sync(AVga2: Integer; var AVga2Actual: Integer): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_set_txvga2(FDevice, AVga2);
  if Result = 0 then
    Result := TxGetVga2Sync(AVga2Actual);
end;

function TBladeThread.TxGetVga2Sync(var AVga2Actual: Integer): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_get_txvga2(FDevice, AVga2Actual);
end;







function TBladeThread.RxInitSync: Integer;
var //* These items configure the underlying asynch stream used by the sync
    // * interface. The "buffer" here refers to those used internally by worker
    // * threads, not the user's sample buffers.
    // *
    // * It is important to remember that TX buffers will not be submitted to
    // * the hardware until `buffer_size` samples are provided via the
    // * bladerf_sync_tx call.  Similarly, samples will not be available to
    // * RX via bladerf_sync_rx() until a block of `buffer_size` samples has been
    // * received.
    // */
    num_buffers   : UINT;
    buffer_size   : UINT;  //* Must be a multiple of 1024 */
    num_transfers : UINT;
    timeout_ms    : UINT;

begin
  //Result := 0;
  //if FRxConfigured then Exit;
  num_buffers   := 32;     // 16
  buffer_size   := 32768;  // 4096
  num_transfers := 16;     // num_buffers / 2
  timeout_ms    := 3500;

  Result := bladerf_sync_config(FDevice,
                                BLADERF_MODULE_RX,
                                BLADERF_FORMAT_SC16_Q11,
                                num_buffers,
				buffer_size,
				num_transfers,
				timeout_ms);
  if Result = 0 then
    FRxConfigured := true;
end;



procedure TBladeThread.RxStart;
var Status : Integer;
    //rx_size : UINT;
begin
  if (not FConnected) OR (FDevice = nil) then Exit;
  if FRxActive then Exit;

  Status := RxInitSync;
  if Status <> 0 then
  begin
    SendMessage(hMainWnd, FromDev_RxStart, Status, 0);
    Exit;
  end;
  Status := bladerf_enable_module(FDevice, BLADERF_MODULE_RX, True);
  if Status <> 0 then
  begin
    SendMessage(hMainWnd, FromDev_RxStart, Status, 0);
    Exit;
  end;

  //FRxSamplesLen := 512*1000; //* May be any (reasonable) size */
  FRxSamplesLen := 48*1000; //* May be any (reasonable) size */
  FRxBufferSize := FRxSamplesLen*SizeOf(SmallInt)*2; //* May be any (reasonable) size */
  GetMem(FRxBuffer, FRxBufferSize);

  FRxActive := True;

  SendMessage(hMainWnd, FromDev_RxStart, Status, 0);
end;

{
function TBladeThread.RxStartSync: Integer;
begin
  Result := -1;
  if (not FConnected) OR (FDevice = nil) then Exit;
  Result := 0;
  if FRxActive then Exit;

  Result := RxInitSync;
  if Result <> 0 then Exit;

  if FRxBuffer = nil then begin
    FRxSamplesLen := 512*1000; //* May be any (reasonable) size */
    FRxBufferSize := FRxSamplesLen*SizeOf(SmallInt)*2; //* May be any (reasonable) size */
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

procedure TBladeThread.RxStop;
var Status : Integer;
begin
  if not FRxActive then Exit;
  Status := bladerf_enable_module(FDevice, BLADERF_MODULE_RX, False);
  if Status <> 0 then
  begin
    SendMessage(hMainWnd, FromDev_RxStop, Status, 0);
    Exit;
  end;

  FRxActive := False;
  //FreeMemory(FRxBuffer);
  //FRxBuffer := nil;

  SendMessage(hMainWnd, FromDev_RxStop, 0, 0);
end;

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




function TBladeThread.RxProcess: Boolean;
var Status : Integer;
    FBufferTmp : PSmallInt;
begin
  Result := False;
  if (not FConnected) OR (FDevice = nil) then Exit;
  if not FRxActive then Exit;

  Status := bladerf_sync_rx(FDevice, FRxBuffer, FRxSamplesLen, nil, 5000);
  if Status <> 0 then
  begin
    Disconnect;
    SendMessage(hMainWnd, FromDev_RxProcessError, Status, 0);
    Exit;
  end;
  GetMem(FBufferTmp, FRxBufferSize);
  CopyMemory(FBufferTmp, FRxBuffer, FRxBufferSize);
  SendMessage(hMainWnd, FromDev_RxProcessData, Integer(FBufferTmp), FRxBufferSize);
end;











function TBladeThread.TxInitSync: Integer;
var //* These items configure the underlying asynch stream used by the sync
    // * interface. The "buffer" here refers to those used internally by worker
    // * threads, not the user's sample buffers.
    // *
    // * It is important to remember that TX buffers will not be submitted to
    // * the hardware until `buffer_size` samples are provided via the
    // * bladerf_sync_tx call.  Similarly, samples will not be available to
    // * RX via bladerf_sync_rx() until a block of `buffer_size` samples has been
    // * received.
    // */
    num_buffers   : UINT;
    buffer_size   : UINT;  //* Must be a multiple of 1024 */
    num_transfers : UINT;
    timeout_ms    : UINT;

begin
  //Result := 0;
  //if FTxConfigured then Exit;
  num_buffers   := 4;     // 16
  buffer_size   := 8192;  // 4096
  num_transfers := 2;     // num_buffers / 2
  timeout_ms    := 100;

  Result := bladerf_sync_config(FDevice,
                                BLADERF_MODULE_TX,
                                BLADERF_FORMAT_SC16_Q11,
                                num_buffers,
				buffer_size,
				num_transfers,
				timeout_ms);
  if Result = 0 then
    FTxConfigured := true;
end;



procedure TBladeThread.TxStart;
var Status : Integer;
    //rx_size : UINT;
begin
  if (not FConnected) OR (FDevice = nil) then Exit;
  if FTxActive then Exit;

  Status := TxInitSync;
  if Status <> 0 then
  begin
    SendMessage(hMainWnd, FromDev_TxStart, Status, 0);
    Exit;
  end;
  Status := bladerf_enable_module(FDevice, BLADERF_MODULE_TX, True);
  if Status <> 0 then
  begin
    SendMessage(hMainWnd, FromDev_TxStart, Status, 0);
    Exit;
  end;

  FTxSamplesLen := 512*1000; //* May be any (reasonable) size */
  FTxBufferSize := FTxSamplesLen*SizeOf(SmallInt)*2; //* May be any (reasonable) size */
  GetMem(FTxBuffer, FTxBufferSize);

  FTxActive := True;

  SendMessage(hMainWnd, FromDev_TxStart, Status, 0);
end;

{
function TBladeThread.TxStartSync: Integer;
begin
  Result := -1;
  if (not FConnected) OR (FDevice = nil) then Exit;
  Result := 0;
  if FTxActive then Exit;

  Result := TxInitSync;
  if Result <> 0 then Exit;

  //if FTxBuffer = nil then begin
    FTxSamplesLen := 512*1000; //* May be any (reasonable) size */
    FTxBufferSize := FTxSamplesLen*SizeOf(SmallInt)*2; //* May be any (reasonable) size */
  //  GetMem(FTxBuffer, FTxBufferSize);
  //end;

  Result := bladerf_enable_module(FDevice, BLADERF_MODULE_TX, True);
  if Result <> 0 then
    Exit;

  FTxActive := True;
end;
}

procedure TBladeThread.TxStop;
var Status : Integer;
begin
  if not FTxActive then Exit;
  FTxActive := False;
  Status := bladerf_enable_module(FDevice, BLADERF_MODULE_TX, False);
  if Status <> 0 then
  begin
    SendMessage(hMainWnd, FromDev_TxStop, Status, 0);
    Exit;
  end;


  SendMessage(hMainWnd, FromDev_TxStop, 0, 0);
end;

{
function TBladeThread.TxStopSync: Integer;
begin
  Result := -1;
  if (not FConnected) OR (FDevice = nil) then Exit;
  if not FTxActive then Exit;

  Lock;
  FTxActive := False;
  Result := bladerf_enable_module(FDevice, BLADERF_MODULE_TX, False);
  Unlock;
end;
}


function TBladeThread.CanSend: Boolean;
begin
  Result := False;
  if (not FConnected) OR (FDevice = nil) then Exit;
  if not FTxActive then Exit;

  try
    Lock;
    Result := FTxDataList.Count < 2;
  finally
    Unlock;
  end;
end;

function TBladeThread.TxAddData(AData: Pointer; ADataSize: Integer): Boolean;
var FDataItem : PTxDataItem;
begin
  Result := False;
  GetMem(FDataItem, SizeOf(TTxDataItem));
  FDataItem^.Data := AData;
  FDataItem^.DataSize := ADataSize;
  try
    Lock;
    FTxDataList.Add(FDataItem);
    Result := True;
  finally
    Unlock;
  end;
end;

procedure TBladeThread.TxProcess;
var Status : Integer;
    FDataItem : PTxDataItem;
    //FDataSended : Integer;
begin
  if (not FConnected) OR (FDevice = nil) then Exit;
  if not FTxActive then Exit;

  FDataItem := nil;
  Exit;

  //FDataSended := 0;
  //while True do begin
    try
      Lock;
      if FTxDataList.Count = 0 then begin
        Unlock;
        //Break;
        Exit;
      end;
      FDataItem := FTxDataList.Items[0];
      FTxDataList.Delete(0);
    finally
      Unlock;
    end;

    if FDataItem = nil then Exit;

    //Status := bladerf_sync_tx(FDevice, FDataItem^.Data, FDataItem^.DataSize div 4, nil, 5000);
    Status := bladerf_sync_tx(FDevice, FTxBuffer, FTxSamplesLen, nil, 5000);
    if Status <> 0 then
    begin
      Disconnect;
      SendMessage(hMainWnd, FromDev_TxProcessError, Status, 0);
      //Break;
      Exit;
    end;
    //FreeMem(FDataItem^.Data);
    //FreeMem(FDataItem);

    //inc(FDataSended, FDataItem^.DataSize);
    //if FDataSended >= FTxBufferSize then Break;
  //end;
end;








procedure TBladeThread.Execute;
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
end;

end.
