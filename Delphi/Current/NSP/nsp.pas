(*
       INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or
nondisclosure agreement with Intel Corporation and may not be copied
or disclosed except in accordance with the terms of that agreement.
  Copyright (c) 1998-2000 Intel Corporation. All Rights Reserved.

From:    nsp.h
Purpose: NSP Common Header file
*)

unit NSP;

{$Z+,A+}

interface

uses
  Windows;

type
  Float  = Single;
  PFloat = ^Float;
  Short  = SmallInt;
  PShort = ^Short;

  NSPStatus = Integer;

  PSCplx = ^TSCplx;
  TSCplx = record
    Re : Float;
    Im : Float;
  end;

  PDCplx = ^TDCplx;
  TDCplx = record
    Re : Double;
    Im : Double;
  end;

  PWCplx = ^TWCplx;
  TWCplx = record
    Re : Short;
    Im : Short;
  end;

  PICplx = ^TICplx;
  TICplx = record
    Re : Integer;
    Im : Integer;
  end;

const
  SCplxZero : TSCplx = (Re: 0; Im: 0);
  DCplxZero : TDCplx = (Re: 0; Im: 0);
  WCplxZero : TWCplx = (Re: 0; Im: 0);
  ICplxZero : TICplx = (Re: 0; Im: 0);

  SCplxOneZero : TSCplx = (Re: 1; Im: 0);
  DCplxOneZero : TDCplx = (Re: 1; Im: 0);

  WCplxOneOne : TWCplx = (Re: 1; Im: 1);
  ICplxOneOne : TICplx = (Re: 1; Im: 1);
  SCplxOneOne : TSCplx = (Re: 1; Im: 1);
  DCplxOneOne : TDCplx = (Re: 1; Im: 1);

const
  NSP_EPS  = 1.0e-12;
  NSP_PI   = 3.14159265358979324;
  NSP_2PI  = 6.28318530717958648;
  NSP_PI_2 = 1.57079632679489662;
  NSP_PI_4 = 0.785398163397448310;

function NSPsDegToRad(const Deg : Float) : Float;
function NSPdDegToRad(const Deg : Double) : Double;

{------------- NSP Library Control Symbols -------------}
{ Insert '$' before 'DEFINE' to use any Symbol below or }
{ 'DEFINE nsp_UsesAll' to use all.                      }

{$DEFINE nsp_UsesAll}
{DEFINE nsp_UsesVector}
(*
nsp_UsesVector:     vector/scalar Zero,Set,Copy,Add,Sub,Mpy,Div,Conj
                    b<,Inv>Tresh<1,2>,bAbs<1,2>,bAdd<2,3>,bExp<1,2>,bLn<1,2>,
                    bMpy<2,3>, bSqr<1,2>, bSqrt<1,2>, Max, Maxext, Min, Minext
                    Mean, StDev
                    Dotprod, Dotprodext,
                    bConj<,Extend><1,2>, bConjFlip2
                    b<Mu,A>LawToLin, bLinTo<Mu,A>Law
                    AutoCorr<Biased,Unbiased>, CrossCorr,
                    Norm, Normext, Normalize, bDiv<1,2,3>,
                    <Up,Down>Sample,bArctan<1,2>
*)

{DEFINE nsp_UsesConversion}
(*
nsp_UsesConversion: bImag, bReal, bCplxTo2Real, b2RealToCplx,
                    b<,r>Mag, b<,rPhase>,
                    bFloatTo<Int,Fix,S31Fix,S15Fix,S7Fix,S1516Fix>,
                    b<Int,Fix,S31Fix,S15Fix,S7Fix,S1516Fix>ToFloat,
                    b<,r>CartToPolar, b<,r>PolarToCart
*)

{DEFINE nsp_UsesConvolution}
(*
nsp_UsesConvolution:Conv
                    Conv2D
                    Filter2D
*)

{DEFINE nsp_UsesDct}
(*
nsp_UsesDct:        Dct
*)

{DEFINE nsp_UsesTransform}
(*
nsp_UsesTransform:  Dft
                    Fft<,Nip>, <Real,Ccs>Fftl<,Nip>, MpyRC<Perm,Pack><2,3>
                    <Real,Ccs><,2>Fft<,Nip>
                    Goertz
*)

{DEFINE nsp_UsesFir}
(*
nsp_UsesFir:        struct FirState,FirInit<,Mr>,FirFree,Fir<Get,Set><Taps,Dlyl>
                    struct Fir<Taps|Dly>State, <Up,Down>Sample,
                    FirlInit<,Mr,Dlyl>, Firl, Firl<Get,Set><Taps,Dlyl>
*)

