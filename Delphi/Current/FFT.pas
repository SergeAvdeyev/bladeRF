unit FFT;

interface

uses Windows, Classes, Math, SysUtils,
     NSP;


const
  //FFT_WIN_SIZE    = 2048;
  FFT_ORDER       = 11;
  //NumSpectrCoeffs = FFT_WIN_SIZE div 2;  // в два раза меньше чем FFT_WIN_SIZE

  DELTA_SIZE      = 2048;   // .. отсчетов
  Sample_Rate     = 22050;

  NumFilters      = 120;

type
  TSCplxArray = array [0..999999] of TSCplx;
  PSCplxArray = ^TSCplxArray;
  TWCplxArray = array [0..999999] of TWCplx;
  PWCplxArray = ^TWCplxArray;
  TShortArray = array [0..999999] of Short;
  PShortArray = ^TShortArray;
  TFloatArray = array [0..0] of Float;
  PFloatArray = ^TFloatArray;
  TDoubleArray= array [0..0] of Double;
  PDoubleArray= ^TDoubleArray;

  TSingleArray = array[0..0] of Single;
  PSingleArray = ^TSingleArray;

  TFFTFilterItem = record
    IdxMin  : Word;
    IdxMax  : Word;
    MidFreq : Word;
    GaindB  : SmallInt;
    GainReal: Real;
  end;

  TFFTFiltersArray = array[0..0] of TFFTFilterItem;
  PFFTFiltersArray = ^TFFTFiltersArray;

  TFFTFilter = class(TComponent)
  private
    InpP     : PFloatArray;
    FwdP     : PSCplxArray;
    //PowP     : PFloatArray;
    InvP     : PFloatArray;
    InList   : TList;
    OutList  : TList;
    //FSampleRate : Integer;

    FNumFilters : Integer;
    FFFTSize    : Integer;
    FWindowSize : Integer;
    function NewBuffer(Buffer: Pointer; BufferSize: Integer): Integer;
    procedure SetNumFilters(Value: Integer);
    procedure SetFFTSize(Value: Integer);
    procedure InitFFT;
    procedure FinishFFT;
    procedure MainCalc;
  public
    //Coeffs      : array[0..511] of Single;
    //SpectrKoef  : Integer;                // Если 1, то массив Coeffs управляет
                                          // всем спектром,
                                          // если 2 - только первой половиной
                                          // вторая половина зануляется
    Filters     : array of TFFTFilterItem;
    constructor Create(AOwner: TComponent); override;//(BufferSize: Integer; DeltaSize: Integer);
    destructor Destroy; override;
    procedure process(Buffer: Pointer; BufferSize: Integer);
    function TestFFTTime(NumIters: Integer): Single;
    //procedure SetFilter(Index: Integer; GaindB: SmallInt; MidFreq, Width: Integer);
    //function PeakFreq(Buffer: Pointer; BufferSize, FFTSize: Integer): Integer;
  published
    //property SampleRate: Integer read FSampleRate write FSampleRate;
    property NumFilters : Integer read FNumFilters write SetNumFilters;
    property FFTSize : Integer read FFFTSize write SetFFTSize;
  end;

  TFFT = class
  private
    InpP     : PFloatArray;
    FwdP     : PSCplxArray;
    //PowP     : PFloatArray;

    FFFTSize    : Integer;
    FWindowSize : Integer;
    procedure SetFFTSize(Value: Integer);
    procedure InitFFT;
    procedure FinishFFT;
    //procedure MainCalc;
  public
    constructor Create;
    destructor Destroy; override;
    procedure Process(BufferIn: Pointer; BufferOut: PFloat; BufferSize: Integer);
    //procedure ProcessCplx(BufferIn: Pointer; BufferOut: PSCplx; BufferSize: Integer);
    procedure ProcessWCplx(BufferIn: PWCplx; BufferOut: PWCplx; BufferSize: Integer);
    procedure ProcessDCplx(BufferIn: PDCplx; BufferOut: PDCplx; BufferSize: Integer);
    function TestFFTTime(NumIters: Integer): Single;
  published
    property FFTSize : Integer read FFFTSize write SetFFTSize;
  end;

  TSignalType = (st8000_8, st8000_16, st11025_8, st11025_16, st22050_8, st22050_16);
  TCompressType = (ctNone, ctALAW, ctMuLAW, cpMP3_5, cpMP3_10); // Для MP3 число показывает во сколько раз сжимать

  TConverter = class
  private
    Taps       : pDoubleArray;
    FirState   : PNSPFIRState;
    UpFactor,
    DownFactor : Integer;
    ResampKoef : Double;
    FInBits,
    FOutBits   : WORD;
    procedure ConverterFree;
  public
    constructor Create;
    destructor Destroy; override;
    procedure ConverterInit(InFreq, InBits, OutFreq, OutBits, Order: DWORD);
    function Convert(InBuf: Pointer; InBufLen: Integer; OutBuf: Pointer; OutBufLen: Integer): Integer;
  end;

  TConverter1 = class
  private
    FTaps       : pDoubleArray;
    FFirState   : PNSPFIRState;
    FUpFactor,
    FDownFactor : Integer;
    FResampCoef : Double;
    FInFreq,
    FOutFreq,
    FInBits,
    FOutBits   : WORD;
    FInCompressType,
    FOutCompressType  : TCompressType;
    procedure ConverterFree;
  public
    constructor Create;
    destructor Destroy; override;
    procedure ConverterInit(InFreq, InBits: DWORD; InCompressType: TCompressType; OutFreq, OutBits: DWORD; OutCompressType: TCompressType; Order: DWORD);
    function Convert(InBuf: Pointer; InBufLen: Integer; OutBuf: Pointer; OutBufLen: Integer): Integer;
  end;

  TPhaser = class(TComponent)
  private
    FDelay      : Integer;
    FSampleRate : Integer;
    FBuffer     : PChar;
    FBufferSize : Integer;
    FNumBits    : Integer;
    FEnabled    : Boolean;
    procedure SetDelay(Value: Integer);
    procedure SetSampleRate(Value: Integer);
    procedure SetNumBits(Value: Integer);
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Process(Buffer: PChar; BufferSize: Integer);
  published
    property Delay: Integer read FDelay write SetDelay;
    property SampleRate: Integer read FSampleRate write SetSampleRate;
    property NumBits : Integer read FNumBits write SetNumBits;
    property Enabled : Boolean read FEnabled write FEnabled;
  end;


  function FindCoefs(InFreq, OutFreq: Integer; var UpFactor, DownFactor: Integer; var DivCoef: Double): Boolean;

  // Конвертирует A-закон 8 бит в линейный вид 16 бит
  function ALawToLin(InBuf: PChar; InBufLen: Integer; OutBuf: PChar): Integer;
  // Конвертирует Mu-закон 8 бит в линейный вид 16 бит
  function MuLawToLin(InBuf: PChar; InBufLen: Integer; OutBuf: PChar): Integer;
  // Конвертирует линейный вид 16 бит в A-закон 8 бит
  function LinToALaw(InBuf: PChar; InBufLen: Integer; OutBuf: PChar): Integer;
  // Конвертирует линейный вид 16 бит в Mu-закон 8 бит
  function LinToMuLaw(InBuf: PChar; InBufLen: Integer; OutBuf: PChar): Integer;
  procedure Convert8to16(InBuf, OutBuf: Pointer; NumSamples: Integer);
  procedure Convert16to8(InBuf, OutBuf: Pointer; NumSamples: Integer);

  function Vect1AddVect2(Buf1, Buf2: PChar; BufLen: Integer): Integer;


  procedure SeparateChannels(Source, Left, Right: Pointer; NumSamplesInChannel: Integer);
  procedure MergeChannels(Dest, Left, Right: Pointer; NumSamplesInChannel: Integer);

  function GetPowerOfTwo(Arg: DWord): Integer;

