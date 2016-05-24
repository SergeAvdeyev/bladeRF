object Form1: TForm1
  Left = 1732
  Top = 14
  Width = 1099
  Height = 948
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
    Left = 512
    Top = 472
    Width = 32
    Height = 13
    Caption = 'Label1'
  end
  object Label19: TLabel
    Left = 760
    Top = 208
    Width = 73
    Height = 15
    Caption = #1042#1099#1074#1086#1076' '#1079#1074#1091#1082#1072
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -13
    Font.Name = 'Times New Roman'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 608
    Top = 268
    Width = 36
    Height = 13
    Caption = 'Re Corr'
  end
  object Label3: TLabel
    Left = 608
    Top = 292
    Width = 33
    Height = 13
    Caption = 'Im Corr'
  end
  object Label4: TLabel
    Left = 640
    Top = 444
    Width = 44
    Height = 13
    Caption = 'TX Value'
  end
  object ZoomInBtn: TSpeedButton
    Left = 944
    Top = 472
    Width = 23
    Height = 22
    Caption = '+'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnClick = ZoomInBtnClick
  end
  object ZoomOutBtn: TSpeedButton
    Left = 992
    Top = 472
    Width = 23
    Height = 22
    Caption = '-'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnClick = ZoomOutBtnClick
  end
  object Panel1: TPanel
    Left = 16
    Top = 256
    Width = 545
    Height = 209
    BevelInner = bvLowered
    Caption = 'Panel1'
    TabOrder = 0
  end
  object OpenDevBtn: TButton
    Left = 736
    Top = 16
    Width = 75
    Height = 25
    Caption = 'Connect'
    TabOrder = 1
    OnClick = OpenDevBtnClick
  end
  object Memo: TMemo
    Left = 840
    Top = 16
    Width = 233
    Height = 169
    ScrollBars = ssVertical
    TabOrder = 2
  end
  object StartRxBtn: TButton
    Left = 736
    Top = 48
    Width = 75
    Height = 25
    Caption = 'Start RX'
    Enabled = False
    TabOrder = 3
    OnClick = StartRxBtnClick
  end
  object RxBox: TGroupBox
    Left = 24
    Top = 8
    Width = 337
    Height = 217
    Caption = ' RX '
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 4
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
    object RxLabel1: TRxLabel
      Left = 184
      Top = 18
      Width = 16
      Height = 26
      AutoSize = False
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel2: TRxLabel
      Left = 198
      Top = 18
      Width = 16
      Height = 26
      AutoSize = False
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel3: TRxLabel
      Left = 212
      Top = 18
      Width = 16
      Height = 26
      AutoSize = False
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel4: TRxLabel
      Left = 8
      Top = 82
      Width = 29
      Height = 18
      AutoSize = False
      Caption = 'BW'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel5: TRxLabel
      Left = 8
      Top = 114
      Width = 29
      Height = 18
      AutoSize = False
      Caption = 'LNA'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel6: TRxLabel
      Left = 8
      Top = 146
      Width = 32
      Height = 18
      AutoSize = False
      Caption = 'VGA1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel7: TRxLabel
      Left = 8
      Top = 178
      Width = 32
      Height = 18
      AutoSize = False
      Caption = 'VGA2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel8: TRxLabel
      Left = 8
      Top = 54
      Width = 29
      Height = 18
      AutoSize = False
      Caption = 'SR'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxSrSlider: TRxSlider
      Left = 42
      Top = 45
      Width = 191
      Height = 26
      Increment = 1
      MinValue = 1
      TabOrder = 0
      Value = 1
      OnChange = RxSrSliderChange
    end
    object RxBwSlider: TRxSlider
      Left = 42
      Top = 76
      Width = 191
      Height = 26
      Increment = 1
      MinValue = 1
      TabOrder = 1
      Value = 1
      OnChange = RxBwSliderChange
    end
    object RxBwLabel: TCurrencyEdit
      Left = 240
      Top = 82
      Width = 73
      Height = 18
      AutoSize = False
      BorderStyle = bsNone
      Color = clBtnFace
      DisplayFormat = ',0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
    end
    object RxLnaSlider: TRxSlider
      Left = 42
      Top = 108
      Width = 191
      Height = 26
      Increment = 1
      MinValue = 1
      MaxValue = 3
      TabOrder = 3
      Value = 1
      OnChange = RxLnaSliderChange
    end
    object RxLnaLabel: TCurrencyEdit
      Left = 240
      Top = 114
      Width = 73
      Height = 18
      AutoSize = False
      BorderStyle = bsNone
      Color = clBtnFace
      DisplayFormat = ',0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 4
    end
    object RxVga1Slider: TRxSlider
      Left = 42
      Top = 140
      Width = 191
      Height = 26
      Increment = 1
      MinValue = 5
      MaxValue = 30
      TabOrder = 5
      Value = 5
      OnChange = RxVga1SliderChange
    end
    object RxVga1Label: TCurrencyEdit
      Left = 240
      Top = 146
      Width = 73
      Height = 18
      AutoSize = False
      BorderStyle = bsNone
      Color = clBtnFace
      DisplayFormat = ',0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 6
    end
    object RxVga2Slider: TRxSlider
      Left = 42
      Top = 172
      Width = 191
      Height = 26
      Increment = 3
      MaxValue = 30
      TabOrder = 7
      OnChange = RxVga2SliderChange
    end
    object RxVga2Label: TCurrencyEdit
      Left = 240
      Top = 178
      Width = 73
      Height = 18
      AutoSize = False
      BorderStyle = bsNone
      Color = clBtnFace
      DisplayFormat = ',0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 8
    end
    object RxSrLabel: TCurrencyEdit
      Left = 240
      Top = 50
      Width = 73
      Height = 18
      AutoSize = False
      BorderStyle = bsNone
      Color = clBtnFace
      DisplayFormat = ',0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 9
    end
  end
  object DebugBox: TCheckBox
    Left = 736
    Top = 120
    Width = 65
    Height = 17
    Caption = 'Debug'
    TabOrder = 5
    OnClick = DebugBoxClick
  end
  object AllBox: TCheckBox
    Left = 752
    Top = 144
    Width = 49
    Height = 17
    Caption = 'All'
    Enabled = False
    TabOrder = 6
  end
  object TxBox: TGroupBox
    Left = 376
    Top = 8
    Width = 337
    Height = 217
    Caption = ' TX '
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 7
    object TxLabel_MHz_100: TRxLabel
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
      OnMouseEnter = TxLabel_GHz_1MouseEnter
      OnMouseLeave = TxLabel_GHz_1MouseLeave
    end
    object TxLabel_MHz_10: TRxLabel
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
      OnMouseEnter = TxLabel_GHz_1MouseEnter
      OnMouseLeave = TxLabel_GHz_1MouseLeave
    end
    object TxLabel_GHz_1: TRxLabel
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
      OnMouseEnter = TxLabel_GHz_1MouseEnter
      OnMouseLeave = TxLabel_GHz_1MouseLeave
    end
    object TxLabel_MHz_1: TRxLabel
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
      OnMouseEnter = TxLabel_GHz_1MouseEnter
      OnMouseLeave = TxLabel_GHz_1MouseLeave
    end
    object TxLabel_kHz_100: TRxLabel
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
      OnMouseEnter = TxLabel_GHz_1MouseEnter
      OnMouseLeave = TxLabel_GHz_1MouseLeave
    end
    object TxLabel_kHz_10: TRxLabel
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
      OnMouseEnter = TxLabel_GHz_1MouseEnter
      OnMouseLeave = TxLabel_GHz_1MouseLeave
    end
    object TxLabel_kHz_1: TRxLabel
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
      OnMouseEnter = TxLabel_GHz_1MouseEnter
      OnMouseLeave = TxLabel_GHz_1MouseLeave
    end
    object RxLabel16: TRxLabel
      Left = 184
      Top = 18
      Width = 16
      Height = 26
      AutoSize = False
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel17: TRxLabel
      Left = 198
      Top = 18
      Width = 16
      Height = 26
      AutoSize = False
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel18: TRxLabel
      Left = 212
      Top = 18
      Width = 16
      Height = 26
      AutoSize = False
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel19: TRxLabel
      Left = 8
      Top = 82
      Width = 29
      Height = 18
      AutoSize = False
      Caption = 'BW'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel21: TRxLabel
      Left = 8
      Top = 146
      Width = 32
      Height = 18
      AutoSize = False
      Caption = 'VGA1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel22: TRxLabel
      Left = 8
      Top = 178
      Width = 32
      Height = 18
      AutoSize = False
      Caption = 'VGA2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object RxLabel23: TRxLabel
      Left = 8
      Top = 54
      Width = 29
      Height = 18
      AutoSize = False
      Caption = 'SR'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Layout = tlCenter
      ParentFont = False
      OnMouseEnter = RxLabel_GHz_1MouseEnter
      OnMouseLeave = RxLabel_GHz_1MouseLeave
    end
    object TxSrSlider: TRxSlider
      Left = 42
      Top = 45
      Width = 191
      Height = 26
      Increment = 1
      MinValue = 1
      TabOrder = 0
      Value = 1
      OnChange = TxSrSliderChange
    end
    object TxBwSlider: TRxSlider
      Left = 42
      Top = 76
      Width = 191
      Height = 26
      Increment = 1
      MinValue = 1
      TabOrder = 1
      Value = 1
      OnChange = TxBwSliderChange
    end
    object TxBwLabel: TCurrencyEdit
      Left = 240
      Top = 82
      Width = 73
      Height = 18
      AutoSize = False
      BorderStyle = bsNone
      Color = clBtnFace
      DisplayFormat = ',0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
    end
    object TxVga1Slider: TRxSlider
      Left = 42
      Top = 140
      Width = 191
      Height = 26
      Increment = 1
      MinValue = -35
      MaxValue = -4
      TabOrder = 3
      Value = -4
      OnChange = TxVga1SliderChange
    end
    object TxVga1Label: TCurrencyEdit
      Left = 240
      Top = 146
      Width = 73
      Height = 18
      AutoSize = False
      BorderStyle = bsNone
      Color = clBtnFace
      DisplayFormat = ',0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 4
    end
    object TxVga2Slider: TRxSlider
      Left = 42
      Top = 172
      Width = 191
      Height = 26
      Increment = 1
      MaxValue = 25
      TabOrder = 5
      OnChange = TxVga2SliderChange
    end
    object TxVga2Label: TCurrencyEdit
      Left = 240
      Top = 178
      Width = 73
      Height = 18
      AutoSize = False
      BorderStyle = bsNone
      Color = clBtnFace
      DisplayFormat = ',0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 6
    end
    object TxSrLabel: TCurrencyEdit
      Left = 240
      Top = 50
      Width = 73
      Height = 18
      AutoSize = False
      BorderStyle = bsNone
      Color = clBtnFace
      DisplayFormat = ',0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 7
    end
  end
  object Panel2: TPanel
    Left = 16
    Top = 496
    Width = 1026
    Height = 401
    BevelInner = bvLowered
    Caption = 'Panel2'
    TabOrder = 8
    object FFTBox: TPaintBox
      Left = 2
      Top = 2
      Width = 1022
      Height = 397
      Align = alClient
      OnPaint = FFTBoxPaint
    end
  end
  object ReCorrectionEdit: TRxSpinEdit
    Left = 656
    Top = 264
    Width = 57
    Height = 21
    Decimal = 3
    MaxValue = 100
    MinValue = -100
    TabOrder = 9
  end
  object ImCorrectionEdit: TRxSpinEdit
    Left = 656
    Top = 288
    Width = 57
    Height = 21
    Decimal = 3
    MaxValue = 100
    MinValue = -100
    TabOrder = 10
  end
  object Audio_Out: TComboBox
    Left = 856
    Top = 202
    Width = 217
    Height = 21
    HelpContext = 7110
    Style = csDropDownList
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ItemHeight = 13
    ParentFont = False
    TabOrder = 11
    OnChange = Audio_OutChange
    Items.Strings = (
      #1053#1077' '#1086#1087#1088#1077#1076#1077#1083#1077#1085#1086)
  end
  object rxSliderVolume: TRxSlider
    Left = 856
    Top = 233
    Width = 217
    Height = 28
    Hint = #1043#1088#1086#1084#1082#1086#1089#1090#1100
    HelpContext = 3150
    ImageHThumb.Data = {
      D6040000424DD6040000000000003604000028000000100000000A0000000100
      080000000000A000000000000000000000000001000000010000000000000000
      80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
      A600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000F0FBFF00A4A0A000808080000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00030000000000
      0000000000000000000300FF070707070707070707070707FF0000FF07070707
      0707070707070707FF0000FFF8F8F8F8F8F8F8F8F8F8F8F8FF0000FF07070707
      0707070707070707FF0000FFFFFFFFFFFFFFFFFFFFFFFFFFFF0000FF07070707
      0707070707070707FF0000FF070707070707070707070707FF0000FFFFFFFFFF
      FFFFFFFFFFFFFFFFFF0003000000000000000000000000000003}
    Increment = 2000
    MaxValue = 31744
    Options = [soShowPoints, soSmooth]
    ParentShowHint = False
    ShowHint = True
    TabOrder = 12
    Value = 31744
    OnChange = rxSliderVolumeChange
    UserImages = {01}
  end
  object StartTxBtn: TButton
    Left = 736
    Top = 80
    Width = 75
    Height = 25
    Caption = 'Start TX'
    Enabled = False
    TabOrder = 13
    OnClick = StartTxBtnClick
  end
  object SpectrumRB: TRadioButton
    Left = 944
    Top = 424
    Width = 89
    Height = 17
    Caption = 'Spectrum'
    Checked = True
    TabOrder = 14
    TabStop = True
  end
  object SignalRB: TRadioButton
    Left = 944
    Top = 448
    Width = 81
    Height = 17
    Caption = 'Signal'
    TabOrder = 15
  end
  object TxValueEdit: TRxSpinEdit
    Left = 696
    Top = 440
    Width = 57
    Height = 21
    Decimal = 3
    Increment = 10
    MaxValue = 2048
    MinValue = -2048
    TabOrder = 16
  end
  object WaveOut: TMMWaveOut
    SampleRate = sr16000
    BitRate = br16bits
    PlayMode = mMono
    BufferSize = 8192
    NumBuffers = 10
    DeviceID = 0
    OnBufferLoad = WaveOutBufferLoad
    OnBufferReady = WaveOutBufferReady
    OnStart = WaveOutStart
    OnStop = WaveOutStop
    Left = 904
    Top = 32
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 50
    OnTimer = Timer1Timer
    Left = 952
    Top = 32
  end
end
