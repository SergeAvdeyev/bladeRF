unit MainFrm;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, OscCEP, StdCtrls, RXCtrls, BladeRFIntf, BladeRFThread,
  Mask, ToolEdit, CurrEdit, RXSlider, NSP;

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
    GetRxFreqBtn: TButton;
    SetRxSRBtn: TButton;
    StartRxBtn: TButton;
    StopRxBtn: TButton;
    RxBox: TGroupBox;
    RxLabel_MHz_100: TRxLabel;
    RxLabel_MHz_10: TRxLabel;
    RxLabel_GHz_1: TRxLabel;
    RxLabel_MHz_1: TRxLabel;
    RxLabel_kHz_100: TRxLabel;
    RxLabel_kHz_10: TRxLabel;
    RxLabel_kHz_1: TRxLabel;
    DebugBox: TCheckBox;
    AllBox: TCheckBox;
    RxLabel1: TRxLabel;
    RxLabel2: TRxLabel;
    RxLabel3: TRxLabel;
    RxSrSlider: TRxSlider;
    RxBwSlider: TRxSlider;
    RxLabel4: TRxLabel;
    RxBwLabel: TCurrencyEdit;
    RxLabel5: TRxLabel;
    RxLnaSlider: TRxSlider;
    RxLnaLabel: TCurrencyEdit;
    RxLabel6: TRxLabel;
    RxVga1Slider: TRxSlider;
    RxVga1Label: TCurrencyEdit;
    RxLabel7: TRxLabel;
    RxVga2Slider: TRxSlider;
    RxVga2Label: TCurrencyEdit;
    RxSrLabel: TCurrencyEdit;
    RxLabel8: TRxLabel;
    procedure FormCreate(Sender: TObject);
    procedure TimerTimer(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Button1Click(Sender: TObject);
    procedure OpenDevBtnClick(Sender: TObject);
    procedure GetRxFreqBtnClick(Sender: TObject);
    procedure SetRxSRBtnClick(Sender: TObject);
    procedure StartRxBtnClick(Sender: TObject);
    procedure StopRxBtnClick(Sender: TObject);
    procedure RxLabel_GHz_1MouseEnter(Sender: TObject);
    procedure RxLabel_GHz_1MouseLeave(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure DebugBoxClick(Sender: TObject);
    procedure RxBwSliderChange(Sender: TObject);
    procedure RxSrSliderChange(Sender: TObject);
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

    //FRxActive : Boolean;

    FTxFreq : UINT;

    FRxBwBlocked : Boolean;
    FRxSrBlocked : Boolean;

    FRxLabelPtr : TRxLabel;
    FRxLabelsList : TList;
    FRxLabelsDescr : TList;

    FDeviceThread : TBladeThread;

    procedure PostMsg(AMessage: Cardinal; WParam: Integer; LParam: Integer);
    
    procedure SetRxFreqLabels;
    procedure UpdateRxFreqLabels;
    //function init_sync(ADevice: Pointer): Integer;

    procedure SetRxFreq;
    procedure GetRxSr;
    procedure GetRxBW;

    procedure OnConnected(var Msg: TMessage); message FromDev_Connect;
    procedure OnDisconnected(var Msg: TMessage); message FromDev_Disconnect;
    procedure OnRxSetFreq(var Msg: TMessage); message FromDev_RxSetFreq;
    procedure OnTxSetFreq(var Msg: TMessage); message FromDev_TxSetFreq;
    procedure OnRxGetFreq(var Msg: TMessage); message FromDev_RxGetFreq;
    procedure OnTxGetFreq(var Msg: TMessage); message FromDev_TxGetFreq;

    procedure OnRxSetSR(var Msg: TMessage); message FromDev_RxSetSR;
    procedure OnRxGetSR(var Msg: TMessage); message FromDev_RxGetSR;

    procedure OnTxSetSR(var Msg: TMessage); message FromDev_TxSetSR;
    procedure OnRxSetBW(var Msg: TMessage); message FromDev_RxSetBW;
    procedure OnRxGetBW(var Msg: TMessage); message FromDev_RxGetBW;
    procedure OnTxSetBW(var Msg: TMessage); message FromDev_TxSetBW;

    procedure OnRxStart(var Msg: TMessage); message FromDev_RxStart;
    procedure OnRxStop(var Msg: TMessage); message FromDev_RxStop;

    procedure OnRxError(var Msg: TMessage); message FromDev_RxProcessError;
    procedure OnRxData(var Msg: TMessage); message FromDev_RxProcessData;

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
  OscR.Hint := 'ƒл€ увеличени€ изображени€ по вертикали' + #10#13 +
               'используйте клавишу Alt и левую кнопку мыши' + #10#13 +
               'ƒл€ увеличени€ изображени€ по горизонтали' + #10#13 +
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
  FRxFreq := 0;    // In kHz
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
  if FDeviceThread.Suspended then
    FDeviceThread.Resume;
  FDeviceThread.Terminate;
  FDeviceThread.WaitFor;
  FDeviceThread.Free;

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

procedure TForm1.PostMsg(AMessage: Cardinal; WParam: Integer; LParam: Integer);
begin
  PostThreadMessage(FDeviceThread.ThreadID, AMessage, WParam, LParam);
  FDeviceThread.Resume;
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
    if AllBox.Checked then
      Memo.Lines.Add('Connecting to device... ');
    PostMsg(ToDevMsg_Connect, 0, 0);
    OpenDevBtn.Enabled := False;
  end
  else
  begin
    if AllBox.Checked then
      Memo.Lines.Add('Disconnecting ... ');
    PostMsg(ToDevMsg_Disconnect, 0, 0);
    OpenDevBtn.Enabled := False;
  end;
end;

procedure TForm1.SetRxFreq;
var I : Integer;
    S : String;
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Setting RX frequency... ');
  I := FDeviceThread.RxSetFreqSync(FRxFreq);
  if I = 0 then
    Memo.Lines.Add('Set RX Frequency - OK')
  else begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to set RX Frequency: ' + S);
  end;
  //PostMsg(ToDevMsg_RxSetFreq, FRxFreq, 0);
end;

procedure TForm1.GetRxFreqBtnClick(Sender: TObject);
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Read RX frequency... ');
  PostMsg(ToDevMsg_RxGetFreq, 0, 0);
end;



procedure TForm1.SetRxSRBtnClick(Sender: TObject);
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Setting RX sampling rate to ' + IntToStr(FRxSR) + ' Hz... ');
  PostMsg(ToDevMsg_RxSetSR, FRxSR, 0);
end;

procedure TForm1.GetRxSr;
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Read RX SampleRate...');
  FRxSrBlocked := True;
  PostMsg(ToDevMsg_RxGetSR, 0, 0);
end;


procedure TForm1.RxSrSliderChange(Sender: TObject);
var I : Integer;
    S : String;
    C : Cardinal;
begin
  if FRxSrBlocked then Exit;
  if FDeviceThread = nil then Exit;

  FRxSR := RxSrSlider.Value*1000000;

  if AllBox.Checked then
    Memo.Lines.Add('Setting RX sampling rate to ' + IntToStr(FRxSR) + ' Hz... ');
  I := FDeviceThread.RxSetSrSync(FRxSR, C);

  if I = 0 then begin
    if AllBox.Checked then
      Memo.Lines.Add('Actual RX sampling rate is ' + IntToStr(C) + ' Hz... ')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to set RX SampleRate: ' + S);
  end;
  //PostMsg(ToDevMsg_RxSetSR, FRxSR, 0);
end;

procedure TForm1.GetRxBW;
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Read RX bandwidth...');
  FRxBwBlocked := True;
  PostMsg(ToDevMsg_RxGetBW, 0, 0);
end;

procedure TForm1.RxBwSliderChange(Sender: TObject);
var F : Cardinal;
    I : Integer;
    S : String;
begin
  if FRxBwBlocked then Exit;
  if FDeviceThread = nil then Exit;

  FRxBW := RxBwSlider.Value*100000;

  if AllBox.Checked then
    Memo.Lines.Add('Setting RX bandwidth to ' + IntToStr(FRxBW) + ' Hz... ');
  I := FDeviceThread.RxSetBwSync(FRxBW, F);
  if I = 0 then begin
    RxBwLabel.Value := F;
    if AllBox.Checked then
      Memo.Lines.Add('Actual RX bandwidth is ' + IntToStr(F) + ' Hz... ')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to set RX Bandwidth: ' + S);
  end;
  //PostMsg(ToDevMsg_RxSetBW, FRxBW, 0);
end;




procedure TForm1.StartRxBtnClick(Sender: TObject);
begin
  if FDeviceThread = nil then Exit;

  if StartRxBtn.Tag = 0 then begin
    if AllBox.Checked then
      Memo.Lines.Add('Starting RX ... ');
    PostMsg(ToDevMsg_RxStart, 0, 0);
  end
  else
  begin
    if AllBox.Checked then
      Memo.Lines.Add('Stopping RX ... ');
    PostMsg(ToDevMsg_RxStop, 0, 0);
  end;
end;


procedure TForm1.StopRxBtnClick(Sender: TObject);
begin
  //FRxActive := False;
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
    if FRxFreq < 237500 then
      FRxFreq := 237500;
    if FRxFreq > 3800000 then
      FRxFreq := 3800000;
    UpdateRxFreqLabels;
    SetRxFreq;
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






///////////////////////////////////////////////////////
//////                                           //////
//////       Device events                       //////
//////                                           //////
///////////////////////////////////////////////////////

//procedure TForm1.OnDeviceError(var Msg: TMessage);
//var S : String;
//    P : PChar;
//begin
//  P := PChar(Msg.WParam);
//  S := StrPas(P);
//  Memo.Lines.Add('Failed: ' + S);
//  FreeMem(P);
//end;

procedure TForm1.OnConnected(var Msg: TMessage);
var S : String;
begin
  if Msg.WParam <> 0 then begin
    if DebugBox.Checked then begin
      S := StrPas(bladerf_strerror(Msg.WParam));
      Memo.Lines.Add('Failed to connect: ' + S);
    end;
    OpenDevBtn.Enabled := True;
    Exit;
  end;

  if AllBox.Checked then
    Memo.Lines.Add('Connected');
  GetRxFreqBtnClick(nil);
  GetRxBW;
  GetRxSr;
  OpenDevBtn.Enabled := True;
  OpenDevBtn.Caption := 'Disconnect';
  OpenDevBtn.Tag := 1;
  SetRxSRBtn.Enabled := true;
  //SetRxBWBtn.Enabled := true;
  GetRxFreqBtn.Enabled := true;
  StartRxBtn.Enabled := true;
  RxBox.Enabled := True;
end;

procedure TForm1.OnDisconnected(var Msg: TMessage);
begin
  Memo.Lines.Add('Disconnected');
  OpenDevBtn.Enabled := True;
  OpenDevBtn.Caption := 'Connect';
  OpenDevBtn.Tag := 0;
  SetRxSRBtn.Enabled := False;
  //SetRxBWBtn.Enabled := False;
  GetRxFreqBtn.Enabled := False;
  StartRxBtn.Enabled := False;
  RxBox.Enabled := False;
end;

procedure TForm1.OnRxSetFreq(var Msg: TMessage);
var S : String;
begin
  if Msg.WParam = 0 then
    Memo.Lines.Add('Set RX Frequency - OK')
  else begin
    S := StrPas(bladerf_strerror(Msg.WParam));
    Memo.Lines.Add('Failed to set RX Frequency: ' + S);
  end;
end;

procedure TForm1.OnTxSetFreq(var Msg: TMessage);
var S : String;
begin
  if Msg.WParam = 0 then
    Memo.Lines.Add('Set TX Frequency - OK')
  else begin
    S := StrPas(bladerf_strerror(Msg.WParam));
    Memo.Lines.Add('Failed to set TX Frequency: ' + S);
  end;
end;

procedure TForm1.OnRxGetFreq(var Msg: TMessage);
var S : String;
begin
  if Msg.WParam = 0 then begin
    FRxFreq := Msg.LParam;
    UpdateRxFreqLabels;
    if AllBox.Checked then
      Memo.Lines.Add('Actual RX Frequency is ' + IntToStr(FRxFreq) + ' к√ц')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(Msg.WParam));
    Memo.Lines.Add('Failed to get RX Frequency: ' + S);
  end;
end;

procedure TForm1.OnTxGetFreq(var Msg: TMessage);
var S : String;
begin
  if Msg.WParam = 0 then begin
    if AllBox.Checked then
      Memo.Lines.Add('Actual TX Frequency is ' + IntToStr(Msg.LParam) + ' к√ц')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(Msg.WParam));
    Memo.Lines.Add('Failed to get TX Frequency: ' + S);
  end;