{DEFINE nsp_UsesFirGen}
(*
nsp_UsesFirGen:     dFirLowpass, nspdFirHighpass, nspdFirBandpass
                    nspdFirBandstop
*)

{DEFINE nsp_UsesIir}
(*
nsp_UsesIir:        struct Iir<Tap,Dly>State, IirlInit<,Bq,Dlyl>, Iirl
                    struct Iir<Tap,Dly>State, IirInit<,Bq>, Iir, IirFree
*)

{DEFINE nsp_UsesLms}
(*
nsp_UsesLms:        struct Lms<Tap,Dly>State, LmslInit<,Mr,Dlyl>,
                    Lmsl<Get,Set><Step,Leak>, Lmsl<,Na>
*)

{DEFINE nsp_UsesMedian}
(*
nsp_UsesMedian:     MedianFilter1,MedianFilter2
*)

{DEFINE nsp_UsesMisc}
(*
nsp_UsesMisc:       BitRevTbl,BitRev,
                    TwdTbl
*)

{DEFINE nsp_UsesSampleGen}
(*
nsp_UsesSampleGen:  struct ToneState, ToneInit, Tone
                    struct TrnglState, TrnglInit, Trngl
                    bRandUni, RandUni, RandUniInit
                    bRandGauss, RandGauss, RandGausInit
*)

{DEFINE nsp_UsesWin}
(*
nsp_UsesWin:        Win<Bartlett,Hann,Hamming,Blackman<,Std,Opt>,Kaiser>
*)

{DEFINE nsp_UsesWavelet}
(*
nsp_UsesWavelet:    WtInit, WtInitLen, WtSetState, WtGetState,
                    WtDecompose, WtReconstruct
*)


{$INCLUDE NSPError.int}
{$INCLUDE NSPAlloc.int}

{$IFDEF nsp_UsesAll}
  {$DEFINE nsp_UsesConvolution}
  {$DEFINE nsp_UsesConversion}
  {$DEFINE nsp_UsesDct}
  {$DEFINE nsp_UsesFir}
  {$DEFINE nsp_UsesFirGen}
  {$DEFINE nsp_UsesIir}
  {$DEFINE nsp_UsesLms}
  {$DEFINE nsp_UsesMedian}
  {$DEFINE nsp_UsesMisc}
  {$DEFINE nsp_UsesSampleGen}
  {$DEFINE nsp_UsesTransform}
  {$DEFINE nsp_UsesVector}
  {$DEFINE nsp_UsesWin}
  {$DEFINE nsp_UsesWavelet}
{$ENDIF nsp_UsesAll}

{$IFDEF nsp_UsesConvolution}
  {$INCLUDE NSPConv.int}
  {$INCLUDE NSPCnv2d.int}
  {$INCLUDE NSPFir2.int}
{$ENDIF nsp_UsesConvolution}

{$IFDEF nsp_UsesConversion}
  {$INCLUDE NSPCvrt.int}
{$ENDIF nsp_UsesConversion}

{$IFDEF nsp_UsesDct}
  {$INCLUDE NSPDct.int}
{$ENDIF nsp_UsesDct}

{$IFDEF nsp_UsesFir}
  {$INCLUDE NSPFirl.int}
  {$INCLUDE NSPFirh.int}
  {$INCLUDE NSPRsmpl.int}
{$ENDIF nsp_UsesFir}

{$IFDEF nsp_UsesFirGen}
  {$INCLUDE NSPFirg.int}
{$ENDIF nsp_UsesFirGen}

{$IFDEF nsp_UsesIir}
  {$INCLUDE NSPIirl.int}
  {$INCLUDE NSPIirh.int}
{$ENDIF nsp_UsesIir}

{$IFDEF nsp_UsesLms}
  {$INCLUDE NSPLmsl.int}
  {$INCLUDE NSPLmsh.int}
{$ENDIF nsp_UsesLms}

{$IFDEF nsp_UsesMedian}
  {$INCLUDE NSPMed.int}
{$ENDIF nsp_UsesMedian}

{$IFDEF nsp_UsesMisc}
  {$INCLUDE NSPMisc.int}
{$ENDIF nsp_UsesMisc}

{$IFDEF nsp_UsesSampleGen}
  {$INCLUDE NSPRand.int}
  {$INCLUDE NSPTone.int}
  {$INCLUDE NSPTrngl.int}
{$ENDIF nsp_UsesSampleGen}

