#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "qcustomplot.h"
#include "bladerf_device.h"
#include "test_thread.h"

namespace Ui {
	class MainWindow;
}

struct TFreqLabelDescr {
  uint8_t	LabelValue;
  uint32_t	Digit;
  uint32_t	Module;
  uint32_t	Multiplier;
};
typedef TFreqLabelDescr * PFreqLabelDescr;



class TWaveGenerator {
	public:
		TWaveGenerator();
		~TWaveGenerator() {}
		void Init(int SampleRate, int Frequency, int InitPhase);

	private:
		int FSampleRate;
		double FInitPhase;
		int FFrequency;
		long double FInterval;
		int FAngle;
	public:
		bool GetWave(QVector<double> * DataBufferX,
					 QVector<double> * DataBufferY,
					 int Offset,
					 int NumSamples,
					 int Amplitude);

};

class TFreqLabel : public QLabel {
	Q_OBJECT

	public:
		explicit TFreqLabel(QWidget *parent = 0);
		virtual ~TFreqLabel();

	protected:

		virtual void wheelEvent(QWheelEvent *event);
		virtual void mouseMoveEvent(QMouseEvent *event);

	signals:
		//void mouseDoubleClick(QMouseEvent *event);
		//void mousePress(QMouseEvent *event);
		void mouseMove(QMouseEvent *event);
		//void mouseRelease(QMouseEvent *event);
		void mouseWheel(QWheelEvent *event);
};

class TMainWindow : public QMainWindow {
	Q_OBJECT

	public:
		explicit TMainWindow(QWidget *parent = 0);
		~TMainWindow();

	private:
		TTestThread * FTestThread;

		Ui::MainWindow *ui;
		TWaveGenerator * WG;

		QSettings * FSettings;

		TBladeRfDevice * FDevice;
		QThread * FDeviceThread;

		uint64_t FRxFreq;
		uint32_t FRxSR;
		uint32_t FRxBW;

		//FRxActive : Boolean;

		uint64_t FTxFreq;
		uint32_t FTxSR;
		uint32_t FTxBW;

		bool	FRxBwBlocked;
		bool	FRxSrBlocked;

		bool	FRxLnaBlocked;
		bool	FRxVga1Blocked;
		bool	FRxVga2Blocked;

		bool	FTxBwBlocked;
		bool	FTxSrBlocked;

		bool	FTxVga1Blocked;
		bool	FTxVga2Blocked;

		TFreqLabel * RxLabel_GHz_1;
		TFreqLabel * RxLabel_MHz_100;
		TFreqLabel * RxLabel_MHz_10;
		TFreqLabel * RxLabel_MHz_1;
		TFreqLabel * RxLabel_kHz_100;
		TFreqLabel * RxLabel_kHz_10;
		TFreqLabel * RxLabel_kHz_1;

		//TFreqLabel * FRxLabelPtr;
		QList<TFreqLabel*> FRxLabelsList;
		QList<PFreqLabelDescr> FRxLabelsDescr;

		TFreqLabel * TxLabel_GHz_1;
		TFreqLabel * TxLabel_MHz_100;
		TFreqLabel * TxLabel_MHz_10;
		TFreqLabel * TxLabel_MHz_1;
		TFreqLabel * TxLabel_kHz_100;
		TFreqLabel * TxLabel_kHz_10;
		TFreqLabel * TxLabel_kHz_1;

		//TFreqLabel * FTxLabelPtr;
		QList<TFreqLabel*> FTxLabelsList;
		QList<PFreqLabelDescr> FTxLabelsDescr;

		void readSettings();
		void writeSettings();

		void SetRxFreqLabels();
		void UpdateRxFreqLabels();
		void SetTxFreqLabels();
		void UpdateTxFreqLabels();

		void SetRxFreq();
		//void GetRxFreq();
		void GetRxSr();
		void GetRxBW();
		void GetRxLna();
		void GetRxVga1();
		void GetRxVga2();

		void SetTxFreq();
		//void GetTxFreq();
		void GetTxSr();
		void GetTxBW();
		void GetTxVga1();
		void GetTxVga2();

		void OnConnected(int64_t WParam, int64_t LParam); //message FromDev_Connect;
		void OnDisconnected(int64_t WParam, int64_t LParam); //message FromDev_Disconnect;

		//    void OnTxSetFreq(int WParam, int LParam); message FromDev_TxSetFreq;
		//    void OnTxGetFreq(int WParam, int LParam); message FromDev_TxGetFreq;
		//    void OnTxSetSR(int WParam, int LParam); message FromDev_TxSetSR;
		//    void OnTxSetBW(int WParam, int LParam); message FromDev_TxSetBW;

		void OnRxStart(int64_t WParam, int64_t LParam); //message FromDev_RxStart;
		void OnRxStop(int64_t WParam, int64_t LParam); //message FromDev_RxStop;

		void OnTxStart(int64_t WParam, int64_t LParam); //message FromDev_TxStart;
		void OnTxStop(int64_t WParam, int64_t LParam); //message FromDev_TxStop;

		void OnRxError(int64_t WParam, int64_t LParam); //message FromDev_RxProcessError;
		void OnRxData(int64_t WParam, int64_t LParam); //message FromDev_RxProcessData;

		void OnTxError(int64_t WParam, int64_t LParam); //message FromDev_TxProcessError;



		//void ReDraw(SignalBuffer: PDCplx);
		//procedure StartAudio;
		//procedure StopAudio;


	private slots:
		void titleDoubleClick(QMouseEvent *event, QCPPlotTitle *title);
		void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
		void legendDoubleClick(QCPLegend* legend, QCPAbstractLegendItem* item);
		void selectionChanged();
		void mousePress();
		void mouseWheel();
		void addRandomGraph();
		void removeSelectedGraph();
		void removeAllGraphs();
		void contextMenuRequest(QPoint pos);
		void moveLegend();
		void graphClicked(QCPAbstractPlottable *plottable);

		void FreqLabelWheel(QWheelEvent* event);
		void on_RxSrSlider_valueChanged(int value);
		void on_RxBwSlider_valueChanged(int value);
		void on_RxLnaSlider_valueChanged(int value);
		void on_RxVga1Slider_valueChanged(int value);
		void on_RxVga2Slider_valueChanged(int value);
		void on_TxSrSlider_valueChanged(int value);
		void on_TxBwSlider_valueChanged(int value);
		void on_TxVga1Slider_valueChanged(int value);
		void on_TxVga2Slider_valueChanged(int value);

		void on_OpenDevBtn_clicked();
		void on_StartRxBtn_clicked();
		void on_StartTxBtn_clicked();

		void on_DebugBox_clicked();


	public slots:
		void MessageFromDevice(uint16_t Msg, int64_t WParam, int64_t LParam);
		void OnDeviceThreadFinished();
	signals:
		void MessageToDevice(uint16_t Msg, int64_t WParam, int64_t LParam);
};



#endif // MAINWINDOW_H