end;

procedure TForm1.OnRxSetSR(var Msg: TMessage);
var S : String;
begin
  if Msg.WParam = 0 then begin
    if AllBox.Checked then
      Memo.Lines.Add('Actual RX sampling rate is ' + IntToStr(Msg.LParam) + ' Hz... ')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(Msg.WParam));
    Memo.Lines.Add('Failed to set RX SampleRate: ' + S);
  end;
end;

procedure TForm1.OnRxGetSR(var Msg: TMessage);
var S : String;
begin
  if Msg.WParam = 0 then begin
    RxSrSlider.Value := Msg.LParam div 1000000;
    RxSrLabel.Value := Msg.LParam;
    FRxSrBlocked := False;
    if AllBox.Checked then
      Memo.Lines.Add('Actual RX sampling rate is ' + IntToStr(Msg.LParam) + ' Hz... ')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(Msg.WParam));
    Memo.Lines.Add('Failed to set RX SampleRate: ' + S);
  end;
end;

procedure TForm1.OnTxSetSR(var Msg: TMessage);
var S : String;
begin
  if Msg.WParam = 0 then begin
    if AllBox.Checked then
      Memo.Lines.Add('Actual TX sampling rate is ' + IntToStr(Msg.LParam) + ' Hz... ')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(Msg.WParam));
    Memo.Lines.Add('Failed to set TX SampleRate: ' + S);
  end;