implementation




constructor TFFTFilter.Create(AOwner: TComponent);
begin
  inherited;

  FFTSize := 11;
  NumFilters := 16;
  //FSampleRate := 22050;
  InitFFT;
  //FSampleRate := Sample_Rate;
  //SpectrKoef := 1;
end;

destructor TFFTFilter.Destroy;
begin
  FinishFFT;
  inherited;
end;

procedure TFFTFilter.InitFFT;
var B : Pointer;
begin

  InList := TList.Create;
  GetMem(B, FWindowSize*2);
  ZeroMemory(B, FWindowSize*2);
  InList.Add(B);
  GetMem(B, FWindowSize*2);
  ZeroMemory(B, FWindowSize*2);
  InList.Add(B);

  OutList := TList.Create;
  GetMem(B, FWindowSize*2);
  ZeroMemory(B, FWindowSize*2);
  OutList.Add(B);
  GetMem(B, FWindowSize*2);
  ZeroMemory(B, FWindowSize*2);
  OutList.Add(B);

  InpP := AllocMem(FWindowSize*SizeOf(Float));
  FwdP := AllocMem((FWindowSize div 2 + 1)*SizeOf(TSCplx));
  InvP := AllocMem(FWindowSize*SizeOf(Float));
  //PowP := AllocMem((FFT_WIN_SIZE div 2)*SizeOf(Float));
