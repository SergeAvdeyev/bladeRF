unit BladeRFThread;

interface

uses Windows, Sysutils, Messages, syncobjs,
  Classes, BladeRFIntf;

const
  ToDevMsg_Connect     = WM_USER + 1;
  FromDev_Connect      = WM_USER + 1; // result

  ToDevMsg_Disconnect  = WM_USER + 2;
  FromDev_Disconnect   = WM_USER + 2; // disconnected from device event


  ToDevMsg_RxSetFreq   = WM_USER + 3; // Set RX Frequency
  FromDev_RxSetFreq    = WM_USER + 3; // Set RX Frequency result
  ToDevMsg_RxGetFreq   = WM_USER + 4; // Get RX Frequency
  FromDev_RxGetFreq    = WM_USER + 4; // Get RX Frequency result

  ToDevMsg_TxSetFreq   = WM_USER + 5; // Set TX Frequency
  FromDev_TxSetFreq    = WM_USER + 5; // Set TX Frequency result
  ToDevMsg_TxGetFreq   = WM_USER + 6; // Get TX Frequency
  FromDev_TxGetFreq    = WM_USER + 6; // Get TX Frequency result

  ToDevMsg_RxSetSR     = WM_USER + 7; // Set RX Sample Rate
  FromDev_RxSetSR      = WM_USER + 7; // Set RX Sample Rate result
  ToDevMsg_RxGetSR     = WM_USER + 8; // Get RX Sample Rate
  FromDev_RxGetSR      = WM_USER + 8; // Get RX Sample Rate result

  ToDevMsg_TxSetSR     = WM_USER + 9; // Set TX Sample Rate
  FromDev_TxSetSR      = WM_USER + 9; // Set TX Sample Rate result
  ToDevMsg_TxGetSR     = WM_USER + 10; // Get TX Sample Rate
  FromDev_TxGetSR      = WM_USER + 10; // Get TX Sample Rate result

  ToDevMsg_RxSetBW     = WM_USER + 11; // Set RX BandWidth
  FromDev_RxSetBW      = WM_USER + 11; // Set RX BandWidth result
  ToDevMsg_RxGetBW     = WM_USER + 12; // Get RX BandWidth
  FromDev_RxGetBW      = WM_USER + 12; // Get RX BandWidth result

  ToDevMsg_TxSetBW     = WM_USER + 13; // Set TX BandWidth
  FromDev_TxSetBW      = WM_USER + 13; // Set TX BandWidth result
  ToDevMsg_TxGetBW     = WM_USER + 14; // Get TX BandWidth
  FromDev_TxGetBW      = WM_USER + 14; // Get TX BandWidth result

  ToDevMsg_RxSetLnaGain   = WM_USER + 15; // Set RX LNA Gain
  FromDev_RxSetLnaGain    = WM_USER + 15; // Set RX LNA Gain result
  ToDevMsg_RxGetLnaGain   = WM_USER + 16; // Get RX LNA Gain
  FromDev_RxGetLnaGain    = WM_USER + 16; // Get RX LNA Gain result

  ToDevMsg_RxSetVga1Gain  = WM_USER + 17; // Set RX Vga1 Gain
  FromDev_RxSetVga1Gain   = WM_USER + 17; // Set RX Vga1 Gain result
  ToDevMsg_RxGetVga1Gain  = WM_USER + 18; // Get RX Vga1 Gain
  FromDev_RxGetVga1Gain   = WM_USER + 18; // Get RX Vga1 Gain result

  ToDevMsg_RxSetVga2Gain  = WM_USER + 19; // Set RX Vga2 Gain
  FromDev_RxSetVga2Gain   = WM_USER + 19; // Set RX Vga2 Gain result
  ToDevMsg_RxGetVga2Gain  = WM_USER + 20; // Get RX Vga2 Gain
  FromDev_RxGetVga2Gain   = WM_USER + 20; // Get RX Vga2 Gain result

  ToDevMsg_TxVga1Gain  = WM_USER + 21; // Set TX Vga1 Gain
  ToDevMsg_TxVga2Gain  = WM_USER + 22; // Set TX Vga2 Gain


  ToDevMsg_RxStart     = WM_USER + 100; // Start receive
  FromDev_RxStart      = WM_USER + 101; // Start receive result

  ToDevMsg_RxStop      = WM_USER + 102; // Stop receive
  FromDev_RxStop       = WM_USER + 103; // Stop receive result

  FromDev_RxProcessError = WM_USER + 110; // Result of receiving
  FromDev_RxProcessData  = WM_USER + 111; // Result of receiving

  //FromDevMsg_Connected    = WM_USER + 1;
  //FromDevMsg_Disconnected = WM_USER + 2;