end;

procedure TForm1.OnRxSetBW(var Msg: TMessage);
var S : String;
begin
  if Msg.WParam = 0 then begin
    RxBwLabel.Value := Msg.LParam;
    if AllBox.Checked then
      Memo.Lines.Add('Actual RX bandwidth is ' + IntToStr(Msg.LParam) + ' Hz... ')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(Msg.WParam));
    Memo.Lines.Add('Failed to set RX Bandwidth: ' + S);
  end;
end;

procedure TForm1.OnRxGetBW(var Msg: TMessage);
var S : String;
begin
  if Msg.WParam = 0 then begin
    RxBwLabel.Value := Msg.LParam;
    RxBwSlider.Value := Msg.LParam div 100000;
    FRxBwBlocked := False;
    if AllBox.Checked then
      Memo.Lines.Add('Actual RX bandwidth is ' + IntToStr(Msg.LParam) + ' Hz... ')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(Msg.WParam));
    Memo.Lines.Add('Failed to set RX Bandwidth: ' + S);
  end;
end;

procedure TForm1.OnTxSetBW(var Msg: TMessage);
var S : String;
begin
  if Msg.WParam = 0 then begin
    if AllBox.Checked then
      Memo.Lines.Add('Actual TX bandwidth is ' + IntToStr(Msg.LParam) + ' Hz... ')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(Msg.WParam));
    Memo.Lines.Add('Failed to set TX Bandwidth: ' + S);
  end;