{$IFDEF nsp_UsesTransform}
  {$INCLUDE NSPFft.int}
  {$INCLUDE NSPGrtz.int}
{$ENDIF nsp_UsesTransform}

{$IFDEF nsp_UsesVector}
  {$INCLUDE NSPArith.int}
  {$INCLUDE NSPCorr.int}
  {$INCLUDE NSPLaw.int}
  {$INCLUDE NSPLnexp.int}
  {$INCLUDE NSPSampl.int}
  {$INCLUDE NSPVec.int}
  {$INCLUDE NSPDotp.int}
  {$INCLUDE NSPNorm.int}
  {$INCLUDE NSPLogic.int}
  {$INCLUDE NSPDiv.int}
  {$INCLUDE NSPAtan.int}
{$ENDIF nsp_UsesVector}

{$IFDEF nsp_UsesWin}
  {$INCLUDE NSPWin.int}
{$ENDIF nsp_UsesWin}

{$IFDEF nsp_UsesWavelet}
  {$INCLUDE NSPWlt.int}
{$ENDIF nsp_UsesWavelet}

implementation

const
  nspDLL = 'NSP.DLL';

function NSPsDegToRad(const Deg : Float) : Float;
begin
  Result := Deg / 180.0 * NSP_PI;
end;

function NSPdDegToRad(const Deg : Double) : Double;
begin
  Result := Deg / 180.0 * NSP_PI;
end;

{$INCLUDE NSPError.imp}
{$INCLUDE NSPAlloc.imp}

{$IFDEF nsp_UsesConvolution}
  {$INCLUDE NSPConv.imp}
  {$INCLUDE NSPCnv2d.imp}
  {$INCLUDE NSPFir2.imp}
{$ENDIF nsp_UsesConvolution}

{$IFDEF nsp_UsesConversion}
  {$INCLUDE NSPCvrt.imp}
{$ENDIF nsp_UsesConversion}

{$IFDEF nsp_UsesDct}
  {$INCLUDE NSPDct.imp}
{$ENDIF nsp_UsesDct}

{$IFDEF nsp_UsesFir}
  {$INCLUDE NSPFirl.imp}
  {$INCLUDE NSPFirh.imp}
  {$INCLUDE NSPRsmpl.imp}
{$ENDIF nsp_UsesFir}

{$IFDEF nsp_UsesFirGen}
  {$INCLUDE NSPFirg.imp}
{$ENDIF nsp_UsesFirGen}

{$IFDEF nsp_UsesIir}
  {$INCLUDE NSPIirl.imp}
  {$INCLUDE NSPIirh.imp}
{$ENDIF nsp_UsesIir}

{$IFDEF nsp_UsesLms}
  {$INCLUDE NSPLmsl.imp}
  {$INCLUDE NSPLmsh.imp}
{$ENDIF nsp_UsesLms}

{$IFDEF nsp_UsesMedian}
  {$INCLUDE NSPMed.imp}
{$ENDIF nsp_UsesMedian}

{$IFDEF nsp_UsesMisc}
  {$INCLUDE NSPMisc.imp}
{$ENDIF nsp_UsesMisc}

{$IFDEF nsp_UsesSampleGen}
  {$INCLUDE NSPRand.imp}
  {$INCLUDE NSPTone.imp}
  {$INCLUDE NSPTrngl.imp}
{$ENDIF nsp_UsesSampleGen}

{$IFDEF nsp_UsesTransform}
  {$INCLUDE NSPFft.imp}
  {$INCLUDE NSPGrtz.imp}
{$ENDIF nsp_UsesTransform}

{$IFDEF nsp_UsesVector}
  {$INCLUDE NSPArith.imp}
  {$INCLUDE NSPCorr.imp}
  {$INCLUDE NSPLaw.imp}
  {$INCLUDE NSPLnexp.imp}
  {$INCLUDE NSPSampl.imp}
  {$INCLUDE NSPVec.imp}
  {$INCLUDE NSPDotp.imp}
  {$INCLUDE NSPNorm.imp}
  {$INCLUDE NSPLogic.imp}
  {$INCLUDE NSPDiv.imp}
  {$INCLUDE NSPAtan.imp}
{$ENDIF nsp_UsesVector}

{$IFDEF nsp_UsesWin}
  {$INCLUDE NSPWin.imp}
{$ENDIF nsp_UsesWin}

{$IFDEF nsp_UsesWavelet}
  {$INCLUDE NSPWlt.imp}
{$ENDIF nsp_UsesWavelet}

end.