type
  TBladeThread = class(TThread)
  private
    hMainWnd         : THandle;
    FCriticalSection : TCriticalSection;

    FDevice : Pointer;
    FConnected : Boolean;
    FRxActive  : Boolean;

    FRxBuffer     : PSmallInt;
    FRxBufferSize : UINT;
    FSamplesLen : UINT;

    procedure Lock;
    procedure Unlock;

    procedure Connect;
    procedure Disconnect;
    procedure RxSetFreq(msg: tagMSG);
    procedure TxSetFreq(msg: tagMSG);
    procedure RxGetFreq;
    procedure TxGetFreq;
    procedure RxSetSR(msg: tagMSG);
    procedure RxGetSR;
    procedure TxSetSR(msg: tagMSG);
    procedure RxSetBW(msg: tagMSG);
    procedure RxGetBW;
    procedure TxSetBW(msg: tagMSG);

    procedure RxStart;
    procedure RxStop;

    function InitSync: Integer;
    function RxProcess: Boolean;

  protected
    procedure Execute; override;

  public
    constructor Create(HWND: THandle);
    destructor Destroy; override;

    function RxSetFreqSync(AFreq: Integer): Integer;
    function RxSetSrSync(ASrFreq: Cardinal; var ASrFreqActual: Cardinal): Integer;
    function RxSetBwSync(ABandWidth: Cardinal; var ABandWidthActual: Cardinal): Integer;
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
  inherited Create(True);
end;

destructor TBladeThread.Destroy;
begin
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
  SendMessage(hMainWnd, FromDev_Connect, Status, 0);
  FConnected := Status = 0;
end;

procedure TBladeThread.Disconnect;
begin
  if (not FConnected) OR (FDevice = nil) then Exit;

  bladerf_close(FDevice);
  FDevice := nil;
  FConnected := False;
  FRxActive := False;
  SendMessage(hMainWnd, FromDev_Disconnect, 0, 0);
end;

procedure TBladeThread.RxSetFreq(msg: tagMSG);
var Status : Integer;
    FFreqTmp : Cardinal;
begin
  if (not FConnected) OR (FDevice = nil) then Exit;

  FFreqTmp := msg.wParam;
  FFreqTmp := FFreqTmp*1000;
  Status := bladerf_set_frequency(FDevice, BLADERF_MODULE_RX, FFreqTmp);
  SendMessage(hMainWnd, FromDev_RxSetFreq, Status, 0);
end;

function TBladeThread.RxSetFreqSync(AFreq: Integer): Integer;
var FFreqTmp : Cardinal;
begin
  if (not FConnected) OR (FDevice = nil) then Exit;

  FFreqTmp := AFreq;
  FFreqTmp := FFreqTmp*1000;
  Result := bladerf_set_frequency(FDevice, BLADERF_MODULE_RX, FFreqTmp);
  //SendMessage(hMainWnd, FromDev_RxSetFreq, Status, 0);
end;

procedure TBladeThread.TxSetFreq(msg: tagMSG);
var Status : Integer;
    FFreqTmp : Cardinal;
begin
  if (not FConnected) OR (FDevice = nil) then Exit;

  FFreqTmp := msg.wParam;
  FFreqTmp := FFreqTmp*1000;
  Status := bladerf_set_frequency(FDevice, BLADERF_MODULE_TX, FFreqTmp);
  SendMessage(hMainWnd, FromDev_TxSetFreq, Status, 0);
end;

procedure TBladeThread.RxGetFreq;
var Status : Integer;
    FFreqTmp : Cardinal;
begin
  if (not FConnected) OR (FDevice = nil) then Exit;

  Status := bladerf_get_frequency(FDevice, BLADERF_MODULE_RX, FFreqTmp);
  SendMessage(hMainWnd, FromDev_RxGetFreq, Status, FFreqTmp div 1000);
end;

procedure TBladeThread.TxGetFreq;
var Status : Integer;
    FFreqTmp : Cardinal;
begin
  if (not FConnected) OR (FDevice = nil) then Exit;

  Status := bladerf_get_frequency(FDevice, BLADERF_MODULE_TX, FFreqTmp);
  SendMessage(hMainWnd, FromDev_TxGetFreq, Status, FFreqTmp div 1000);
end;



procedure TBladeThread.RxSetSR(msg: tagMSG);
var Status : Integer;
    FSrTmp : Cardinal;
begin
  if (not FConnected) OR (FDevice = nil) then Exit;

  Status := bladerf_set_sample_rate(FDevice, BLADERF_MODULE_RX, Cardinal(msg.wParam), FSrTmp);
  SendMessage(hMainWnd, FromDev_RxSetSR, Status, Integer(FSrTmp));
end;

procedure TBladeThread.RxGetSR;
var Status : Integer;
    FSrTmp : Cardinal;
begin
  if (not FConnected) OR (FDevice = nil) then Exit;

  Status := bladerf_get_sample_rate(FDevice, BLADERF_MODULE_RX, FSrTmp);
  SendMessage(hMainWnd, FromDev_RxGetSR, Status, Integer(FSrTmp));
end;

function TBladeThread.RxSetSrSync(ASrFreq: Cardinal; var ASrFreqActual: Cardinal): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_set_sample_rate(FDevice, BLADERF_MODULE_RX, ASrFreq, ASrFreqActual);
  //SendMessage(hMainWnd, FromDev_RxSetSR, Status, Integer(FSrTmp));
end;