end;

procedure TFFTFilter.FinishFFT;
begin

  FreeMem(InpP);
  //FreeMem(PowP);
  FreeMem(FwdP);
  FreeMem(InvP);
  while InList.Count > 0 do
  begin
    FreeMem(InList.Items[0]);
    InList.Delete(0);
  end;
  InList.Destroy;
  while OutList.Count > 0 do
  begin
    FreeMem(OutList.Items[0]);
    OutList.Delete(0);
  end;
  OutList.Destroy;
end;


function TFFTFilter.TestFFTTime(NumIters: Integer): Single;
var
  TBegin : LongInt;
  TEnd   : LongInt;
  i      : Integer;
begin
  TBegin := GetTickCount;
  for i := 0 to NumIters - 1 do
  begin
    nspsWinHann(PFloat(InpP), FWindowSize);
    nspsRealFftNip(PFloat(InpP), PSCplx(FwdP), FFT_ORDER, NSP_Forw);
    nspsCcsFftNip(PSCplx(FwdP), PFloat(InvP), FFT_ORDER, NSP_Inv);
  end;
  TEnd := GetTickCount;
  Result := (TEnd - TBegin)/NumIters;
end;




procedure TFFTFilter.Process(Buffer: Pointer; BufferSize: Integer);
var i    : Integer;
    P0   : ^SmallInt;
    P1   : ^SmallInt;
    P2   : ^SmallInt;
    SFactor : Integer;
begin
  if (Buffer = nil) or (BufferSize <= 0) then Exit;
  NewBuffer(Buffer, BufferSize);
  nspsbIntToFloat(InList.Items[2], PFloat(InpP), BufferSize div 2, 16, NSP_NOFlags);
  MainCalc;
  nspsbFloatToInt(PFloat(InvP), OutList.Items[2], BufferSize div 2, 16, NSP_Round);

  nspsbIntToFloat(InList.Items[3], PFloat(InpP), BufferSize div 2, 16, NSP_NOFlags);
  MainCalc;
  nspsbFloatToInt(PFloat(InvP), OutList.Items[3], BufferSize div 2, 16, NSP_Round);

  P0 := Pointer(OutList.Items[0]);
  P1 := Pointer(OutList.Items[1]);
  P2 := Pointer(OutList.Items[2]);
  inc(P0, BufferSize div 4);

  SFactor := NSP_NO_SCALE;
  nspwbAdd2(PShort(P0), PShort(P1), BufferSize div 4, NSP_AUTO_SCALE, SFactor);
  inc(P1, BufferSize div 4);

  SFactor := NSP_NO_SCALE;
  nspwbAdd2(PShort(P2), PShort(P1), BufferSize div 4, NSP_AUTO_SCALE, SFactor);

  CopyMemory(Buffer, OutList.Items[1], BufferSize);
  for i := 0 to 1 do
  begin
    FreeMem(InList.Items[0]);
    InList.Delete(0);
    FreeMem(OutList.Items[0]);
    OutList.Delete(0);
  end;
end;

function TFFTFilter.NewBuffer(Buffer: Pointer; BufferSize: Integer): Integer;
var P  : ^SmallInt;
    P1 : ^SmallInt;
    TempBuf : Pointer;
begin
  Result := 0;

  GetMem(TempBuf, BufferSize);
  P := Pointer(InList.Items[1]);
  inc(P, FWindowSize div 2);
  P1 := Pointer(TempBuf);

  CopyMemory(P1, P, BufferSize div 2);
  inc(P1, BufferSize div 4);
  CopyMemory(P1, Buffer, BufferSize div 2);
  InList.Add(TempBuf);

  GetMem(TempBuf, BufferSize);
  CopyMemory(TempBuf, Buffer, BufferSize);
  InList.Add(TempBuf);

  GetMem(TempBuf, BufferSize);
  OutList.Add(TempBuf);
  GetMem(TempBuf, BufferSize);
  OutList.Add(TempBuf);

end;

procedure TFFTFilter.MainCalc;
var
  i, j : integer;
  //N : Integer;
begin
  nspsWinHann(PFloat(InpP), FWindowSize);
  nspsRealFftNip(PFloat(InpP), PSCplx(FwdP), FFT_ORDER, NSP_Forw);
  for i := 0 to FNumFilters - 1 do
    //if Filters[i].MidFreq > 0 then
    for j := Filters[i].IdxMin to Filters[i].IdxMax do
    begin
      FwdP^[j].Re := FwdP^[j].Re*(Filters[i].GainReal);
      FwdP^[j].Im := FwdP^[j].Im*(Filters[i].GainReal);
    end;
  nspsCcsFftNip(PSCplx(FwdP), PFloat(InvP), FFT_ORDER, NSP_Inv);
