object Form1: TForm1
  Left = 631
  Top = 149
  Width = 955
  Height = 741
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 600
    Top = 552
    Width = 32
    Height = 13
    Caption = 'Label1'
  end
  object Panel1: TPanel
    Left = 16
    Top = 192
    Width = 913
    Height = 281
    Caption = 'Panel1'
    TabOrder = 0
  end
  object Button1: TButton
    Left = 608
    Top = 576
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 1
    OnClick = Button1Click
  end
  object OpenDevBtn: TButton
    Left = 496
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Connect'
    TabOrder = 2
    OnClick = OpenDevBtnClick
  end
  object Memo: TMemo
    Left = 16
    Top = 480
    Width = 425
    Height = 209
    ScrollBars = ssVertical
    TabOrder = 3
  end
  object SetRxFreqBtn: TButton
    Left = 584
    Top = 8
    Width = 75
    Height = 25
    Caption = 'SetRxFreq'
    Enabled = False
    TabOrder = 4
    OnClick = SetRxFreqBtnClick
  end
  object GetRxFreqBtn: TButton
    Left = 584
    Top = 40
    Width = 75
    Height = 25
    Caption = 'GerRxFreq'
    Enabled = False
    TabOrder = 5
    OnClick = GetRxFreqBtnClick
  end
  object SetRxSRBtn: TButton
    Left = 672
    Top = 8
    Width = 75
    Height = 25
    Caption = 'SetRxSRBtn'
    Enabled = False
    TabOrder = 6
    OnClick = SetRxSRBtnClick
  end
  object SetRxBWBtn: TButton
    Left = 760
    Top = 8
    Width = 75
    Height = 25
    Caption = 'SetRxBWBtn'
    Enabled = False
    TabOrder = 7
    OnClick = SetRxBWBtnClick
  end
  object StartRxBtn: TButton
    Left = 848
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Start RX'
    Enabled = False
    TabOrder = 8
    OnClick = StartRxBtnClick
  end
  object StopRxBtn: TButton
    Left = 848
    Top = 48
    Width = 75
    Height = 25
    Caption = 'Stop RX'
    Enabled = False
    TabOrder = 9
    OnClick = StopRxBtnClick
  end
  object GroupBox1: TGroupBox
    Left = 24
    Top = 8
    Width = 281
    Height = 105
    Caption = ' RX '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 10
    object RxLabel_MHz_100: TRxLabel
      Left = 64
      Top = 16
      Width = 20
      Height = 26
      AutoSize = False
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel_MHz_10: TRxLabel
      Left = 82
      Top = 16
      Width = 20
      Height = 26
      AutoSize = False
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel_GHz_1: TRxLabel
      Left = 40
      Top = 16
      Width = 20
      Height = 26
      AutoSize = False
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel_MHz_1: TRxLabel
      Left = 100
      Top = 16
      Width = 20
      Height = 26
      AutoSize = False
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel_kHz_100: TRxLabel
      Left = 124
      Top = 16
      Width = 20
      Height = 26
      AutoSize = False
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel_kHz_10: TRxLabel
      Left = 142
      Top = 16
      Width = 20
      Height = 26
      AutoSize = False
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel_kHz_1: TRxLabel
      Left = 160
      Top = 16
      Width = 20
      Height = 26
      AutoSize = False
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
  end
  object Timer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = TimerTimer
    Left = 688
    Top = 576
  end
end
