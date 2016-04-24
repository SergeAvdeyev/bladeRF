unit BladeRFThread;

interface

uses Windows, Sysutils, Messages, syncobjs,
  Classes, BladeRFIntf;

const
  ToDevMsg_Connect     = WM_USER + 1;
  ToDevMsg_Disconnect  = WM_USER + 2;
  ToDevMsg_RxSetFreq   = WM_USER + 3;


  FromDevMsg_Connected = WM_USER + 1;
  FromDevMsg_Disconnected = WM_USER + 2;

type
  TBladeThread = class(TThread)
  private
    hMainWnd         : THandle;
    FCriticalSection : TCriticalSection;

    FDevice : Pointer;
    procedure Lock;
    procedure Unlock;

    procedure SendErrorMessage(AStatus: Integer; AMsg: Dword);
    function OpenDevice : Integer;
    procedure CloseDevice;
    function SetRxFreq(AFreq: DWord): Integer;
    function SetRxSampleRate(ASampleRate: DWord): Integer;

  protected
    procedure Execute; override;

  public
    constructor Create(HWND: THandle);
    destructor Destroy; override;

  end;

implementation

constructor TBladeThread.Create(HWND: THandle);
begin
  FCriticalSection := TCriticalSection.Create;
  FreeOnTerminate := False;
  hMainWnd := HWND;
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

function TBladeThread.OpenDevice : Integer;
begin
  Result := 0;
  if FDevice <> nil then Exit;

  Result := bladerf_open(FDevice, nil);
end;

procedure TBladeThread.CloseDevice;
begin
  if FDevice = nil then Exit;

  bladerf_close(FDevice);
  FDevice := nil;
end;

procedure TBladeThread.SenderrorMessage(AStatus: Integer; AMsg: Dword);
begin
end;

function TBladeThread.SetRxFreq(AFreq: DWord): Integer;
begin
end;

function TBladeThread.SetRxSampleRate(ASampleRate: DWord): Integer;
begin
end;

procedure TBladeThread.Execute;
var msg : tagMSG;
    P : PChar;
    n : Cardinal;
    ErrorBuf : PChar;
    status, oldstatus : Integer;
    S : String;
begin
  oldstatus := 0;
  while not Terminated do
  begin
    if PeekMessage(msg, 0, 0, 0, PM_REMOVE) then
    begin
      case msg.message of
        ToDevMsg_Connect: begin
            // Команда на подключение
            if FDevice = nil then
            begin
              while True do
              begin
                status := OpenDevice;
                if status = 0 then break;
                if Terminated then break;
                if status <> oldstatus then
                begin
                  oldstatus := status;
                  P := bladerf_strerror(status);
                  S := StrPas(P);
                  n := Length(S) + 1;
                  GetMem(ErrorBuf, n);
                  CopyMemory(ErrorBuf, P, n);
                  SendMessage(hMainWnd, WM_USER + 3, Integer(ErrorBuf), n);
                end;
                Sleep(1000);
              end;
              if Terminated then break;
              SendMessage(hMainWnd, FromDevMsg_Connected, 0, 0);
            end;
          end;

        ToDevMsg_Disconnect: begin
            // Команда на отключение
            if FDevice <> nil then
            begin
              CloseDevice;
              SendMessage(hMainWnd, FromDevMsg_Disconnected, 0, 0);
            end;
          end;

        ToDevMsg_RxSetFreq: begin
            // Команда на установку Rx Freq
            if FDevice <> nil then
            begin
              if SetRxFreq(msg.wParam) = 0 then
                SendMessage(hMainWnd, WM_USER + 4, 0, 0)
              else
                SendMessage(hMainWnd, WM_USER + 5, 0, 0);
            end;
          end;

      end;


    end;

    Sleep(100);
  end;
end;

end.