end;






{
procedure TFFTFilter.SetFilter(Index: Integer; GaindB: SmallInt; MidFreq, Width: Integer);
var dF : Real;
    Min, Max: Integer;
    FGain : Real;
begin
  Filters[Index].MidFreq := MidFreq;
  dF := (FSampleRate div 2)/(FWindowSize div 2);
  Min := MidFreq - Width div 2;
  if Min < 0 then
    Min := 0;
  Max := MidFreq + Width div 2;
  Filters[Index].IdxMin := Round(Min/dF);
  Filters[Index].IdxMax := Trunc(Max/dF);
  FGain := GaindB/20;

  Filters[Index].GainReal := Power(10, FGain);
  Filters[Index].GainReal := 1/Filters[Index].GainReal;
end;

function TFFTFilter.PeakFreq(Buffer: Pointer; BufferSize, FFTSize: Integer): Integer;
var F1 : PFloat;
    F2 : PSCplx;
    R  : Integer;
begin
  Result := -1;
  GetMem(F1, SizeOf(Float)*(BufferSize div 2));
  GetMem(F2, SizeOf(TSCplx)*(BufferSize div 4 + 1));
  try
    nspsbIntToFloat(Buffer, PFloat(F1), BufferSize div 2, 16, NSP_NOFlags);
    nspsRealFftNip(PFloat(F1), PSCplx(F2), FFTSize, NSP_Forw);
    nspcbPowerSpectr(PSCplx(F2), PFloat(F1), BufferSize div 4);
    nspsMaxExt(PFloat(F1), BufferSize div 4, R);
    Result := R;
  except
  end;
  FreeMem(F1);
  FreeMem(F2);
end;
}
procedure TFFTFilter.SetNumFilters(Value: Integer);
var i, j : Integer;
begin
  if Value <= 0 then Exit;
  FNumFilters := Value;

  SetLength(Filters, FNumFilters);
  j := FWindowSize div 2 div FNumFilters;
  for i := 0 to FNumFilters - 1 do
  begin
    Filters[i].IdxMin := i*j;
    Filters[i].IdxMax := Filters[i].IdxMin + j - 1;
  end;
end;

procedure TFFTFilter.SetFFTSize(Value: Integer);
begin
  if Value <= 0 then Exit;
  FFFTSize := Value;
  FWindowSize := Round(Power(2, FFFTSize));
end;










constructor TFFT.Create;
begin

  FFTSize := 11;
  InitFFT;
end;

destructor TFFT.Destroy;
begin
  FinishFFT;
  inherited;
end;

procedure TFFT.InitFFT;
begin
  InpP := AllocMem(FWindowSize*SizeOf(Float));
  FwdP := AllocMem((FWindowSize div 2 + 1)*SizeOf(TSCplx));
  //PowP := AllocMem(FWindowSize*SizeOf(Float));
end;

procedure TFFT.FinishFFT;
begin
  FreeMem(InpP);
  FreeMem(FwdP);
  //FreeMem(PowP);
end;


function TFFT.TestFFTTime(NumIters: Integer): Single;
var
  TBegin : LongInt;
  TEnd   : LongInt;
  i      : Integer;
begin
  TBegin := GetTickCount;
  for i := 0 to NumIters - 1 do
  begin
//    nspsWinHann(PFloat(InpP), FWindowSize);
//    nspsRealFftNip(PFloat(InpP), PSCplx(FwdP), FFT_ORDER, NSP_Forw);
//    nspsCcsFftNip(PSCplx(FwdP), PFloat(PowP), FFT_ORDER, NSP_Inv);
  end;
  TEnd := GetTickCount;
  Result := (TEnd - TBegin)/NumIters;
end;




procedure TFFT.Process(BufferIn: Pointer; BufferOut: PFloat; BufferSize: Integer);
begin
  if (BufferIn = nil) or (BufferSize <= 0) then Exit;
  nspsbIntToFloat(BufferIn, PFloat(InpP), BufferSize div 2, 16, NSP_NOFlags);

  nspsWinHann(PFloat(InpP), FWindowSize);
  nspsRealFftNip(PFloat(InpP), PSCplx(FwdP), FFT_ORDER, NSP_Forw);

  //nspcbMag(PSCplx(FwdP), BufferOut, FWindowSize div 2);
  nspcbPowerSpectr(PSCplx(FwdP), BufferOut, FWindowSize div 2);

  //MainCalc;
  //nspsbFloatToInt(PFloat(PowP), BufferOut, BufferSize div 2, 16, NSP_Round);
