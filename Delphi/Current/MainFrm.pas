unit MainFrm;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, OscCEP, StdCtrls, RXCtrls, BladeRFIntf, BladeRFThread,
  Mask, ToolEdit, CurrEdit, RXSlider, FIRFilter, RXSpin, FFT, Nsp, Math,
  MMWaveOut, MMLevel, MMWaveBasic, MMTypes, MMSystem, IniFiles, Buttons;

Const
   Buffer_Size = 8192;


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
    Label1: TLabel;
    OpenDevBtn: TButton;
    Memo: TMemo;
    StartRxBtn: TButton;
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
    TxBox: TGroupBox;
    TxLabel_MHz_100: TRxLabel;
    TxLabel_MHz_10: TRxLabel;
    TxLabel_GHz_1: TRxLabel;
    TxLabel_MHz_1: TRxLabel;
    TxLabel_kHz_100: TRxLabel;
    TxLabel_kHz_10: TRxLabel;
    TxLabel_kHz_1: TRxLabel;
    RxLabel16: TRxLabel;
    RxLabel17: TRxLabel;
    RxLabel18: TRxLabel;
    RxLabel19: TRxLabel;
    RxLabel21: TRxLabel;
    RxLabel22: TRxLabel;
    RxLabel23: TRxLabel;
    TxSrSlider: TRxSlider;
    TxBwSlider: TRxSlider;
    TxBwLabel: TCurrencyEdit;
    TxVga1Slider: TRxSlider;
    TxVga1Label: TCurrencyEdit;
    TxVga2Slider: TRxSlider;
    TxVga2Label: TCurrencyEdit;
    TxSrLabel: TCurrencyEdit;
    Panel2: TPanel;
    FFTBox: TPaintBox;
    ReCorrectionEdit: TRxSpinEdit;
    ImCorrectionEdit: TRxSpinEdit;
    WaveOut: TMMWaveOut;
    Audio_Out: TComboBox;
    Label19: TLabel;
    rxSliderVolume: TRxSlider;
    StartTxBtn: TButton;
    Timer1: TTimer;
    Label2: TLabel;
    Label3: TLabel;
    SpectrumRB: TRadioButton;
    SignalRB: TRadioButton;
    TxValueEdit: TRxSpinEdit;
    Label4: TLabel;
    ZoomInBtn: TSpeedButton;
    ZoomOutBtn: TSpeedButton;
    procedure FormCreate(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure OpenDevBtnClick(Sender: TObject);
    procedure StartRxBtnClick(Sender: TObject);
    procedure RxLabel_GHz_1MouseEnter(Sender: TObject);
    procedure RxLabel_GHz_1MouseLeave(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure DebugBoxClick(Sender: TObject);
    procedure RxBwSliderChange(Sender: TObject);
    procedure RxSrSliderChange(Sender: TObject);
    procedure RxLnaSliderChange(Sender: TObject);
    procedure RxVga1SliderChange(Sender: TObject);
    procedure RxVga2SliderChange(Sender: TObject);
    procedure TxLabel_GHz_1MouseEnter(Sender: TObject);
    procedure TxLabel_GHz_1MouseLeave(Sender: TObject);
    procedure FFTBoxPaint(Sender: TObject);
    procedure WaveOutStop(Sender: TObject);
    procedure WaveOutStart(Sender: TObject);
    procedure WaveOutBufferReady(Sender: TObject; lpWaveHdr: PWaveHdr);
    procedure WaveOutBufferLoad(Sender: TObject; lpWaveHdr: PWaveHdr;
      var MoreBuffers: Boolean);
    procedure Audio_OutChange(Sender: TObject);
    procedure rxSliderVolumeChange(Sender: TObject);
    procedure TxSrSliderChange(Sender: TObject);
    procedure TxBwSliderChange(Sender: TObject);
    procedure TxVga1SliderChange(Sender: TObject);
    procedure TxVga2SliderChange(Sender: TObject);
    procedure StartTxBtnClick(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure ZoomInBtnClick(Sender: TObject);
    procedure ZoomOutBtnClick(Sender: TObject);
  private
    { Private declarations }
    IniFile : TIniFile;
    Osc    : TOsc;
    //pBuffer : PChar;
    //BufferSize : Integer;
    //hFile   : Integer;
    
    //FIR_High  : TFIRFilter;
    FIR_Low1  : TFIRFilter;
    //FIR_Low2  : TFIRFilter;

    //FCounter : Integer;

    FDevice : Pointer;

    FRxFreq : UINT;
    FRxSR   : UINT;
    FRxBW   : UINT;

    //FRxActive : Boolean;

    FTxFreq : UINT;
    FTxSR   : UINT;
    FTxBW   : UINT;

    FRxBwBlocked : Boolean;
    FRxSrBlocked : Boolean;

    FRxLnaBlocked : Boolean;
    FRxVga1Blocked : Boolean;
    FRxVga2Blocked : Boolean;

    FTxBwBlocked : Boolean;
    FTxSrBlocked : Boolean;

    FTxVga1Blocked : Boolean;
    FTxVga2Blocked : Boolean;

    FRxLabelPtr : TRxLabel;
    FRxLabelsList : TList;
    FRxLabelsDescr : TList;

    FTxLabelPtr : TRxLabel;
    FTxLabelsList : TList;
    FTxLabelsDescr : TList;

    FDeviceThread : TBladeThread;

    FBitMap : TBitMap;

    Fft : TFFT;

    //SpectrWCplx1 : PWCplx;
    //SpectrWCplx2 : PWCplx;

    SpectrDCplxRe : PDCplx;
    SpectrDCplxIm : PDCplx;

    FMax : Double;

    Phase : Double;

    HotBuffer : PSmallInt;
    HotBufferWritten : Integer;
    HotList : TList;

    ZoomFactor : Integer;

    //FTxSamplesLen : Integer;
    //FTxBufferSize : Integer;
    //FTxBuffer     : PChar; 

    procedure PostMsg(AMessage: Cardinal; WParam: Integer; LParam: Integer);
    
    procedure SetRxFreqLabels;
    procedure UpdateRxFreqLabels;

    procedure SetTxFreqLabels;
    procedure UpdateTxFreqLabels;

    procedure SetRxFreq;
    //procedure GetRxFreq;
    procedure GetRxSr;
    procedure GetRxBW;
    procedure GetRxLna;
    procedure GetRxVga1;
    procedure GetRxVga2;

    procedure SetTxFreq;
    //procedure GetTxFreq;
    procedure GetTxSr;
    procedure GetTxBW;
    procedure GetTxVga1;
    procedure GetTxVga2;

    procedure OnConnected(var Msg: TMessage); message FromDev_Connect;
    procedure OnDisconnected(var Msg: TMessage); message FromDev_Disconnect;

//    procedure OnTxSetFreq(var Msg: TMessage); message FromDev_TxSetFreq;
//    procedure OnTxGetFreq(var Msg: TMessage); message FromDev_TxGetFreq;
//    procedure OnTxSetSR(var Msg: TMessage); message FromDev_TxSetSR;
//    procedure OnTxSetBW(var Msg: TMessage); message FromDev_TxSetBW;

    procedure OnRxStart(var Msg: TMessage); message FromDev_RxStart;
    procedure OnRxStop(var Msg: TMessage); message FromDev_RxStop;

    procedure OnTxStart(var Msg: TMessage); message FromDev_TxStart;
    procedure OnTxStop(var Msg: TMessage); message FromDev_TxStop;

    procedure OnRxError(var Msg: TMessage); message FromDev_RxProcessError;
    procedure OnRxData(var Msg: TMessage); message FromDev_RxProcessData;

    procedure OnTxError(var Msg: TMessage); message FromDev_TxProcessError;

    procedure OnMouseWheel(var Message: TMessage); message WM_MOUSEWHEEL;

    procedure ReDraw(SignalBuffer: PDCplx);
    procedure StartAudio;
    procedure StopAudio;
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

procedure TForm1.FormCreate(Sender: TObject);
var i : Integer;
    DevInfo : TMMDeviceInfo;
begin
  IniFile := TIniFile.Create('GuiApp.ini');

  Randomize;
  
  Osc := TOsc.Create(Panel1);
  Osc.Align := alClient;
  //Osc.Freq := 4000000;
  Osc.Freq := 48000;
  Osc.CountSamples := Buffer_Size;
  Osc.RealTime := False;
  Osc.EnableSpectr := True;
  Osc.LineColor := clAqua;
  Osc.CountFFT := 2048;
  Osc.MaxSpectrValue := 10;
  Osc.MinSpectrValue := -130;
  Osc.Visible := True;

  //WaveOut := TMMWaveOut.Create(Self);
  //WaveOut.OnBufferLoad := WaveOutBufferLoad;
  //WaveOut.OnBufferReady := WaveOutBufferReady;
  //WaveOut.OnStart := WaveOutStart;
  //WaveOut.OnStop  := WaveOutStop;
  WaveOut.SampleRate := sr48000;
  WaveOut.BitRate    := br16bits;
  WaveOut.PlayMode   := mMono;
  WaveOut.BufferSize := Buffer_Size*2;
  WaveOut.NumBuffers := 4;
  //WaveOut.LevelLeft  := MMLevel;
  for i := 0 to WaveOut.DeviceCount - 1 do
  begin
    WaveOut.GetDeviceInfo(i, @DevInfo);
    Audio_Out.Items.Add(DevInfo.szPname + ' - ' + IntToStr(i));
  end;
  Audio_Out.ItemIndex := IniFile.ReadInteger('Audio', 'DeviceID', -1);
  rxSliderVolume.Value := IniFile.ReadInteger('Audio', 'Volume', 0);
  //WaveOut.DeviceID   := 0;

  FDevice := nil;
  FRxFreq := 0;    // In kHz
  FRxSR := 4000000;
  FRxBW := 1500000;

  FTxFreq := 0;   // In kHz
  FBitMap := TBitMap.Create;
  FBitMap.Width := FFTBox.Width;
  FBitMap.Height := FFTBox.Height;
  
  FDeviceThread := TBladeThread.Create(Handle);
  FDeviceThread.Resume;

  //FIR_High := TFIRFilter.Create(4000000);
  //FIR_High.FIRInitHighPass(200000, 88);

  FIR_Low1 := TFIRFilter.Create(4800000);
  FIR_Low1.FIRInitLowPass(2300000, 120);

  //GetMem(SpectrWCplx1, (2048 + 1)*SizeOf(TWCplx));
  //ZeroMemory(SpectrWCplx1, (2048 + 1)*SizeOf(TWCplx));
  //GetMem(SpectrWCplx2, (2048 + 1)*SizeOf(TWCplx));
  //ZeroMemory(SpectrWCplx2, (2048 + 1)*SizeOf(TWCplx));

  GetMem(SpectrDCplxRe, (2048)*SizeOf(TDCplx));
  ZeroMemory(SpectrDCplxRe, (2048)*SizeOf(TDCplx));
  GetMem(SpectrDCplxIm, (2048)*SizeOf(TDCplx));
  ZeroMemory(SpectrDCplxIm, (2048)*SizeOf(TDCplx));

  Fft := TFFT.Create;
  Fft.FFTSize := 11;

  HotList := TList.Create;
  HotBuffer := nil;
  HotBufferWritten := 0;
  ZoomFactor := 1;
end;

procedure TForm1.FormActivate(Sender: TObject);
begin
  SetRxFreqLabels;
  UpdateRxFreqLabels;
  SetTxFreqLabels;
  UpdateTxFreqLabels;
end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
var FFreqLabelDescr : PFreqLabelDescr;
begin
  IniFile.WriteInteger('Audio', 'DeviceID', Audio_Out.ItemIndex);
  IniFile.WriteInteger('Audio', 'Volume', rxSliderVolume.Value);

  IniFile.WriteInteger('Blade_RX', 'CenterFreq', FRxFreq);
  IniFile.WriteInteger('Blade_RX', 'SampleRate', RxSrSlider.Value);
  IniFile.WriteInteger('Blade_RX', 'BandWidth', RxBwSlider.Value);
  IniFile.WriteInteger('Blade_RX', 'LNA', RxLnaSlider.Value);
  IniFile.WriteInteger('Blade_RX', 'VGA1', RxVga1Slider.Value);
  IniFile.WriteInteger('Blade_RX', 'VGA2', RxVga2Slider.Value);

  IniFile.WriteInteger('Blade_TX', 'CenterFreq', FTxFreq);
  IniFile.WriteInteger('Blade_TX', 'SampleRate', TxSrSlider.Value);
  IniFile.WriteInteger('Blade_TX', 'BandWidth', TxBwSlider.Value);
  IniFile.WriteInteger('Blade_TX', 'VGA1', TxVga1Slider.Value);
  IniFile.WriteInteger('Blade_TX', 'VGA2', TxVga2Slider.Value);

  if FDeviceThread.Suspended then
    FDeviceThread.Resume;
  FDeviceThread.Terminate;
  FDeviceThread.WaitFor;
  FDeviceThread.Free;

  StopAudio;
  while HotList.Count > 0 do begin
    FreeMem(HotList.Items[0]);
    HotList.Delete(0);
  end;
  HotList.Free;
  FreeMem(HotBuffer);

  FRxLabelsList.Free;

  while FRxLabelsDescr.Count > 0 do
  begin
    FFreqLabelDescr := FRxLabelsDescr.Items[0];
    FreeMem(FFreqLabelDescr);
    FRxLabelsDescr.Delete(0);
  end;
  FRxLabelsDescr.Free;

  //FIR_Low1.Free;
  //FIR_High.Free;

  Fft.Free;

  //FreeMem(SpectrWCplx1);
  //FreeMem(SpectrWCplx2);

  FreeMem(SpectrDCplxRe);
  FreeMem(SpectrDCplxIm);
  IniFile.Free;
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
    end
    else if i = 1 then begin
      FFreqLabelDescr^.Module := 100;
      FFreqLabelDescr^.Multiplier := 10;
    end
    else if i = 2 then begin
      FFreqLabelDescr^.Module := 1000;
      FFreqLabelDescr^.Multiplier := 100;
    end
    else if i = 3 then begin
      FFreqLabelDescr^.Module := 10000;
      FFreqLabelDescr^.Multiplier := 1000;
    end
    else if i = 4 then begin
      FFreqLabelDescr^.Module := 100000;
      FFreqLabelDescr^.Multiplier := 10000;
    end
    else if i = 5 then begin
      FFreqLabelDescr^.Module := 1000000;
      FFreqLabelDescr^.Multiplier := 100000;
    end
    else if i = 6 then begin
      FFreqLabelDescr^.Module := 10000000;
      FFreqLabelDescr^.Multiplier := 1000000;
    end;
    FRxLabelsDescr.Add(FFreqLabelDescr);
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



procedure TForm1.SetTxFreqLabels;
var FFreqLabelDescr : PFreqLabelDescr;
    i : Integer;
begin
  FTxLabelPtr := nil;
  FTxLabelsList := TList.Create;
  FTxLabelsList.Add(TxLabel_kHz_1);
  FTxLabelsList.Add(TxLabel_kHz_10);
  FTxLabelsList.Add(TxLabel_kHz_100);
  FTxLabelsList.Add(TxLabel_MHz_1);
  FTxLabelsList.Add(TxLabel_MHz_10);
  FTxLabelsList.Add(TxLabel_MHz_100);
  FTxLabelsList.Add(TxLabel_GHz_1);

  i := 0;
  FTxLabelsDescr := TList.Create;
  while i < 7 do begin
    GetMem(FFreqLabelDescr, SizeOf(TFreqLabelDescr));
    FFreqLabelDescr^.Digit := i;
    if i = 0 then begin
      FFreqLabelDescr^.Module := 10;
      FFreqLabelDescr^.Multiplier := 1;
    end
    else if i = 1 then begin
      FFreqLabelDescr^.Module := 100;
      FFreqLabelDescr^.Multiplier := 10;
    end
    else if i = 2 then begin
      FFreqLabelDescr^.Module := 1000;
      FFreqLabelDescr^.Multiplier := 100;
    end
    else if i = 3 then begin
      FFreqLabelDescr^.Module := 10000;
      FFreqLabelDescr^.Multiplier := 1000;
    end
    else if i = 4 then begin
      FFreqLabelDescr^.Module := 100000;
      FFreqLabelDescr^.Multiplier := 10000;
    end
    else if i = 5 then begin
      FFreqLabelDescr^.Module := 1000000;
      FFreqLabelDescr^.Multiplier := 100000;
    end
    else if i = 6 then begin
      FFreqLabelDescr^.Module := 10000000;
      FFreqLabelDescr^.Multiplier := 1000000;
    end;
    FTxLabelsDescr.Add(FFreqLabelDescr);
    inc(i);
  end;
end;

procedure TForm1.UpdateTxFreqLabels;
var F : PFreqLabelDescr;
    i : Integer;
    L : TRxLabel;
begin
  for i := 0 to FTxLabelsList.Count - 1 do
  begin
    L := FTxLabelsList.Items[i];
    F := FTxLabelsDescr.Items[i];
    F^.LabelValue := (FTxFreq mod F^.Module) div F^.Multiplier;
    L.Caption := IntToStr(F^.LabelValue);
  end;
end;




procedure TForm1.OpenDevBtnClick(Sender: TObject);
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


///////////////////////////////////////////
////////                        ///////////
////////     RX                 ///////////
////////                        ///////////
///////////////////////////////////////////

procedure TForm1.SetRxFreq;
var I : Integer;
    S : String;
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Setting RX frequency... ');
  I := FDeviceThread.RxSetFreqSync(FRxFreq);
  if I = 0 then begin
    if AllBox.Checked then
      Memo.Lines.Add('Set RX Frequency - OK');
  end
  else begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to set RX Frequency: ' + S);
  end;
end;

{
procedure TForm1.GetRxFreq;
var I : Integer;
    S : String;
    F : Integer;
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Read RX frequency... ');

  I := FDeviceThread.RxGetFreqSync(F);

  if I = 0 then begin
    FRxFreq := F;
    UpdateRxFreqLabels;
    if AllBox.Checked then
      Memo.Lines.Add('Actual RX Frequency is ' + IntToStr(FRxFreq) + ' к√ц')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to get RX Frequency: ' + S);
  end;
end;
}



procedure TForm1.GetRxSr;
var I : Integer;
    S : String;
    F : Cardinal;
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Read RX SampleRate...');
  FRxSrBlocked := True;

  I := FDeviceThread.RxGetSrSync(F);
  if I = 0 then begin
    //RxSrSlider.Value := F div 100000;
    RxSrLabel.Value := F;
    FRxSrBlocked := False;
    if AllBox.Checked then
      Memo.Lines.Add('Actual RX sampling rate is ' + IntToStr(F) + ' Hz')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to get RX SampleRate: ' + S);
  end;
end;


procedure TForm1.GetRxBW;
var I : Integer;
    S : String;
    F : Cardinal;
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Read RX bandwidth...');
  FRxBwBlocked := True;
  I := FDeviceThread.RxGetBwSync(F);
  if I = 0 then begin
    RxBwLabel.Value := F;
    //RxBwSlider.Value := F div 100000;
    FRxBwBlocked := False;
    if AllBox.Checked then
      Memo.Lines.Add('Actual RX bandwidth is ' + IntToStr(F) + ' Hz')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to get RX Bandwidth: ' + S);
  end;
end;

procedure TForm1.GetRxLna;
var I : Integer;
    S : String;
    F : Cardinal;
begin
  Exit;
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Read RX LNA...');
  FRxLnaBlocked := True;
  I := FDeviceThread.RxGetLnaSync(F);
  if I = 0 then begin
    RxLnaLabel.Value := F;
    //RxBwSlider.Value := F div 100000;
    FRxLnaBlocked := False;
    if AllBox.Checked then
      Memo.Lines.Add('Actual RX LNA is ' + IntToStr(F) + ' dB');
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to get RX LNA: ' + S);
  end;
end;

procedure TForm1.GetRxVga1;
var I : Integer;
    S : String;
    F : Integer;
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Read RX VGA1...');
  FRxVga1Blocked := True;
  I := FDeviceThread.RxGetVga1Sync(F);
  if I = 0 then begin
    RxVga1Label.Value := F;
    //RxVga1Slider.Value := F;
    FRxVga1Blocked := False;
    if AllBox.Checked then
      Memo.Lines.Add('Actual RX VGA1 is ' + IntToStr(F));
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to get RX VGA1: ' + S);
  end;
end;

procedure TForm1.GetRxVga2;
var I : Integer;
    S : String;
    F : Integer;
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Read RX VGA2...');
  FRxVga2Blocked := True;
  I := FDeviceThread.RxGetVga2Sync(F);
  if I = 0 then begin
    RxVga2Label.Value := F;
    //RxVga2Slider.Value := F;
    FRxVga2Blocked := False;
    if AllBox.Checked then
      Memo.Lines.Add('Actual RX VGA2 is ' + IntToStr(F));
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to get RX VGA2: ' + S);
  end;
end;






///////////////////////////////////////////
////////                        ///////////
////////     TX                 ///////////
////////                        ///////////
///////////////////////////////////////////
procedure TForm1.SetTxFreq;
var I : Integer;
    S : String;
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Setting TX frequency... ');
  I := FDeviceThread.TxSetFreqSync(FTxFreq);
  if I = 0 then begin
    if AllBox.Checked then
      Memo.Lines.Add('Set TX Frequency - OK');
  end
  else begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to set TX Frequency: ' + S);
  end;
end;

{
procedure TForm1.GetTxFreq;
var I : Integer;
    S : String;
    F : Integer;
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Read TX frequency... ');

  I := FDeviceThread.TxGetFreqSync(F);

  if I = 0 then begin
    FTxFreq := F;
    UpdateTxFreqLabels;
    if AllBox.Checked then
      Memo.Lines.Add('Actual TX Frequency is ' + IntToStr(FTxFreq) + ' к√ц')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to get TX Frequency: ' + S);
  end;
end;
}

procedure TForm1.GetTxSr;
var I : Integer;
    S : String;
    F : Cardinal;
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Read TX SampleRate...');
  FTxSrBlocked := True;

  I := FDeviceThread.TxGetSrSync(F);
  if I = 0 then begin
    //RxSrSlider.Value := F div 100000;
    TxSrLabel.Value := F;
    FTxSrBlocked := False;
    if AllBox.Checked then
      Memo.Lines.Add('Actual TX sampling rate is ' + IntToStr(F) + ' Hz')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to get TX SampleRate: ' + S);
  end;
end;


procedure TForm1.GetTxBW;
var I : Integer;
    S : String;
    F : Cardinal;
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Read TX bandwidth...');
  FTxBwBlocked := True;
  I := FDeviceThread.TxGetBwSync(F);
  if I = 0 then begin
    TxBwLabel.Value := F;
    //RxBwSlider.Value := F div 100000;
    FTxBwBlocked := False;
    if AllBox.Checked then
      Memo.Lines.Add('Actual TX bandwidth is ' + IntToStr(F) + ' Hz')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to get TX Bandwidth: ' + S);
  end;
end;

procedure TForm1.GetTxVga1;
var I : Integer;
    S : String;
    F : Integer;
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Read TX VGA1...');
  FTxVga1Blocked := True;
  I := FDeviceThread.TxGetVga1Sync(F);
  if I = 0 then begin
    TxVga1Label.Value := F;
    //RxVga1Slider.Value := F;
    FTxVga1Blocked := False;
    if AllBox.Checked then
      Memo.Lines.Add('Actual TX VGA1 is ' + IntToStr(F));
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to get TX VGA1: ' + S);
  end;
end;

procedure TForm1.GetTxVga2;
var I : Integer;
    S : String;
    F : Integer;
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Read TX VGA2...');
  FTxVga2Blocked := True;
  I := FDeviceThread.TxGetVga2Sync(F);
  if I = 0 then begin
    TxVga2Label.Value := F;
    //RxVga2Slider.Value := F;
    FTxVga2Blocked := False;
    if AllBox.Checked then
      Memo.Lines.Add('Actual TX VGA2 is ' + IntToStr(F));
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to get TX VGA2: ' + S);
  end;
end;
















procedure TForm1.RxSrSliderChange(Sender: TObject);
var I : Integer;
    S : String;
    F : Cardinal;
begin
  if FRxSrBlocked then Exit;
  if FDeviceThread = nil then Exit;

  FRxSR := RxSrSlider.Value*100000;

  if AllBox.Checked then
    Memo.Lines.Add('Setting RX sampling rate to ' + IntToStr(FRxSR) + ' Hz... ');
  I := FDeviceThread.RxSetSrSync(FRxSR, F);

  if I = 0 then begin
    RxSrLabel.Value := F;
    if AllBox.Checked then
      Memo.Lines.Add('Actual RX sampling rate is ' + IntToStr(F) + ' Hz... ')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to set RX SampleRate: ' + S);
  end;
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
end;

procedure TForm1.RxLnaSliderChange(Sender: TObject);
var I : Integer;
    S : String;
begin
  //if FRxLnaBlocked then Exit;
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Setting RX LNA to ' + IntToStr(RxLnaSlider.Value));
  I := FDeviceThread.RxSetLnaSync(RxLnaSlider.Value);
  if I = 0 then
    RxLnaLabel.Value := RxLnaSlider.Value
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to set RX LNA: ' + S);
  end;
end;

procedure TForm1.RxVga1SliderChange(Sender: TObject);
var I : Integer;
    S : String;
    F : Integer;
begin
  if FRxVga1Blocked then Exit;
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Setting RX VGA1 to ' + IntToStr(RxVga1Slider.Value));
  I := FDeviceThread.RxSetVga1Sync(RxVga1Slider.Value, F);
  if I = 0 then
    RxVga1Label.Value := F
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to set RX VGA1: ' + S);
  end;
end;

procedure TForm1.RxVga2SliderChange(Sender: TObject);
var I : Integer;
    S : String;
    F : Integer;
begin
  if FRxVga2Blocked then Exit;
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Setting RX VGA2 to ' + IntToStr(RxVga2Slider.Value));
  I := FDeviceThread.RxSetVga2Sync(RxVga2Slider.Value, F);
  if I = 0 then
    RxVga2Label.Value := F
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to set RX VGA2: ' + S);
  end;
end;




procedure TForm1.TxSrSliderChange(Sender: TObject);
var I : Integer;
    S : String;
    F : Cardinal;
begin
  if FTxSrBlocked then Exit;
  if FDeviceThread = nil then Exit;

  FTxSR := TxSrSlider.Value*100000;

  if AllBox.Checked then
    Memo.Lines.Add('Setting TX sampling rate to ' + IntToStr(FTxSR) + ' Hz... ');
  I := FDeviceThread.TxSetSrSync(FTxSR, F);

  if I = 0 then begin
    TxSrLabel.Value := F;
    if AllBox.Checked then
      Memo.Lines.Add('Actual TX sampling rate is ' + IntToStr(F) + ' Hz... ')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to set TX SampleRate: ' + S);
  end;
end;

procedure TForm1.TxBwSliderChange(Sender: TObject);
var F : Cardinal;
    I : Integer;
    S : String;
begin
  if FTxBwBlocked then Exit;
  if FDeviceThread = nil then Exit;

  FTxBW := TxBwSlider.Value*100000;

  if AllBox.Checked then
    Memo.Lines.Add('Setting TX bandwidth to ' + IntToStr(FTxBW) + ' Hz... ');
  I := FDeviceThread.TxSetBwSync(FTxBW, F);
  if I = 0 then begin
    TxBwLabel.Value := F;
    if AllBox.Checked then
      Memo.Lines.Add('Actual TX bandwidth is ' + IntToStr(F) + ' Hz... ')
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to set TX Bandwidth: ' + S);
  end;
end;

procedure TForm1.TxVga1SliderChange(Sender: TObject);
var I : Integer;
    S : String;
    F : Integer;
begin
  if FTxVga1Blocked then Exit;
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Setting TX VGA1 to ' + IntToStr(TxVga1Slider.Value));
  I := FDeviceThread.TxSetVga1Sync(TxVga1Slider.Value, F);
  if I = 0 then
    TxVga1Label.Value := F
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to set TX VGA1: ' + S);
  end;
end;

procedure TForm1.TxVga2SliderChange(Sender: TObject);
var I : Integer;
    S : String;
    F : Integer;
begin
  if FTxVga2Blocked then Exit;
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
    Memo.Lines.Add('Setting TX VGA2 to ' + IntToStr(TxVga2Slider.Value));
  I := FDeviceThread.TxSetVga2Sync(TxVga2Slider.Value, F);
  if I = 0 then
    TxVga2Label.Value := F
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(I));
    Memo.Lines.Add('Failed to set TX VGA2: ' + S);
  end;
end;

















procedure TForm1.StartRxBtnClick(Sender: TObject);
var Status : Integer;
    S : String;
begin
  if FDeviceThread = nil then Exit;

  if StartRxBtn.Tag = 0 then begin
    if AllBox.Checked then
      Memo.Lines.Add('Starting RX ... ');
    PostMsg(ToDevMsg_RxStart, 0, 0);
    Exit;

    Status := FDeviceThread.RxStartSync;

    if Status = 0 then begin
      StartRxBtn.Tag := 1;
      StartRxBtn.Caption := 'Stop RX';
      if AllBox.Checked then
        Memo.Lines.Add('RX started');
      FDeviceThread.Resume;
    end
    else if DebugBox.Checked then begin
      S := StrPas(bladerf_strerror(Status));
      Memo.Lines.Add('Failed to start RX: ' + S);
    end;

  end
  else
  begin
    if AllBox.Checked then
      Memo.Lines.Add('Stopping RX ... ');
    PostMsg(ToDevMsg_RxStop, 0, 0);
    Exit;
    
    Status := FDeviceThread.RxStopSync;

    if Status = 0 then begin
      StartRxBtn.Tag := 0;
      StartRxBtn.Caption := 'Start RX';
      if AllBox.Checked then
        Memo.Lines.Add('RX stopped');
      FDeviceThread.Resume;
    end
    else if DebugBox.Checked then begin
      S := StrPas(bladerf_strerror(Status));
      Memo.Lines.Add('Failed to stop RX: ' + S);
    end;
  end;
end;

procedure TForm1.StartTxBtnClick(Sender: TObject);
var Status : Integer;
    S : String;
begin
  if FDeviceThread = nil then Exit;

  if StartTxBtn.Tag = 0 then begin
    if AllBox.Checked then
      Memo.Lines.Add('Starting TX ... ');
    PostMsg(ToDevMsg_TxStart, 0, 0);
    Exit;

    Status := FDeviceThread.TxStartSync;

    if Status = 0 then begin
      StartTxBtn.Tag := 1;
      StartTxBtn.Caption := 'Stop TX';
      if AllBox.Checked then
        Memo.Lines.Add('TX started');
      FDeviceThread.Resume;
    end
    else if DebugBox.Checked then begin
      S := StrPas(bladerf_strerror(Status));
      Memo.Lines.Add('Failed to start TX: ' + S);
    end;
  end
  else
  begin
    Timer1.Enabled := False;
    if AllBox.Checked then
      Memo.Lines.Add('Stopping TX ... ');
    PostMsg(ToDevMsg_TxStop, 0, 0);
  end;
end;




procedure TForm1.OnMouseWheel(var Message: TMessage);
var i : Integer;
    zDelta : Short;
    F : PFreqLabelDescr;
    FMult : Cardinal;
begin
  if (FRxLabelPtr = nil) and (FTxLabelPtr = nil) then Exit;

  zDelta := HiWord(Message.WParam);
  if FRxLabelPtr <> nil then begin
    i := FRxLabelsList.IndexOf(FRxLabelPtr);
    if i <> -1 then
    begin
      F := FRxLabelsDescr.Items[i];
      FMult := 1;
      for i := 0 to F^.Digit - 1 do
        FMult := FMult * 10;
      if zDelta > 0 then
        inc(FRxFreq, FMult)
      else begin
        if FMult > FRxFreq then
          FRxFreq := 237500
        else
          dec(FRxFreq, FMult);
      end;
      if FRxFreq < 237500 then
        FRxFreq := 237500;
      if FRxFreq > 3799999 then
        FRxFreq := 3799999;
      UpdateRxFreqLabels;
      SetRxFreq;
    end;
  end
  else begin
    i := FTxLabelsList.IndexOf(FTxLabelPtr);
    if i <> -1 then
    begin
      F := FTxLabelsDescr.Items[i];
      FMult := 1;
      for i := 0 to F^.Digit - 1 do
        FMult := FMult * 10;
      if zDelta > 0 then
        inc(FTxFreq, FMult)
      else begin
        if FMult > FTxFreq then
          FTxFreq := 237500
        else
          dec(FTxFreq, FMult);
      end;
      if FTxFreq < 237500 then
        FTxFreq := 237500;
      if FTxFreq > 3799999 then
        FTxFreq := 3799999;
      UpdateTxFreqLabels;
      SetTxFreq;
    end;
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

procedure TForm1.TxLabel_GHz_1MouseEnter(Sender: TObject);
begin
  FTxLabelPtr := TRxLabel(Sender);
end;

procedure TForm1.TxLabel_GHz_1MouseLeave(Sender: TObject);
begin
  FTxLabelPtr := nil;
end;







///////////////////////////////////////////////////////
//////                                           //////
//////       Device events                       //////
//////                                           //////
///////////////////////////////////////////////////////


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
  //GetRxFreq;

  FRxFreq := IniFile.ReadInteger('Blade_RX', 'CenterFreq', 435900);
  UpdateRxFreqLabels;
  SetRxFreq;

  RxSrSlider.Value := IniFile.ReadInteger('Blade_RX', 'SampleRate', 40);
  GetRxSr;
  RxBwSlider.Value := IniFile.ReadInteger('Blade_RX', 'BandWidth', 40);
  GetRxBW;
  RxLnaSlider.Value := IniFile.ReadInteger('Blade_RX', 'LNA', 3);
  GetRxLna;
  RxVga1Slider.Value := IniFile.ReadInteger('Blade_RX', 'VGA1', 5);
  GetRxVga1;
  RxVga2Slider.Value := IniFile.ReadInteger('Blade_RX', 'VGA2', 0);
  GetRxVga2;

  FTxFreq := IniFile.ReadInteger('Blade_TX', 'CenterFreq', 435900);
  UpdateTxFreqLabels;
  SetTxFreq;

  TxSrSlider.Value := IniFile.ReadInteger('Blade_TX', 'SampleRate', 40);
  GetTxSr;
  TxBwSlider.Value := IniFile.ReadInteger('Blade_TX', 'BandWidth', 40);
  GetTxBW;     
  TxVga1Slider.Value := IniFile.ReadInteger('Blade_TX', 'VGA1', 5);
  GetTxVga1;
  TxVga2Slider.Value := IniFile.ReadInteger('Blade_TX', 'VGA2', 0);
  GetTxVga2;

  OpenDevBtn.Enabled := True;
  OpenDevBtn.Caption := 'Disconnect';
  OpenDevBtn.Tag := 1;
  StartRxBtn.Enabled := True;
  StartTxBtn.Enabled := True;
  RxBox.Enabled := True;
  TxBox.Enabled := True;
end;

procedure TForm1.OnDisconnected(var Msg: TMessage);
begin
  if AllBox.Checked then
    Memo.Lines.Add('Disconnected');
  OpenDevBtn.Enabled := True;
  OpenDevBtn.Caption := 'Connect';
  OpenDevBtn.Tag := 0;
  StartRxBtn.Enabled := False;
  StartRxBtn.Tag := 0;
  StartRxBtn.Caption := 'Start RX';
  StartTxBtn.Enabled := False;
  StartTxBtn.Tag := 0;
  StartTxBtn.Caption := 'Start TX';
  RxBox.Enabled := False;
  TxBox.Enabled := False;

  IniFile.WriteInteger('Blade_RX', 'CenterFreq', FRxFreq);
  IniFile.WriteInteger('Blade_RX', 'SampleRate', RxSrSlider.Value);
  IniFile.WriteInteger('Blade_RX', 'BandWidth', RxBwSlider.Value);
  IniFile.WriteInteger('Blade_RX', 'LNA', RxLnaSlider.Value);
  IniFile.WriteInteger('Blade_RX', 'VGA1', RxVga1Slider.Value);
  IniFile.WriteInteger('Blade_RX', 'VGA2', RxVga2Slider.Value);

  IniFile.WriteInteger('Blade_TX', 'CenterFreq', FTxFreq);
  IniFile.WriteInteger('Blade_TX', 'SampleRate', TxSrSlider.Value);
  IniFile.WriteInteger('Blade_TX', 'BandWidth', TxBwSlider.Value);
  IniFile.WriteInteger('Blade_TX', 'VGA1', TxVga1Slider.Value);
  IniFile.WriteInteger('Blade_TX', 'VGA2', TxVga2Slider.Value);
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
    StopAudio;
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



procedure TForm1.OnTxStart(var Msg: TMessage);
var S : String;
begin
  if Msg.WParam = 0 then begin
    StartTxBtn.Tag := 1;
    StartTxBtn.Caption := 'Stop TX';
    if AllBox.Checked then
      Memo.Lines.Add('TX started');
    Timer1.Enabled := True;
    //if FTxBuffer = nil then begin
    //  FTxSamplesLen := 512*1000; //* May be any (reasonable) size */
    //  FTxBufferSize := FTxSamplesLen*SizeOf(SmallInt)*2; //* May be any (reasonable) size */
    //  GetMem(FTxBuffer, FTxBufferSize);
    //end;
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(Msg.WParam));
    Memo.Lines.Add('Failed to start TX: ' + S);
  end;
end;

procedure TForm1.OnTxStop(var Msg: TMessage);
var S : String;
begin
  if Msg.WParam = 0 then begin
    StartTxBtn.Tag := 0;
    StartTxBtn.Caption := 'Start TX';
    if AllBox.Checked then
      Memo.Lines.Add('TX stopped');
    //if FTxBuffer <> nil then begin
    //  FreeMem(FTxBuffer);
    //  FTxBuffer := nil;
    //end;
  end
  else if DebugBox.Checked then begin
    S := StrPas(bladerf_strerror(Msg.WParam));
    Memo.Lines.Add('Failed to stop TX: ' + S);
  end;
end;







procedure TForm1.OnRxError(var Msg: TMessage);
var S : String;
begin
  S := StrPas(bladerf_strerror(Msg.WParam));
  Memo.Lines.Add('Failed receiving Data: ' + S);
end;

procedure TForm1.OnTxError(var Msg: TMessage);
var S : String;
begin
  S := StrPas(bladerf_strerror(Msg.WParam));
  Memo.Lines.Add('Failed transmitting Data: ' + S);
end;


procedure TForm1.OnRxData(var Msg: TMessage);
var i : Integer;
    n : Cardinal;

    Buffer : PSmallInt;
    //BufferPtr : PSmallInt;
    //BufferTmpPtr : PSmallInt;

    samples_len_in : UINT;
    //samples_len_out : UINT;
    //Val : Smallint;

    WBufferPtr : PWCplx;

    BufferRe : PSmallInt;
    BufferRePtr : PSmallInt;
    BufferIm : PSmallInt;
    BufferImPtr : PSmallInt;

    BufferDRe : PDCplx;
    BufferDIm : PDCplx;
    BufferDRePtr : PDCplx;
    BufferDImPtr : PDCplx;

    HotBufferPtr : PSmallInt;
    //CValue : TWCplx;
begin
  Buffer := PSmallInt(Msg.WParam);
  //BufferPtr := Buffer;
  //BufferTmpPtr := Buffer;


  samples_len_in := Msg.LParam div 4;
  //samples_len_out := samples_len_in div 2;

  WBufferPtr := PWCplx(Buffer);
  GetMem(BufferDRe, 2048*SizeOf(TDCplx));
  GetMem(BufferDIm, 2048*SizeOf(TDCplx));
  BufferDRePtr := BufferDRe;
  BufferDImPtr := BufferDIm;

  i := 0;
  while i < 2048 do
  begin
    BufferDRePtr^.Re := WBufferPtr^.Re + Int(ReCorrectionEdit.Value);
    BufferDRePtr^.Im := WBufferPtr^.Im + Int(ImCorrectionEdit.Value);
    BufferDImPtr^.Re := WBufferPtr^.Im + Int(ImCorrectionEdit.Value);
    BufferDImPtr^.Im := WBufferPtr^.Re + Int(ReCorrectionEdit.Value);

    inc(BufferDRePtr);
    inc(BufferDImPtr);
    inc(WBufferPtr);
    inc(i);
  end;

  if SpectrumRB.Checked then begin
    Fft.ProcessDCplx(BufferDRe, SpectrDCplxRe, 2048);
    Fft.ProcessDCplx(BufferDIm, SpectrDCplxIm, 2048);
    ReDraw(nil);
  end
  else
    ReDraw(BufferDRe);

  FreeMem(BufferDRe);
  FreeMem(BufferDIm);

  //if CB3.Checked then
  //begin
  //  FIR_High.Process(PChar(Buffer), samples_len_out);
  //  FIR_Low1.Process(PChar(Buffer), samples_len_out);
  //end;

  if HotBuffer = nil then begin
    GetMem(HotBuffer, Buffer_Size*2);
    HotBufferPtr := HotBuffer;
  end else begin
    HotBufferPtr := HotBuffer;
    inc(HotBufferPtr, HotBufferWritten div 2);
  end;

  GetMem(BufferRe, samples_len_in*SizeOf(SmallInt));
  GetMem(BufferIm, samples_len_in*SizeOf(SmallInt));
  BufferRePtr := BufferRe;
  BufferImPtr := BufferIm;
  SeparateChannels(Buffer, BufferRe, BufferIm, samples_len_in);
  //FIR_Low1.Process(PChar(BufferIm), samples_len_in);

  n := 0;
  //WBufferPtr := PWCplx(Buffer);
  while n < samples_len_in do begin
    //CValue := WBufferPtr^;
    //HotBufferPtr^ := CValue.Re; // + CValue.Im;
    //HotBufferPtr^ := Round(32767*sin(2*Pi*2000*Phase/48000));
    //Phase := Phase + 1;
    //inc(WBufferPtr, 100);

    HotBufferPtr^ := BufferRePtr^ + BufferImPtr^;
    inc(BufferRePtr, 100);
    inc(BufferImPtr, 100);

    inc(HotBufferPtr);

    inc(HotBufferWritten, 2);
    if HotBufferWritten >= Buffer_Size*2 then begin
      HotList.Add(HotBuffer);
      HotBufferWritten := 0;
      GetMem(HotBuffer, Buffer_Size*2);
      HotBufferPtr := HotBuffer;
      if HotList.Count > 4 then
        StartAudio;
    end;

    inc(n, 100);
  end;


  FreeMemory(Buffer);

  FreeMem(BufferRe);
  FreeMem(BufferIm);
end;







procedure TForm1.DebugBoxClick(Sender: TObject);
begin
  if not DebugBox.Checked then
    AllBox.Checked := False;
  AllBox.Enabled := DebugBox.Checked;
end;






procedure TForm1.FFTBoxPaint(Sender: TObject);
begin
  ReDraw(nil);
end;

procedure TForm1.ReDraw(SignalBuffer: PDCplx);
var r : TRect;
    i : Integer;
    BufferOut : PShort;
    BufferOutPtr : PShort;
    BufferInPtr : PDCplx;
    FZeroLine : Integer;
    F1, F2 : Double;

    FCenter : Integer;

    SignalBufferPtr : PDCplx;
begin
  r.Left    := 0;
  r.Top     := 0;
  r.Right   := FBitMap.Width;
  r.Bottom  := FBitMap.Height;
  FBitMap.Canvas.Brush.Color := clBlack;
  FBitMap.Canvas.FillRect(r);

  FBitMap.Canvas.Pen.Width := 1;

  if SignalBuffer <> nil then begin
    FZeroLine := FBitMap.Height div 2;
    FBitMap.Canvas.Pen.Color := clLime;
    FBitMap.Canvas.Pen.Style := psDot;
    FBitMap.Canvas.MoveTo(0, FZeroLine);
    FBitMap.Canvas.LineTo(FBitMap.Width, FZeroLine);
    FBitMap.Canvas.Pen.Style := psSolid;
    SignalBufferPtr := SignalBuffer;
    i := 0;
    FBitMap.Canvas.Pen.Color := clAqua;
    FBitMap.Canvas.MoveTo(0, FZeroLine);
    while i < FBitMap.Width do begin
      FBitMap.Canvas.LineTo(i, FZeroLine - Round(SignalBufferPtr^.Re));
      inc(SignalBufferPtr);
      inc(i, ZoomFactor);
    end;
    FBitMap.Canvas.Pen.Color := clYellow;
    SignalBufferPtr := SignalBuffer;
    i := 0;
    FBitMap.Canvas.MoveTo(0, FZeroLine);
    while i < FBitMap.Width do begin
      FBitMap.Canvas.LineTo(i, FZeroLine - Round(SignalBufferPtr^.Im));
      inc(SignalBufferPtr);
      inc(i, ZoomFactor);
    end;

    FFTBox.Canvas.Draw(0, 0, FBitMap);
    Exit;
  end;

  FBitMap.Canvas.Pen.Color := clAqua;
  FZeroLine := 0;
  FCenter := FBitMap.Width div 2;

  GetMem(BufferOut, 2048*SizeOf(Short));

  BufferOutPtr := BufferOut;
  BufferInPtr := SpectrDCplxRe;
  //inc(BufferInPtr, 2);
  for i := 0 to 512 - 1 do
  begin
    if BufferInPtr^.Re < 0 then
      BufferInPtr^.Re := BufferInPtr^.Re*-1;
    if BufferInPtr^.Im < 0 then
      BufferInPtr^.Im := BufferInPtr^.Im*-1;
    F1 := BufferInPtr^.Re + BufferInPtr^.Im;
    Inc(BufferInPtr);
    if BufferInPtr^.Re < 0 then
      BufferInPtr^.Re := BufferInPtr^.Re*-1;
    if BufferInPtr^.Im < 0 then
      BufferInPtr^.Im := BufferInPtr^.Im*-1;
    F2 := F1 + BufferInPtr^.Re + BufferInPtr^.Im;
    Inc(BufferInPtr);

    if F2 = 0 then
    begin
      BufferOutPtr^ := -70;
      if FZeroLine = 0 then
        FZeroLine := 290;
    end
    else begin
      BufferOutPtr^ := Round(20*Log10(F2/5000000)); //Round(F1 + F2) div 500;
      if FZeroLine = 0 then
        FZeroLine := 10 - BufferOutPtr^*4;
    end;
    Inc(BufferOutPtr);
    if F2 > FMax then
      FMax := F2;
  end;
  BufferOutPtr := BufferOut;
  //inc(BufferOutPtr, 10);

  FBitMap.Canvas.MoveTo(FCenter, FZeroLine);
  for i := FCenter to FBitMap.Width - 2 do
  begin
    FBitMap.Canvas.LineTo(i, 10 - BufferOutPtr^*4);
    inc(BufferOutPtr);
  end;

  BufferOutPtr := BufferOut;
  BufferInPtr := SpectrDCplxIm;
  for i := 0 to 515 - 1 do
  begin
    if BufferInPtr^.Re < 0 then
      BufferInPtr^.Re := BufferInPtr^.Re*-1;
    if BufferInPtr^.Im < 0 then
      BufferInPtr^.Im := BufferInPtr^.Im*-1;
    F1 := BufferInPtr^.Re + BufferInPtr^.Im;
    Inc(BufferInPtr);
    if BufferInPtr^.Re < 0 then
      BufferInPtr^.Re := BufferInPtr^.Re*-1;
    if BufferInPtr^.Im < 0 then
      BufferInPtr^.Im := BufferInPtr^.Im*-1;
    F2 := F1 + BufferInPtr^.Re + BufferInPtr^.Im;
    Inc(BufferInPtr);

    if F2 = 0 then
    begin
      BufferOutPtr^ := -70;
      if FZeroLine = 0 then
        FZeroLine := 290;
    end
    else begin
      BufferOutPtr^ := Round(20*Log10(F2/5000000));
      if FZeroLine = 0 then
        FZeroLine := 10 - BufferOutPtr^*4;
    end;
    Inc(BufferOutPtr);
  end;
  BufferOutPtr := BufferOut;

  FBitMap.Canvas.MoveTo(FCenter, FZeroLine);
  for i := FCenter downto 0 do
  begin
    FBitMap.Canvas.LineTo(i, 10 - BufferOutPtr^*4);
    inc(BufferOutPtr);
  end;

  FFTBox.Canvas.Draw(0, 0, FBitMap);
  FreeMem(BufferOut);

  Label1.Caption := FloatToStr(FMax);
end;




procedure TForm1.WaveOutStop(Sender: TObject);
begin
  if wosOpen in WaveOut.State then
    WaveOut.Close;
end;

procedure TForm1.WaveOutStart(Sender: TObject);
begin
  rxSliderVolumeChange(nil);
end;

procedure TForm1.WaveOutBufferReady(Sender: TObject; lpWaveHdr: PWaveHdr);
begin
//
end;

procedure TForm1.WaveOutBufferLoad(Sender: TObject; lpWaveHdr: PWaveHdr; var MoreBuffers: Boolean);
var
  //i : Integer;
  //j,a,a1,ll : Integer;
  //nSamples    : Integer;
  nRead       : Integer;
  //pDataRead   : PChar;
  //pDataOutL   : PChar;
  //pDataOutR   : PChar;

  //pData : PSmallInt;
  //k           : Integer;
  //P : ^SmallInt;
begin
  //nRead := 0;
  //GetMem(pDataRead, Buffer_Size);
  //GetMem(pDataOutL, Buffer_Size*2);
  //GetMem(pDataOutR, Buffer_Size*2);

  if HotList.Count > 0 then begin
    CopyMemory(lpWaveHdr^.lpData, HotList.Items[0], lpWaveHdr^.dwBufferLength);
    FreeMem(HotList.Items[0]);
    HotList.Delete(0);
  end else begin
    ZeroMemory(lpWaveHdr^.lpData, lpWaveHdr^.dwBufferLength);
  end;
  nRead := Buffer_Size*2;

  {
  pData := PSmallInt(lpWaveHdr^.lpData);
  nSamples := Buffer_Size;
  i := 0;
  while i < nSamples do
  begin
    //pData^ := Random(512) - 255;
    pData^ := Round(32767*sin(2*Pi*2000*Phase/48000));
    inc(pData);
    inc(i);
    Phase := Phase + 1;
  end;
  }
  {
  if HotQueue.Count <> 0 then
  begin
    CopyMemory(pDataRead, HotQueue.Items[0], Buffer_Size);
    FreeMem(HotQueue.Items[0]);
    HotQueue.Delete(0);
  end;
  }
  MoreBuffers := True;

  {
  if ptrplay.ARU_ON_OFF then
  begin
    if ptrplay.ARUSN = 100 then a1 := 1
    else a1 := trunc(327.67*(100 - ptrplay.ARUSN));
    ll := trunc(327.67*ptrplay.ARUN);
    for i := 0 to 7 do
    begin
      a := a1;
      for j := 0 to 255 do
        if abs(PMyArray(pDataRead)^[i*256 + j]) > a then a := abs(PMyArray(pDataRead)^[i*256 + j]);
        for j := 0 to 255 do PMyArray(pDataRead)^[i*256 + j] := trunc((PMyArray(pDataRead)^[i*256 + j])/a*ll);
    end;
  end;
  k := Converter.Convert(pDataRead, Buffer_Size, pDataOutL, Buffer_Size*2);
  if not PreSet0.Checked then
  begin
    FFTFilter.Process(pDataOutL, Buffer_Size);
    FFTFilter.Process(Pointer(Integer(pDataOutL) + Buffer_Size), Buffer_Size);
    //FFTFilter.Process(pDataOutL, k div 2);
    //FFTFilter.Process(Pointer(Integer(pDataOutL) + k), k div 2);
  end;
  }

  {
  if nRead > 0 then
  begin
    CopyMemory(pDataOutR, pDataOutL, Buffer_Size*2);
    //Phaser.Process(pDataOutR, Buffer_Size*2);
    MergeChannels(lpWaveHdr^.lpData, pDataOutL, pDataOutR, Buffer_Size);
  end;
  //Equalizer.Process(@BufOut_16, Buffer_Size*4);
  }

  if Osc.DrawSpectr then
    Osc.AddSignal(lpWaveHdr^.lpData^, Buffer_Size)
  else
    Osc.AddSignal(lpWaveHdr^.lpData^, Buffer_Size);


  lpWaveHdr^.dwBytesRecorded := nRead;
  //FreeMem(pDataRead);
  //FreeMem(pDataOutL);
  //FreeMem(pDataOutR);
end;

procedure TForm1.Audio_OutChange(Sender: TObject);
Var volume : LongInt ;
    left   : WORD;
begin
  WaveOut.DeviceID := Audio_Out.ItemIndex - 1;
  { определ€ем текущий уровень громкости }
  waveOutGetVolume(WaveOut.DeviceID, @volume);
  left := HIWORD(volume);
  rxSliderVolume.Value := rxSliderVolume.Value - (left div 2);
end;

procedure TForm1.rxSliderVolumeChange(Sender: TObject);
Var
  volume : longint ;
  left, right : WORD;
begin
  { измен€ем уровень громкости }
  { определ€ем текущий уровень громкости }
  waveOutGetVolume(WaveOut.DeviceID, @volume);
  left := rxSliderVolume.Value*2;
  right := rxSliderVolume.Value*2;
  volume := MakeLong(left, right);
  waveOutSetVolume(WaveOut.DeviceID, volume);
end;

procedure TForm1.StartAudio;
begin
  if not (wosOpen in WaveOut.State) then
    WaveOut.Open;
  if not (wosPlay in WaveOut.State) then
    WaveOut.Start;
end;

procedure TForm1.StopAudio;
begin
  if wosPlay in WaveOut.State then
    WaveOut.Stop;
  if wosOpen in WaveOut.State then
    WaveOut.Close;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
var FBuffer : PByte;
    FBufferPtr : PWCplx;
    FBufferSize : Integer;
    FSamplesLen : Integer;
    i : Integer;
begin
  Timer1.Enabled := False;

  if not FDeviceThread.CanSend then begin
    Timer1.Enabled := True;
    Exit;
  end;

  FSamplesLen := 8192;
  FBufferSize := FSamplesLen*SizeOf(TWCplx);
  GetMem(FBuffer, FBufferSize);
  FBufferPtr := PWCplx(FBuffer);
  i := 0;
  while i < FSamplesLen do begin
    FBufferPtr^.Re := Round(TxValueEdit.Value);
    FBufferPtr^.Im := 0;
    inc(i);
    inc(FBufferPtr);
  end;
  //ZeroMemory(FBuffer, FBufferSize);
  FDeviceThread.TxAddData(FBuffer, FBufferSize);
  Timer1.Enabled := True;
end;

procedure TForm1.ZoomInBtnClick(Sender: TObject);
begin
  ZoomFactor := ZoomFactor*2;
end;

procedure TForm1.ZoomOutBtnClick(Sender: TObject);
begin
  if ZoomFactor = 1 then Exit;
  ZoomFactor := ZoomFactor div 2;
end;

end.
