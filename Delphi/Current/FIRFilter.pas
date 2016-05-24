unit FIRFilter;

interface

uses Windows, NSP;


const
  MAX_TAPS_LEN = 191;

type

  PFloatArray = ^TFloatArray;
  TFloatArray = array[0..MAX_TAPS_LEN - 1] of Float;

  TFIRFilterType = (FIRLowPass, FIRHighPass, FIRBandPass);
  TFIRFilter = class
  private
    FFIRState   : TNSPFirState;
    FSampleRate : Integer;
    FTapsLen    : Integer;
    FPhase      : Integer;
    procedure FIRDeInit;
  public
    Coeffs      : PFloatArray;
    constructor Create(SampleRate: Integer);
    destructor Destroy; override;

    procedure FIRInitLowPass(Freq, Order: Integer);
    procedure FIRInitHighPass(Freq, Order: Integer);

    procedure Process(Buffer: PChar; BufferSize: Integer);
  end;

//procedure FFTSpectr(InSamps: Pointer; InSize: Integer; OutSpectr: Pointer; OutSize: Integer



implementation


constructor TFIRFilter.Create(SampleRate: Integer);
begin
  inherited Create;
  FSampleRate := SampleRate;
end;

destructor TFIRFilter.Destroy;
begin
  FIRDeInit;
  inherited;
end;

type
  PDoubleArray = ^TDoubleArray;
  TDoubleArray = array[0..MAX_TAPS_LEN - 1] of Double;

procedure TFIRFilter.FIRInitLowPass(Freq, Order: Integer);
var RFreq : Double;
    FTaps : PDoubleArray;
    i     : Integer;
begin
  if Order > MAX_TAPS_LEN then Exit;
  FTapsLen := Order;
  GetMem(Coeffs, SizeOf(Float)*FTapsLen);
  ZeroMemory(Coeffs, SizeOf(Float)*FTapsLen);
  GetMem(FTaps, SizeOf(Double)*FTapsLen);
  ZeroMemory(FTaps, SizeOf(Double)*FTapsLen);
  RFreq := Freq/FSampleRate;
  NspdFirLowPass(RFreq, PDouble(FTaps), FTapsLen, NSP_WinBlackmanOpt,1);
  for i := 0 to FTapsLen - 1 do
    Coeffs^[i] := FTaps^[i];
  FreeMem(FTaps);
  NspwFIRInit(PFloat(Coeffs), FTapsLen, nil, FFIRState);
  FPhase := NSP_NO_SCALE;
end;

procedure TFIRFilter.FIRInitHighPass(Freq, Order: Integer);
var RFreq : Double;
    FTaps : PDoubleArray;
    i     : Integer;
begin
  if Order > MAX_TAPS_LEN then Exit;
  FTapsLen := Order;
  GetMem(Coeffs, SizeOf(Float)*FTapsLen);
  ZeroMemory(Coeffs, SizeOf(Float)*FTapsLen);
  GetMem(FTaps, SizeOf(Double)*FTapsLen);
  ZeroMemory(FTaps, SizeOf(Double)*FTapsLen);
  RFreq := Freq/FSampleRate;
  NspdFirHighPass(RFreq, PDouble(FTaps), FTapsLen, NSP_WinHann, 1);
  for i := 0 to FTapsLen - 1 do
    Coeffs^[i] := FTaps^[i];
  FreeMem(FTaps);
  NspwFIRInit(PFloat(Coeffs), FTapsLen, nil, FFIRState);
  FPhase := NSP_NO_SCALE;

end;

procedure TFIRFilter.Process(Buffer: PChar; BufferSize: Integer);
var TempBuf : PShort;
begin
  GetMem(TempBuf, BufferSize);
  NspwbFir(FFIRState, PShort(Buffer), TempBuf, BufferSize div 2, NSP_SATURATE, FPhase);
  CopyMemory(Buffer, TempBuf, BufferSize);
  FreeMem(TempBuf);
end;

procedure TFIRFilter.FIRDeInit;
begin
  NSPFIRFree(FFIRState);
  FreeMem(Coeffs);
end;





////////////////////////////////////////////////////////////////////////////////
////////////////////////    FFT  ///////////////////////////////////////////////


end.