end;

procedure TFFT.ProcessWCplx(BufferIn: PWCplx; BufferOut: PWCplx; BufferSize: Integer);
var S : Integer;
begin
  if (BufferIn = nil) or (BufferSize <= 0) then Exit;
  //nspsbIntToFloat(BufferIn, PFloat(InpP), BufferSize div 2, 16, NSP_NOFlags);

  //nspvWinHann(BufferIn, FWindowSize);
  S := 2;
  nspvFftNip(BufferIn, BufferOut, FFT_ORDER, NSP_Forw, NSP_FIXED_SCALE, S);
end;

procedure TFFT.ProcessDCplx(BufferIn: PDCplx; BufferOut: PDCplx; BufferSize: Integer);
begin

  nspzWinHann(BufferIn, FWindowSize);
  nspzFftNip(BufferIn, BufferOut, FFT_ORDER, NSP_Forw);
end;

{
procedure TFFT.MainCalc;
//var
//  i, j : integer;
  //N : Integer;
begin
  nspsWinHann(PFloat(InpP), FWindowSize);
  nspsRealFftNip(PFloat(InpP), PSCplx(FwdP), FFT_ORDER, NSP_Forw);

  //nspcbMag(PSCplx(FwdP), PFloat(PowP), FWindowSize div 2);
  nspcbPowerSpectr(PSCplx(FwdP), PFloat(PowP), FWindowSize div 2);

//  for i := 0 to FNumFilters - 1 do
//    for j := Filters[i].IdxMin to Filters[i].IdxMax do
//    begin
//      FwdP^[j].Re := FwdP^[j].Re*(Filters[i].GainReal);
//      FwdP^[j].Im := FwdP^[j].Im*(Filters[i].GainReal);
//    end;
//  nspsCcsFftNip(PSCplx(FwdP), PFloat(InvP), FFT_ORDER, NSP_Inv);
end;
}




procedure TFFT.SetFFTSize(Value: Integer);
begin
  if Value <= 0 then Exit;
  FFFTSize := Value;
  FWindowSize := Round(Power(2, FFFTSize));
end;







constructor TConverter.Create;
begin
  inherited;
end;

destructor TConverter.Destroy;
begin
  ConverterFree;
  inherited;
end;

procedure TConverter.ConverterInit(InFreq, InBits, OutFreq, OutBits, Order: DWORD);
begin
  ConverterFree;
  if (InFreq = 200000) and (OutFreq = 44100) then
  begin
    UpFactor   := 11;
    DownFactor := 50;
    ResampKoef := 0.22;
  end;

  if (InFreq = 8000) and (OutFreq = 11025) then
  begin
    UpFactor   := 11;
    DownFactor := 8;
    ResampKoef := 1.375;
  end;
  if (InFreq = 8000) and (OutFreq = 22050) then
  begin
    UpFactor   := 22;
    DownFactor := 8;
    ResampKoef := 2.75;
  end;
  if (InFreq = 11025) and (OutFreq = 22050) then
  begin
    UpFactor   := 4;
    DownFactor := 2;
    ResampKoef := 2*(OutBits/InBits);
  end;
  if (InFreq = 16000) and (OutFreq = 22050) then
  begin
    UpFactor   := 11;
    DownFactor := 8;
    ResampKoef := 1.375;
  end;
  if (InFreq = 22050) and (OutFreq = 11025) then
  begin
    UpFactor   := 2;
    DownFactor := 4;
    ResampKoef := 0.5*(OutBits/InBits);
  end;
  if (InFreq = 11025) and (OutFreq = 8000) then
  begin
    UpFactor   := 8;
    DownFactor := 11;
    ResampKoef := 8/11*(OutBits/InBits);
  end;
  FInBits  := InBits;
  FOutBits := OutBits;

  GetMem(Taps, sizeof(Double)*Order);
  nspdFirLowpass(1/(2*UpFactor), pDouble(Taps), Order, NSP_WinHamming, 0);
  GetMem(FirState, SizeOf(TNSPFIRState));
  nspdFirInitMr(pDouble(Taps), Order, nil, UpFactor, 0, DownFactor, 0, FirState^);
end;

procedure TConverter.ConverterFree;
begin
  if FirState <> nil then
    nspFirFree(FirState^);
  if Taps <> nil then
    FreeMem(Taps);
end;

