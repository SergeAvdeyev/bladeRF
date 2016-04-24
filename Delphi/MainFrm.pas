unit MainFrm;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, OscCEP, StdCtrls, RXCtrls, BladeRFIntf, BladeRFThread;

type
  TFreqLabelDescr = record
    LabelValue : Byte;
    Digit      : DWord;
    Module     : DWord;
    Multiplier : DWord;
  end;
  PFreqLabelDescr = ^TFreqLabelDescr;


  TForm1 = class(TForm)
    Panel1: TPanel;
    Timer: TTimer;
    Button1: TButton;
    Label1: TLabel;
    OpenDevBtn: TButton;
    Memo: TMemo;
    SetRxFreqBtn: TButton;
    GetRxFreqBtn: TButton;
    SetRxSRBtn: TButton;
    SetRxBWBtn: TButton;
    StartRxBtn: TButton;
    StopRxBtn: TButton;
    GroupBox1: TGroupBox;
    RxLabel_MHz_100: TRxLabel;
    RxLabel_MHz_10: TRxLabel;
    RxLabel_GHz_1: TRxLabel;
    RxLabel_MHz_1: TRxLabel;
    RxLabel_kHz_100: TRxLabel;
    RxLabel_kHz_10: TRxLabel;
    RxLabel_kHz_1: TRxLabel;
    procedure FormCreate(Sender: TObject);
    procedure TimerTimer(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Button1Click(Sender: TObject);
    procedure OpenDevBtnClick(Sender: TObject);
    procedure SetRxFreqBtnClick(Sender: TObject);
    procedure GetRxFreqBtnClick(Sender: TObject);
    procedure SetRxSRBtnClick(Sender: TObject);
    procedure SetRxBWBtnClick(Sender: TObject);
    procedure StartRxBtnClick(Sender: TObject);
    procedure StopRxBtnClick(Sender: TObject);
    procedure RxLabel_GHz_1MouseEnter(Sender: TObject);
    procedure RxLabel_GHz_1MouseLeave(Sender: TObject);
    procedure FormActivate(Sender: TObject);
  private
    { Private declarations }
    OscR    : TOsc;
    pBuffer : PChar;
    BufferSize : Integer;
    hFile   : Integer;

    FCounter : Integer;

    FDevice : Pointer;

    FRxFreq : UINT;
    FRxSR   : UINT;
    FRxBW   : UINT;

    FRxActive : Boolean;

    FTxFreq : UINT;

    FRxLabelPtr : TRxLabel;
    FRxLabelsList : TList;
    FRxLabelsDescr : TList;

    FDeviceThread : TBladeThread;

    procedure SetRxFreqLabels;
    procedure UpdateRxFreqLabels;
    function init_sync(ADevice: Pointer): Integer;

    procedure OnConnected(var Msg: TMessage); message FromDevMsg_Connected;
    procedure OnDisconnected(var Msg: TMessage); message FromDevMsg_Disconnected;
    procedure OnDeviceError(var Msg: TMessage); message WM_USER + 3;

    procedure OnMouseWheel(var Message: TMessage); message WM_MOUSEWHEEL;
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

procedure TForm1.FormCreate(Sender: TObject);
begin
  OscR := TOsc.Create(Panel1);
  OscR.Align := alClient;
  //OscR.Freq := 4000000;
  OscR.Freq := 40000;
  OscR.CountSamples := 16384;
  OscR.RealTime := False;
  OscR.EnableSpectr := True;
  OscR.LineColor := clAqua;
  OscR.CountFFT := 2048;
  OscR.Hint := 'Для увеличения изображения по вертикали' + #10#13 +
               'используйте клавишу Alt и левую кнопку мыши' + #10#13 +
               'Для увеличения изображения по горизонтали' + #10#13 +
               'используйте клавишу Ctrl и левую кнопку мыши';
  OscR.ShowHint := True;
  OscR.MaxSpectrValue := 10;
  OscR.MinSpectrValue := -130;
  OscR.Visible := True;
  //hFile := FileOpen('d:\11025.wav', fmOpenReadWrite or fmShareDenyNone);
  hFile := FileOpen('d:\bladerf.dat', fmOpenReadWrite or fmShareDenyNone);

  BufferSize := 4096;
  GetMem(pBuffer, BufferSize);

  FDevice := nil;
  FRxFreq := 434925;    // In kHz
  FRxSR := 4000000;
  FRxBW := 1500000;

  FTxFreq := 435925;   // In kHz

  FDeviceThread := TBladeThread.Create(Handle);
  FDeviceThread.Resume;
end;

procedure TForm1.FormActivate(Sender: TObject);
begin
  SetRxFreqLabels;
  UpdateRxFreqLabels;
end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
var FFreqLabelDescr : PFreqLabelDescr;
begin
  if hFile > 0 then
    FileClose(hFile);
  FreeMem(pBuffer);

  FRxLabelsList.Free;

  while FRxLabelsDescr.Count > 0 do
  begin
    FFreqLabelDescr := FRxLabelsDescr.Items[0];
    FreeMem(FFreqLabelDescr);
    FRxLabelsDescr.Delete(0);
  end;
  FRxLabelsDescr.Free;
end;






procedure TForm1.SetRxFreqLabels;
var FFreqLabelDescr : PFreqLabelDescr;
    i : Integer;
begin
  FRxLabelPtr := nil;
  FRxLabelsList := TList.Create;
  FRxLabelsList.Add(RxLabel_kHz_1);
  FRxLabelsList.Add(RxLabel_kHz_10);
  FRxLabelsList.Add(RxLabel_kHz_100);
  FRxLabelsList.Add(RxLabel_MHz_1);
  FRxLabelsList.Add(RxLabel_MHz_10);
  FRxLabelsList.Add(RxLabel_MHz_100);
  FRxLabelsList.Add(RxLabel_GHz_1);

  i := 0;
  FRxLabelsDescr := TList.Create;
  while i < 7 do begin
    GetMem(FFreqLabelDescr, SizeOf(TFreqLabelDescr));
    FFreqLabelDescr^.Digit := i;
    if i = 0 then begin
      FFreqLabelDescr^.Module := 10;
      FFreqLabelDescr^.Multiplier := 1;
      FRxLabelsDescr.Add(FFreqLabelDescr);
    end
    else if i = 1 then begin
      FFreqLabelDescr^.Module := 100;
      FFreqLabelDescr^.Multiplier := 10;
      FRxLabelsDescr.Add(FFreqLabelDescr);
    end
    else if i = 2 then begin
      FFreqLabelDescr^.Module := 1000;
      FFreqLabelDescr^.Multiplier := 100;
      FRxLabelsDescr.Add(FFreqLabelDescr);
    end
    else if i = 3 then begin
      FFreqLabelDescr^.Module := 10000;
      FFreqLabelDescr^.Multiplier := 1000;
      FRxLabelsDescr.Add(FFreqLabelDescr);
    end
    else if i = 4 then begin
      FFreqLabelDescr^.Module := 100000;
      FFreqLabelDescr^.Multiplier := 10000;
      FRxLabelsDescr.Add(FFreqLabelDescr);
    end
    else if i = 5 then begin
      FFreqLabelDescr^.Module := 1000000;
      FFreqLabelDescr^.Multiplier := 100000;
      FRxLabelsDescr.Add(FFreqLabelDescr);
    end
    else if i = 6 then begin
      FFreqLabelDescr^.Module := 10000000;
      FFreqLabelDescr^.Multiplier := 1000000;
      FRxLabelsDescr.Add(FFreqLabelDescr);
    end;
    inc(i);
  end;
end;

procedure TForm1.UpdateRxFreqLabels;
var F : PFreqLabelDescr;
    i : Integer;
    L : TRxLabel;
begin
  for i := 0 to FRxLabelsList.Count - 1 do
  begin
    L := FRxLabelsList.Items[i];
    F := FRxLabelsDescr.Items[i];
    F^.LabelValue := (FRxFreq mod F^.Module) div F^.Multiplier;
    L.Caption := IntToStr(F^.LabelValue);
  end;
end;





procedure TForm1.TimerTimer(Sender: TObject);
var nread : Integer;
begin
  if hFile <= 0 then
    Exit;

  nRead := FileRead(hFile, pBuffer^, BufferSize);
  if nRead <= 0 then
  begin
    Timer.Enabled := False;
    Exit;
  end;

  if OscR.DrawSpectr then
    OscR.AddSignal(pBuffer^, nRead div 2)
  else
    OscR.AddSignal(pBuffer^, nRead div 2);

  inc(FCounter, 2);
  Label1.Caption := IntToStr(FCounter);
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
  if hFile > 0 then
    FileSeek(hFile, 46, 0);
  Timer.Enabled := true;
  FCounter := 0;
end;

procedure TForm1.OpenDevBtnClick(Sender: TObject);
//var status : Integer;
    //S : String;
    //i : Integer;
begin
  if FDeviceThread = nil then Exit;

  if OpenDevBtn.Tag = 0 then
  begin
    if FDeviceThread = nil then Exit;
    Memo.Lines.Add('Connecting to device... ');
    PostThreadMessage(FDeviceThread.ThreadID, ToDevMsg_Connect, 0, 0);
    OpenDevBtn.Enabled := False;
  end
  else
  begin
    Memo.Lines.Add('Disconnecting ... ');
    PostThreadMessage(FDeviceThread.ThreadID, ToDevMsg_Disconnect, 0, 0);
    OpenDevBtn.Enabled := False;

  end;
{
  if OpenDevBtn.Tag = 0 then
  begin
    Memo.Lines.Add('Connecting to device... ');
    status := bladerf_open(FDevice, nil);
    if status = 0 then
    begin
      i := Memo.Lines.Count - 1;
      S := Memo.Lines.Strings[i] + 'OK';
      Memo.Lines.Strings[i] := S;
      OpenDevBtn.Caption := 'Close';
      OpenDevBtn.Tag := 1;
      SetRxFreqBtn.Enabled := true;
      SetRxSRBtn.Enabled := true;
      SetRxBWBtn.Enabled := true;
      GetRxFreqBtn.Enabled := true;
      SetRxBWBtn.Enabled := true;
      StartRxBtn.Enabled := true;
    end
    else
    begin
      S := StrPas(bladerf_strerror(status));
      Memo.Lines.Add('Failed: ' + S);
    end;
  end
  else
  begin
    Memo.Lines.Add('Disconnecting ... ');
    bladerf_close(FDevice);
    i := Memo.Lines.Count - 1;
    S := Memo.Lines.Strings[i] + 'OK';
    Memo.Lines.Strings[i] := S;
    OpenDevBtn.Caption := 'Open';
    OpenDevBtn.Tag := 0;
    FDevice := nil;
    SetRxFreqBtn.Enabled := false;
    SetRxSRBtn.Enabled := false;
    SetRxBWBtn.Enabled := false;
    GetRxFreqBtn.Enabled := false;
    SetRxBWBtn.Enabled := false;
    StartRxBtn.Enabled := false;
  end;
  }
end;

procedure TForm1.OnConnected(var Msg: TMessage);
begin
  Memo.Lines.Add('Connected');
  OpenDevBtn.Enabled := True;
  OpenDevBtn.Caption := 'Disconnect';
  OpenDevBtn.Tag := 1;
  SetRxFreqBtn.Enabled := true;
  SetRxSRBtn.Enabled := true;
  SetRxBWBtn.Enabled := true;
  GetRxFreqBtn.Enabled := true;
  SetRxBWBtn.Enabled := true;
  StartRxBtn.Enabled := true;
end;

procedure TForm1.OnDisconnected(var Msg: TMessage);
begin
  Memo.Lines.Add('Disconnected');
  OpenDevBtn.Enabled := True;
  OpenDevBtn.Caption := 'Connect';
  OpenDevBtn.Tag := 0;
  SetRxFreqBtn.Enabled := False;
  SetRxSRBtn.Enabled := False;
  SetRxBWBtn.Enabled := False;
  GetRxFreqBtn.Enabled := False;
  SetRxBWBtn.Enabled := False;
  StartRxBtn.Enabled := False;
end;

procedure TForm1.OnDeviceError(var Msg: TMessage);
var S : String;
    P : PChar;
begin
  P := PChar(Msg.WParam);
  S := StrPas(P);
  Memo.Lines.Add('Failed: ' + S);
  FreeMem(P);
end;

procedure TForm1.SetRxFreqBtnClick(Sender: TObject);
var status : Integer;
    i : Integer;
    S : String;
begin
  if FDevice = nil then Exit;
  Memo.Lines.Add('Tuning RX to ' + IntToStr(FRxFreq*1000) + ' Hz... ');
  status := bladerf_set_frequency(FDevice, BLADERF_MODULE_RX, FRxFreq*1000);

  if status <> 0 then
  begin
    S := StrPas(bladerf_strerror(status));
    Memo.Lines.Add('Failed: ' + S);
    Exit;
  end;

  i := Memo.Lines.Count - 1;
  S := Memo.Lines.Strings[i] + 'OK';
  Memo.Lines.Strings[i] := S;
end;

procedure TForm1.GetRxFreqBtnClick(Sender: TObject);
var status : Integer;
    i : Integer;
    tmp: UINT;
    S : String;
begin
  if FDevice = nil then Exit;
  Memo.Lines.Add('Read RX frequency... ');
  status := bladerf_get_frequency(FDevice, BLADERF_MODULE_RX, tmp);

  if status <> 0 then
  begin
    S := StrPas(bladerf_strerror(status));
    Memo.Lines.Add('Failed: ' + S);
    Exit;
  end;

  i := Memo.Lines.Count - 1;
  S := Memo.Lines.Strings[i] + 'OK';
  Memo.Lines.Strings[i] := S;
  Memo.Lines.Add('Rx Frequency is ' + IntToStr(tmp));
end;



procedure TForm1.SetRxSRBtnClick(Sender: TObject);
var status : Integer;
    i : Integer;
    S : String;
    RxSRtmp: UINT;
begin
  if FDevice = nil then Exit;
  Memo.Lines.Add('Setting RX sampling rate to ' + IntToStr(FRxSR) + ' Hz... ');
  status := bladerf_set_sample_rate(FDevice, BLADERF_MODULE_RX, FRxSR, RxSRtmp);

  if status <> 0 then
  begin
    S := StrPas(bladerf_strerror(status));
    Memo.Lines.Add('Failed: ' + S);
    Exit;
  end;

  i := Memo.Lines.Count - 1;
  S := Memo.Lines.Strings[i] + 'OK';
  Memo.Lines.Strings[i] := S;
  Memo.Lines.Add('Actual RX sampling rate is ' + IntToStr(RxSRtmp) + ' Hz... ');
end;


procedure TForm1.SetRxBWBtnClick(Sender: TObject);
var status : Integer;
    i : Integer;
    S : String;
    RxBWtmp: UINT;
begin
  if FDevice = nil then Exit;
  Memo.Lines.Add('Setting RX bandwidth to ' + IntToStr(FRxBW) + ' Hz... ');
  status := bladerf_set_bandwidth(FDevice, BLADERF_MODULE_RX, FRxBW, RxBWtmp);

  if status <> 0 then
  begin
    S := StrPas(bladerf_strerror(status));
    Memo.Lines.Add('Failed: ' + S);
    Exit;
  end;

  i := Memo.Lines.Count - 1;
  S := Memo.Lines.Strings[i] + 'OK';
  Memo.Lines.Strings[i] := S;
  Memo.Lines.Add('Actual RX bandwidth is ' + IntToStr(RxBWtmp) + ' Hz... ');
end;

procedure TForm1.StartRxBtnClick(Sender: TObject);
var status : Integer;
    S : String;
    i : Integer;
    n : DWord;

    Buffer : PSmallInt;
    BufferPtr : PSmallInt;
    samples_len : UINT;
    rx_size : UINT;
    BufferTmp : PSmallInt;
    BufferTmpPtr : PSmallInt;
    val : Smallint;
begin
  if FDevice = nil then Exit;
  Memo.Lines.Add('Starting RX ... ');

  status := init_sync(FDevice);
  if status <> 0 then
  begin
    S := StrPas(bladerf_strerror(status));
    Memo.Lines.Add('Failed to configure RX sync interface: ' + S);
    Exit;
  end;

  status := bladerf_enable_module(FDevice, BLADERF_MODULE_RX, True);
  if status <> 0 then
  begin
    S := StrPas(bladerf_strerror(status));
    Memo.Lines.Add('Failed to enable RX module: ' + S);
    Exit;
  end;

  i := Memo.Lines.Count - 1;
  S := Memo.Lines.Strings[i] + 'OK';
  Memo.Lines.Strings[i] := S;

  FRxActive := True;
  samples_len := 512*1000; //* May be any (reasonable) size */
  rx_size := samples_len*SizeOf(SmallInt)*2; //* May be any (reasonable) size */

  GetMem(Buffer, rx_size);
  GetMem(BufferTmp, rx_size div 2);

  StopRxBtn.Enabled := True;
  StartRxBtn.Enabled := False;
  while FRxActive = True do
  begin
    BufferPtr := Buffer;
    status := bladerf_sync_rx(FDevice, BufferPtr, samples_len, nil, 5000);
    if status <> 0 then
      break;

    n := 0;
    BufferTmpPtr := BufferTmp;
    while n < rx_size div 2 do
    begin
      val := BufferPtr^;
      inc(BufferPtr);
      val := val + BufferPtr^;
      inc(BufferPtr);
      BufferTmpPtr^ := val;
      inc(BufferTmpPtr);
      inc(n, 2);
    end;
    if OscR.DrawSpectr then
      OscR.AddSignal(BufferTmp^, samples_len div 200)
    else
      OscR.AddSignal(BufferTmp^, samples_len div 100);
    //
    Application.ProcessMessages;
  end;
  if status <> 0 then
  begin
    S := StrPas(bladerf_strerror(status));
    Memo.Lines.Add('Failed to enable RX module: ' + S);
  end;

  status := bladerf_enable_module(FDevice, BLADERF_MODULE_RX, False);
  if status <> 0 then
  begin
    S := StrPas(bladerf_strerror(status));
    Memo.Lines.Add('Failed to disable RX module: ' + S);
  end;
  StopRxBtn.Enabled := False;
  StartRxBtn.Enabled := True;
end;


function TForm1.init_sync(ADevice: Pointer): Integer;
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

  Result := bladerf_sync_config(ADevice,
                                BLADERF_MODULE_RX,
                                BLADERF_FORMAT_SC16_Q11,
                                num_buffers,
				buffer_size,
				num_transfers,
				timeout_ms);
end;


procedure TForm1.StopRxBtnClick(Sender: TObject);
begin
  FRxActive := False;
  //StopRxBtn.Enabled := False;
end;

procedure TForm1.OnMouseWheel(var Message: TMessage);
var i : Integer;
    zDelta : Short;
    F : PFreqLabelDescr;
    FMult : Cardinal;
begin
  if FRxLabelPtr = nil then Exit;

  i := FRxLabelsList.IndexOf(FRxLabelPtr);
  if i <> -1 then
  begin
    zDelta := HiWord(Message.WParam);
    F := FRxLabelsDescr.Items[i];
    FMult := 1;
    for i := 0 to F^.Digit - 1 do
      FMult := FMult * 10;
    if zDelta > 0 then
      inc(FRxFreq, FMult)
    else
      dec(FRxFreq, FMult);
    if FRxFreq < 300000 then
      FRxFreq := 300000;
    if FRxFreq > 2600000 then
      FRxFreq := 2600000;
    UpdateRxFreqLabels;
    SetRxFreqBtnClick(nil);
  end;
end;

procedure TForm1.RxLabel_GHz_1MouseEnter(Sender: TObject);
begin
  FRxLabelPtr := TRxLabel(Sender);
end;

procedure TForm1.RxLabel_GHz_1MouseLeave(Sender: TObject);
begin
  FRxLabelPtr := nil;
end;

end.
