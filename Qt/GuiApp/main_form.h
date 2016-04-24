#ifndef MAIN_FORM_H
#define MAIN_FORM_H

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMouseEvent>
#include <QLabel>
#include <QLayout>
#include <QPainter>
#include <QTimer>

#include <windows.h>
#include <windowsx.h>

namespace Ui {
	class TMainForm;
}



class TMyApplication : public QApplication {
	public:
		explicit TMyApplication(int &argc, char **argv);
};

struct TFreqLabelDescr {
	int		LabelValue;
	int		Digit;
	int		Module;
	int		Multiplier;
};
typedef TFreqLabelDescr * PFreqLabelDescr;

class TPaintLabel : public QLabel {
	Q_OBJECT
	public:
		TPaintLabel(QWidget *parent = 0);

		int FSpectr[512];
	private:
		void paintEvent(QPaintEvent *event);
};



class TMainForm : public QMainWindow {
	Q_OBJECT

	public:
		explicit TMainForm(TMyApplication * ApplicationPtr, QWidget *parent = 0);
		~TMainForm();

		virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);
		//virtual void paintEvent(QPaintEvent *event);

	private:
		Ui::TMainForm *ui;
		TMyApplication * FApplicationPtr;

		u_int64 FRxFreq;
		QHash<QLabel *, PFreqLabelDescr> FRxLabels;

		u_int64 FTxFreq;
		QHash<QLabel *, PFreqLabelDescr> FTxLabels;

		QWidget * FFocusedWidget;

		TPaintLabel * FPaintLabel;

		QTimer * FTimer;
		int idx;

		bool eventFilter(QObject *obj, QEvent *event);

		void SetRxFreqLabels();
		void UpdateRxFreqLabels();

		void SetTxFreqLabels();
		void UpdateTxFreqLabels();

	private slots:
		void OnTimer();
};

#endif // MAIN_FORM_H