function TConverter.Convert(InBuf: Pointer; InBufLen: Integer;
                                      OutBuf: Pointer; OutBufLen: Integer): Integer;
var Src, Dst: PDouble;
    FInSamps,
    FOutSamps : Integer;
    PIn : ^Double;
    POut: ^Byte;
    i : Integer;
begin
  FInSamps  := (InBufLen div 2)*(16 div FInBits);
  FOutSamps := (OutBufLen div 2)*(16 div FOutBits);
  GetMem(Src, SizeOf(Double)*FInSamps);
  GetMem(Dst, SizeOf(Double)*FOutSamps);
  if FInBits = 8 then
  begin
    POut := Pointer(InBuf);
    PIn := Pointer(Src);
    i := 0;
    while i < FInSamps do
    begin
      PIn^ := POut^*256 - 32767;
      inc(PIn);
      inc(POut);
      inc(i);
    end;
  end
  else
    nspdbIntToFloat(InBuf, Src, FInSamps, 16, NSP_NOFLAGS);
  if UpFactor > 2 then
    nspdbMpy1(UpFactor - 1, Src, FInSamps)
  else
    nspdbMpy1(UpFactor, Src, FInSamps);

  if FInSamps >= DownFactor then
    nspdbFir(FirState^, Src, Dst, FInSamps div DownFactor);
  if FOutBits = 8 then
  begin
    PIn := Pointer(Dst);
    POut := Pointer(OutBuf);
    i := 0;
    while i < FOutSamps do
    begin
      POut^ := Round(PIn^) div 256 + 127;
      inc(PIn);
      inc(POut);
      inc(i);
    end;
  end
  else
    nspdbFloatToInt(Dst, OutBuf, FOutSamps, 16, NSP_TRUNCNEG);
  FreeMem(Src);
  FreeMem(Dst);
  Result := Round(InBufLen*ResampKoef);
end;


constructor TConverter1.Create;
begin
  inherited;

  FFirState := nil;
  FTaps := nil;
end;

destructor TConverter1.Destroy;
begin
  ConverterFree;
  inherited;
end;

function FindCoefs(InFreq, OutFreq: Integer; var UpFactor, DownFactor: Integer; var DivCoef: Double): Boolean;
var F : Single;
    FMin, FMax : Single;
    Delta : Single;
    i, j : Integer;
    k : Single;
    N : Single;
begin
  F := InFreq/OutFreq;
  DivCoef := 1/F;
  Result := False;
  N := 0.01;
  while true do
  begin
    Delta := F/100*N;
    FMin := F - Delta;
    FMax := F + Delta;
    for i := 1 to 20 do
    begin
      for j := 1 to 20 do
      begin
        k := i/j;
        if (k >= FMin) AND (k <= FMax) {AND (i mod 2 = 0)} then
        begin
          UpFactor   := j;
          DownFactor := i;
          Result := True;
          Break;
        end;
      end;
      if Result then Break;
    end;
    N := N + 0.01;
    if (N >= 10) or (Result) then
      Break;
  end;
  DivCoef := UpFactor/DownFactor;
end;

procedure TConverter1.ConverterInit(InFreq, InBits: DWORD; InCompressType: TCompressType; OutFreq, OutBits: DWORD; OutCompressType: TCompressType; Order: DWORD);
begin
  ConverterFree;
  FInFreq  := InFreq;
  FOutFreq := OutFreq;
  FInBits  := InBits;
  FOutBits := OutBits;
  FInCompressType  := InCompressType;
  FOutCompressType := OutCompressType;
  if not FindCoefs(FInFreq, FOutFreq, FUpFactor, FDownFactor, FResampCoef) then Exit;
  GetMem(FTaps, sizeof(Double)*Order);
  //nspdFirLowpass(1/(2*FUpFactor), pDouble(FTaps), Order, NSP_WinHamming, 0);
  if FUpFactor < FDownFactor then
    //nspdFirLowpass((FOutFreq/2.167)/(FInFreq*FUpFactor), pDouble(FTaps), Order, NSP_WinHamming, 0)
    nspdFirLowpass((FOutFreq/2.0045)/(FInFreq*FUpFactor), pDouble(FTaps), Order, NSP_WinHamming, 0)
  else
    nspdFirLowpass((FInFreq/2.167)/(FInFreq*FUpFactor), pDouble(FTaps), Order, NSP_WinHamming, 0);
  //nspdFirLowpass((3000)/(FInFreq*FUpFactor), pDouble(FTaps), Order, NSP_WinHamming, 0);
  GetMem(FFirState, SizeOf(TNSPFIRState));
  nspdFirInitMr(pDouble(FTaps), Order, nil, FUpFactor, 0, FDownFactor, 0, FFirState^);
