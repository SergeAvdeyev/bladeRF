#include "main_form.h"
#include <QApplication>

TMyApplication * FApplication;

int main(int argc, char *argv[]) {
	int FResult;
	TMainForm * FMainForm;

	FApplication = new TMyApplication(argc, argv);
	FMainForm = new TMainForm(FApplication);
	FMainForm->show();

	FResult = FApplication->exec();
	delete FMainForm;
	delete FApplication;
	return FResult;
}