end;


procedure TForm1.OnRxStart(var Msg: TMessage);
var S : String;
begin
  if Msg.WParam = 0 then begin
    StartRxBtn.Tag := 1;
    StartRxBtn.Caption := 'Stop RX';
    if AllBox.Checked then
      Memo.Lines.Add('RX started')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(Msg.WParam));
    Memo.Lines.Add('Failed to start RX: ' + S);
  end;
end;

procedure TForm1.OnRxStop(var Msg: TMessage);
var S : String;
begin
  if Msg.WParam = 0 then begin
    StartRxBtn.Tag := 0;
    StartRxBtn.Caption := 'Start RX';
    if AllBox.Checked then
      Memo.Lines.Add('RX stopped')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(Msg.WParam));
    Memo.Lines.Add('Failed to stop RX: ' + S);
  end;
end;



procedure TForm1.OnRxError(var Msg: TMessage);
var S : String;
begin
  S := StrPas(bladerf_strerror(Msg.WParam));
  Memo.Lines.Add('Failed receiving Data: ' + S);
end;

procedure TForm1.OnRxData(var Msg: TMessage);
var
    //i : Integer;
    n : Integer;

    Buffer : PSmallInt;
    BufferPtr : PSmallInt;

    samples_len : UINT;
    BufferTmpPtr : PSmallInt;
    val : Smallint;
begin
  Buffer := PSmallInt(Msg.WParam);
  BufferPtr := Buffer;
  BufferTmpPtr := Buffer;


  samples_len := Msg.LParam div 4;

  n := 0;
  while n < Msg.LParam div 2 do
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
    OscR.AddSignal(Buffer^, samples_len div 200)
  else
    OscR.AddSignal(Buffer^, samples_len div 100);
  

  FreeMemory(Buffer);
end;







procedure TForm1.DebugBoxClick(Sender: TObject);
begin
  if not DebugBox.Checked then
    AllBox.Checked := False;
  AllBox.Enabled := DebugBox.Checked;
end;


end.