end;

procedure TConverter1.ConverterFree;
begin
  if FFirState <> nil then
  begin
    //nspFirFree(FFirState^);
    FreeMem(FFirState);
    FFirState := nil;
  end;
  if FTaps <> nil then
  begin
    FreeMem(FTaps);
    FTaps := nil;
  end;
end;

function TConverter1.Convert(InBuf: Pointer; InBufLen: Integer;
                                      OutBuf: Pointer; OutBufLen: Integer): Integer;
var Src, Dst: PDouble;
    FInSamps,
    FOutSamps : Integer;
    N : Integer;
    TempBuf : PChar;
    //PIn : ^Double;
    //POut : ^Byte;
    //i : Integer;
begin
  FInSamps  := (InBufLen div 2)*(16 div FInBits);
  FOutSamps := (OutBufLen div 2)*(16 div FOutBits);
  GetMem(Src, SizeOf(Double)*FInSamps);
  GetMem(Dst, SizeOf(Double)*FOutSamps);
  if FInBits = 8 then
  begin
    GetMem(TempBuf, SizeOf(SmallInt)*(FInSamps + 1));
    Convert8to16(InBuf, TempBuf, FInSamps);
    nspdbIntToFloat(TempBuf, Src, FInSamps, 16, NSP_NOFLAGS);
    FreeMem(TempBuf);
  end
  else
    nspdbIntToFloat(InBuf, Src, FInSamps, 16, NSP_NOFLAGS);
  if FUpFactor > 2 then
    nspdbMpy1(FUpFactor - 1, Src, FInSamps)
  else
    nspdbMpy1(FUpFactor, Src, FInSamps);

  N := FInSamps div FDownFactor;
  nspdbFir(FFirState^, Src, Dst, N);
  Result := N*FUpFactor;
  if FOutBits = 8 then
  begin

    GetMem(TempBuf, Result*SizeOf(SmallInt));
    nspdbFloatToInt(Dst, TempBuf, Result, 16, NSP_TRUNCNEG);
    Convert16to8(TempBuf, OutBuf, Result);
    FreeMem(TempBuf);

    {
    PIn := Pointer(Dst);
    POut := Pointer(OutBuf);
    i := 0;
    while i < FOutSamps do
    begin
      POut^ := Round(PIn^) div 256 + 127;
      inc(PIn);
      inc(POut);
      inc(i);
    end;
    }
  end
  else
  begin
    nspdbFloatToInt(Dst, OutBuf, FOutSamps, 16, NSP_TRUNCNEG);
    Result := Result*2;
  end;
  FreeMem(Src);
  FreeMem(Dst);
end;






constructor TPhaser.Create(AOwner: TComponent);
begin
  inherited;
  FSampleRate := 11025;
  FNumBits    := 16;
  Delay       := 10;
  FEnabled    := True;
end;

destructor TPhaser.Destroy;
begin
  if FBuffer <> nil then
    FreeMem(FBuffer);
  inherited;
end;

procedure TPhaser.SetDelay(Value: Integer);
begin
  if Value < 0 then
    FDelay := 0
  else
    FDelay := Value;
  FBufferSize := (FSampleRate div 1000)*(FNumBits div 8)*FDelay;
  if FBuffer = nil then
    GetMem(FBuffer, FBufferSize)
  else
    ReallocMem(FBuffer, FBufferSize);
  ZeroMemory(FBuffer, FBufferSize);
end;

procedure TPhaser.SetSampleRate(Value: Integer);
begin
  FSampleRate := Value;
  SetDelay(FDelay);
end;

procedure TPhaser.SetNumBits(Value: Integer);
begin
  FNumBits := Value;
  SetDelay(FDelay);
end;

procedure TPhaser.Process(Buffer: PChar; BufferSize: Integer);
var BufTemp : PChar;
    P1 : ^Byte;
    P2 : ^Byte;
begin
  if (FDelay <= 0) or (not FEnabled) then
    Exit;
  GetMem(BufTemp, BufferSize);
  P1 := Pointer(BufTemp);
  P2 := Pointer(Buffer);
  CopyMemory(P1, FBuffer, FBufferSize);
  inc(P1, FBufferSize);
  CopyMemory(P1, P2, BufferSize - FBufferSize);
  inc(P2, BufferSize - FBufferSize);
  CopyMemory(FBuffer, P2, FBufferSize);
  CopyMemory(Buffer, BufTemp, BufferSize);
  FreeMem(BufTemp);
