program GuiApp;

uses
  Forms,
  MainFrm in 'MainFrm.pas' {Form1},
  BladeRFIntf in 'BladeRFIntf.pas',
  BladeRFThread in 'BladeRFThread.pas',
  FFT in 'FFT.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
