object Form1: TForm1
  Left = 1827
  Top = 71
  Width = 955
  Height = 832
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
    Top = 256
    Width = 913
    Height = 313
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
    Top = 592
    Width = 425
    Height = 169
    ScrollBars = ssVertical
    TabOrder = 3
  end
  object GetRxFreqBtn: TButton
    Left = 584
    Top = 8
    Width = 75
    Height = 25
    Caption = 'GerRxFreq'
    Enabled = False
    TabOrder = 4
    OnClick = GetRxFreqBtnClick
  end
  object SetRxSRBtn: TButton
    Left = 672
    Top = 8
    Width = 75
    Height = 25
    Caption = 'SetRxSRBtn'
    Enabled = False
    TabOrder = 5
    OnClick = SetRxSRBtnClick
  end
  object StartRxBtn: TButton
    Left = 848
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Start RX'
    Enabled = False
    TabOrder = 6
    OnClick = StartRxBtnClick
  end
  object StopRxBtn: TButton
    Left = 848
    Top = 48
    Width = 75
    Height = 25
    Caption = 'Stop RX'
    Enabled = False
    TabOrder = 7
    OnClick = StopRxBtnClick
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
    TabOrder = 8
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
      MinValue = 1
      MaxValue = 40
      TabOrder = 0
      Value = 1
      OnChange = RxSrSliderChange
    end
    object RxBwSlider: TRxSlider
      Left = 42
      Top = 76
      Width = 191
      Height = 26
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
      Value = 20000000
    end
    object RxLnaSlider: TRxSlider
      Left = 42
      Top = 108
      Width = 191
      Height = 26
      MinValue = 1
      TabOrder = 3
      Value = 1
      OnChange = RxBwSliderChange
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
      Value = 20000000
    end
    object RxVga1Slider: TRxSlider
      Left = 42
      Top = 140
      Width = 191
      Height = 26
      MinValue = 1
      TabOrder = 5
      Value = 1
      OnChange = RxBwSliderChange
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
      Value = 20000000
    end
    object RxVga2Slider: TRxSlider
      Left = 42
      Top = 172
      Width = 191
      Height = 26
      MinValue = 1
      TabOrder = 7
      Value = 1
      OnChange = RxBwSliderChange
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
      Value = 20000000
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
      Value = 20000000
    end
  end
  object DebugBox: TCheckBox
    Left = 376
    Top = 16
    Width = 65
    Height = 17
    Caption = 'Debug'
    TabOrder = 9
    OnClick = DebugBoxClick
  end
  object AllBox: TCheckBox
    Left = 392
    Top = 40
    Width = 49
    Height = 17
    Caption = 'All'
    Enabled = False
    TabOrder = 10
  end
  object Timer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = TimerTimer
    Left = 688
    Top = 576
  end
end