end;





function ALawToLin(InBuf: PChar; InBufLen: Integer; OutBuf: PChar): Integer;
begin
  try
    nspwbALawToLin(PUChar(InBuf), PShort(OutBuf), InBufLen);
    Result := InBufLen*2;
  except
    Result := -1;
  end;
end;

function MuLawToLin(InBuf: PChar; InBufLen: Integer; OutBuf: PChar): Integer;
begin
  try
    nspwbMuLawToLin(PUChar(InBuf), PShort(OutBuf), InBufLen);
    Result := InBufLen*2;
  except
    Result := -1;
  end;
end;

function LinToALaw(InBuf: PChar; InBufLen: Integer; OutBuf: PChar): Integer;
begin
  try
    nspwbLinToALaw(PShort(InBuf), PUChar(OutBuf), InBufLen);
    Result := InBufLen div 2;
  except
    Result := -1;
  end;
end;

function LinToMuLaw(InBuf: PChar; InBufLen: Integer; OutBuf: PChar): Integer;
begin
  try
    nspwbLinToMuLaw(PShort(InBuf), PUChar(OutBuf), InBufLen);
    Result := InBufLen div 2;
  except
    Result := -1;
  end;
end;

procedure Convert8to16(InBuf, OutBuf: Pointer; NumSamples: Integer);
asm
           push     eax
           push     ebx
           push     ecx
           push     edx
           //push     esi
           //push     edi

   @Process_Loop:
           xor      ebx, ebx
           mov      bl,[eax]
           shl      ebx, $08
           sub      ebx, $00007fff
           mov      [edx], ebx

           inc      eax
           add      edx, 2

           dec      ecx
           jne      @Process_Loop

           //pop      edi
           //pop      esi
           pop      edx
           pop      ecx
           pop      ebx
           pop      eax
end;

procedure Convert16to8(InBuf, OutBuf: Pointer; NumSamples: Integer);
asm
           push     eax
           push     ebx
           push     ecx
           push     edx
           //push     esi
           //push     edi

   @Process_Loop:
           //xor      ebx, ebx
           mov      ebx,[eax]
           shr      ebx, $08
           add      bl, $7f
           mov      [edx], bl

           add      eax, 2
           inc      edx

           dec      ecx
           jne      @Process_Loop

           //pop      edi
           //pop      esi
           pop      edx
           pop      ecx
           pop      ebx
           pop      eax
end;

function Vect1AddVect2(Buf1, Buf2: PChar; BufLen: Integer): Integer;
var SFactor: Integer;
begin
  Result := 0;
  try
    SFactor := NSP_NO_SCALE;
    nspwbAdd2(PShort(Buf1), PShort(Buf2), BufLen div 2, NSP_AUTO_SCALE, SFactor);
  except
    Result := -1;
  end;
end;



procedure SeparateChannels(Source, Left, Right: Pointer; NumSamplesInChannel: Integer);
asm
           push     eax
           push     ebx
           push     ecx
           push     edx
           push     esi
           push     edi
           mov      ebx, NumSamplesInChannel

   @Process_Loop:
           mov      si,[eax]
           mov      di,[eax+2]

           mov      [edx],si
           mov      [ecx],di

           add      edx,2
           add      eax,4
           add      ecx,2

           dec      ebx
           jne      @Process_Loop

           pop      edi
           pop      esi
           pop      edx
           pop      ecx
           pop      ebx
           pop      eax

end;

procedure MergeChannels(Dest, Left, Right: Pointer; NumSamplesInChannel: Integer);
asm

           push     eax
           push     ebx
           push     ecx
           push     edx
           push     esi
           push     edi


           mov      ebx, NumSamplesInChannel
           test     ebx, ebx
           jz       @Process_End

   @Process_Loop:
           mov      si,[edx]
           mov      di,[ecx]
           mov      [eax],si
           add      eax, 2
           mov      [eax],di

           add      edx,2
           add      eax,2
           add      ecx,2

           dec      ebx
           jne      @Process_Loop

   @Process_End:
           pop      edi
           pop      esi
           pop      edx
           pop      ecx
           pop      ebx
           pop      eax

end;

function GetPowerOfTwo(Arg: DWord): Integer;
var i : Integer;
    Temp : Double;
begin
  Result := -1;
  if Arg = 0 then Exit;
  i := 0;
  Temp := Arg;
  while True do
  begin
    Temp := Temp / 2;
    if Frac(Temp) <> 0 then Break;
    if Temp = 1 then Break;
    inc(i);
  end;
  if Temp = 1 then
    Result := i;
end;



end.