procedure TBladeThread.TxSetSR(msg: tagMSG);
var Status : Integer;
    FSrTmp : Cardinal;
begin
  if (not FConnected) OR (FDevice = nil) then Exit;

  Status := bladerf_set_sample_rate(FDevice, BLADERF_MODULE_TX, Cardinal(msg.wParam), FSrTmp);
  SendMessage(hMainWnd, FromDev_TxSetSR, Status, Integer(FSrTmp));
end;

procedure TBladeThread.RxSetBW(msg: tagMSG);
var Status : Integer;
    FBWTmp : Cardinal;
begin
  if (not FConnected) OR (FDevice = nil) then Exit;

  Status := bladerf_set_bandwidth(FDevice, BLADERF_MODULE_RX, Cardinal(msg.wParam), FBWtmp);
  SendMessage(hMainWnd, FromDev_RxSetBW, Status, Integer(FBWTmp));
end;

procedure TBladeThread.RxGetBW;
var Status : Integer;
    FBWTmp : Cardinal;
begin
  if (not FConnected) OR (FDevice = nil) then Exit;

  Status := bladerf_get_bandwidth(FDevice, BLADERF_MODULE_RX, FBWtmp);
  SendMessage(hMainWnd, FromDev_RxGetBW, Status, Integer(FBWTmp));
end;

function TBladeThread.RxSetBwSync(ABandWidth: Cardinal; var ABandWidthActual: Cardinal): Integer;
begin
  Result := 0;
  if (not FConnected) OR (FDevice = nil) then Exit;

  Result := bladerf_set_bandwidth(FDevice, BLADERF_MODULE_RX, ABandWidth, ABandWidthActual);
end;

procedure TBladeThread.TxSetBW(msg: tagMSG);
var Status : Integer;
    FBWTmp : Cardinal;
begin
  if (not FConnected) OR (FDevice = nil) then Exit;

  Status := bladerf_set_bandwidth(FDevice, BLADERF_MODULE_TX, Cardinal(msg.wParam), FBWtmp);
  SendMessage(hMainWnd, FromDev_TxSetBW, Status, Integer(FBWTmp));
end;


function TBladeThread.InitSync: Integer;
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
  num_buffers   := 32;
  buffer_size   := 32768;  
  num_transfers := 16;
  timeout_ms    := 3500;

  Result := bladerf_sync_config(FDevice,
                                BLADERF_MODULE_RX,
                                BLADERF_FORMAT_SC16_Q11,
                                num_buffers,
				buffer_size,
				num_transfers,
				timeout_ms);
end;



procedure TBladeThread.RxStart;
var Status : Integer;
    //rx_size : UINT;
begin
  if (not FConnected) OR (FDevice = nil) then Exit;
  if FRxActive then Exit;

  Status := InitSync;
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

  FSamplesLen := 512*1000; //* May be any (reasonable) size */
  FRxBufferSize := FSamplesLen*SizeOf(SmallInt)*2; //* May be any (reasonable) size */
  GetMem(FRxBuffer, FRxBufferSize);

  FRxActive := True;

  SendMessage(hMainWnd, FromDev_RxStart, Status, 0);
end;

procedure TBladeThread.RxStop;
var Status : Integer;
begin
  Status := bladerf_enable_module(FDevice, BLADERF_MODULE_RX, False);
  if Status <> 0 then
  begin
    SendMessage(hMainWnd, FromDev_RxStop, Status, 0);
    Exit;
  end;

  FRxActive := False;
  FreeMemory(FRxBuffer);
  FRxBuffer := nil;

  SendMessage(hMainWnd, FromDev_RxStop, 0, 0);
end;



function TBladeThread.RxProcess: Boolean;
var Status : Integer;
    FBufferTmp : PSmallInt;
begin
  Result := False;
  if (not FConnected) OR (FDevice = nil) then Exit;
  if not FRxActive then Exit;

  Status := bladerf_sync_rx(FDevice, FRxBuffer, FSamplesLen, nil, 5000);
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
         
         ToDevMsg_RxSetFreq  : RxSetFreq(msg);
         ToDevMsg_RxGetFreq  : RxGetFreq;

         ToDevMsg_TxSetFreq  : TxSetFreq(msg);
         ToDevMsg_TxGetFreq  : TxGetFreq;

         ToDevMsg_RxSetSR    : RxSetSR(msg);
         ToDevMsg_RxGetSR    : RxGetSR;

         ToDevMsg_TxSetSR    : TxSetSR(msg);

         ToDevMsg_RxSetBW    : RxSetBW(msg);
         ToDevMsg_RxGetBW    : RxGetBW;

         ToDevMsg_TxSetBW    : TxSetBW(msg);

         ToDevMsg_RxStart    : RxStart;
         ToDevMsg_RxStop     : RxStop;
       end;
     end
     else
     begin
       if not FRxActive then
       begin
         Suspend;
         Continue;
       end;
     end;

     if FRxActive then
     begin
        RxProcess;
        //Continue;
     end;
     //Sleep(2);
  end;
  if FConnected then
    Disconnect;
end;

end.
