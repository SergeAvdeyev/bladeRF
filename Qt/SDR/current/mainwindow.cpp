#include "mainwindow.h"
#include "ui_mainwindow.h"


TWaveGenerator::TWaveGenerator() {
	FSampleRate = 0;
	FInitPhase = 0;
}

void TWaveGenerator::Init(int SampleRate, int Frequency, int InitPhase) {
	FSampleRate = SampleRate;
	if (InitPhase == 0)
		FInitPhase = 0;
	else
		FInitPhase = (2*M_PI)/(360/InitPhase);
	FFrequency = Frequency;

	FAngle = 0;

	FInterval = (2*M_PI)/(FSampleRate/FFrequency);
}

bool TWaveGenerator::GetWave(QVector<double> * DataBufferX,
							 QVector<double> * DataBufferY,
							 int Offset,
							 int NumSamples,
							 int Amplitude) {
	if (FSampleRate == 0) return false;
	if (Offset > DataBufferX->size()) return false;
	int i, to;

	to = Offset + NumSamples;
	if (to > DataBufferX->size())
		to = DataBufferX->size();

	for (i = Offset; i < to; i++) {
		(*DataBufferX)[i] = i;

		(*DataBufferY)[i] = Amplitude*sin((FInterval*FAngle) + FInitPhase);
		FAngle++;
	};
	FAngle = FAngle % FSampleRate;
	return true;
}



TFreqLabel::TFreqLabel(QWidget *parent) : QLabel(parent) {

}

TFreqLabel::~TFreqLabel() {

}


void TFreqLabel::wheelEvent(QWheelEvent *event) {
	//QObject FSender = QObject.sender();

	emit mouseWheel(event);


	// call event of affected layout element:
	//if (QCPLayoutElement *el = layoutElementAt(event->pos()))
	//  el->wheelEvent(event);

	QWidget::wheelEvent(event);
}

void TFreqLabel::mouseMoveEvent(QMouseEvent *event) {
	//QObject FSender = QObject.sender();

	emit mouseMove(event);

	// call event of affected layout element:
	//if (mMouseEventElement)
	//  mMouseEventElement->mouseMoveEvent(event);

	QWidget::mouseMoveEvent(event);
}









TMainWindow::TMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);

	FSettings = new QSettings("settings.conf", QSettings::NativeFormat);

	WG = new TWaveGenerator;
	WG->Init(44100, 10, 0);
	FSpectrBuffer = NULL;

	FRxFreq = 100;
	FTxFreq = 100;

	FRxBwBlocked = false;
	FRxSrBlocked = false;

	FRxLnaBlocked = false;
	FRxVga1Blocked = false;
	FRxVga2Blocked = false;

	FTxBwBlocked = false;
	FTxSrBlocked = false;

	FTxVga1Blocked = false;
	FTxVga2Blocked = false;

	ui->StartRxBtn->setEnabled(false);
	ui->StartTxBtn->setEnabled(false);

	SetRxFreqLabels();
	UpdateRxFreqLabels();
	SetTxFreqLabels();
	UpdateTxFreqLabels();

	IIR_FILTER_I = new TButtFilter;
	IIR_FILTER_Q = new TButtFilter;
	IIR_FILTER_I->InitLowPass(4, 2000000, 40000);
	IIR_FILTER_Q->InitLowPass(4, 2000000, 40000);

//	IIR_Filter.setup(3,    // order
//					 480000,// sample rate
//					 200000, // center frequency
//					 50000,  // band width
//					 3);   // ripple dB

//	IIR_Filter_2 = new CFilterIIRStream;
//	IIR_Filter_2->Init(CFilterIIRStream::LPF, 100000, 4800000, 15, 20);

//	IIR_Filter = new Dsp::SmoothedFilterDesign<Dsp::RBJ::Design::LowPass, 2> (1024);
//	Dsp::Params params;
//	params[0] = 4800000; // sample rate
//	params[1] = 1000000; // cutoff frequency
//	params[2] = 1.25; // Q
//	IIR_Filter->setParams (params);



	ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
								  QCP::iSelectLegend | QCP::iSelectPlottables);
	//ui->customPlot->xAxis->setRange(-8, 8);
	ui->customPlot->xAxis->setRange(0, 4800);
	ui->customPlot->xAxis->setNumberFormat("f");
	ui->customPlot->xAxis->setNumberPrecision(0);
	ui->customPlot->xAxis->setTickLabelRotation(60);
	ui->customPlot->yAxis->setRange(-2048, 2048);
	ui->customPlot->axisRect()->setupFullAxesBox();

	ui->customPlot->plotLayout()->insertRow(0);
	ui->customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->customPlot, "Interaction Example"));

	//ui->customPlot->xAxis->setLabel("x Axis");
	//ui->customPlot->yAxis->setLabel("y Axis");
	ui->customPlot->legend->setVisible(false);
	QFont legendFont = font();
	legendFont.setPointSize(10);
	ui->customPlot->legend->setFont(legendFont);
	ui->customPlot->legend->setSelectedFont(legendFont);
	ui->customPlot->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items

	QPen graphPen;
	graphPen.setWidthF(1);
	graphPen.setColor(QColor(0, 200, 255));

	ui->customPlot->addGraph();
	ui->customPlot->graph(0)->setName(QString("New graph %1").arg(ui->customPlot->graphCount() - 1));
	ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
	ui->customPlot->graph(0)->setPen(graphPen);

	ui->customPlot->addGraph();
	ui->customPlot->graph(1)->setName(QString("New graph %1").arg(ui->customPlot->graphCount() - 1));
	ui->customPlot->graph(1)->setLineStyle(QCPGraph::lsLine);
	graphPen.setColor(QColor(200, 0, 255));
	ui->customPlot->graph(1)->setPen(graphPen);
	ui->customPlot->graph(1)->setVisible(ui->QBox->isChecked());

	addRandomGraph();

	// connect slot that ties some axis selections together (especially opposite axes):
	connect(ui->customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
	// connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
	connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
	connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

	// make bottom and left axes transfer their ranges to top and right axes:
	connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

	FDevice = new TBladeRfDevice(this);

	readSettings();

}

TMainWindow::~TMainWindow() {
	if (ui->OpenDevBtn->text() == "Disconnect") {
		FDevice->Disconnect();
		OnDisconnected();
	};


	delete RxLabel_GHz_1;
	delete RxLabel_MHz_100;
	delete RxLabel_MHz_10;
	delete RxLabel_MHz_1;
	delete RxLabel_kHz_100;
	delete RxLabel_kHz_10;
	delete RxLabel_kHz_1;

	delete TxLabel_GHz_1;
	delete TxLabel_MHz_100;
	delete TxLabel_MHz_10;
	delete TxLabel_MHz_1;
	delete TxLabel_kHz_100;
	delete TxLabel_kHz_10;
	delete TxLabel_kHz_1;

	writeSettings();
	delete ui;
}

void TMainWindow::writeSettings() {

	FSettings->beginGroup("MainWindow");
	FSettings->setValue("size", size());
	FSettings->setValue("pos", pos());
	FSettings->endGroup();
	FSettings->sync();
}

void TMainWindow::readSettings() {

	FSettings->beginGroup("MainWindow");
	resize(FSettings->value("size", QSize(400, 400)).toSize());
	move(FSettings->value("pos", QPoint(200, 200)).toPoint());
	FSettings->endGroup();
}


void TMainWindow::SetRxFreqLabels() {
	PFreqLabelDescr FFreqLabelDescr;
	int i;

	RxLabel_GHz_1 = new TFreqLabel(ui->RxPanel);
	RxLabel_MHz_100 = new TFreqLabel(ui->RxPanel);
	RxLabel_MHz_10 = new TFreqLabel(ui->RxPanel);
	RxLabel_MHz_1 = new TFreqLabel(ui->RxPanel);
	RxLabel_kHz_100 = new TFreqLabel(ui->RxPanel);
	RxLabel_kHz_10 = new TFreqLabel(ui->RxPanel);
	RxLabel_kHz_1 = new TFreqLabel(ui->RxPanel);

	RxLabel_GHz_1->setGeometry(20, 20, 19, 26);
	RxLabel_MHz_100->setGeometry(43, 20, 19, 26);
	RxLabel_MHz_10->setGeometry(61, 20, 19, 26);
	RxLabel_MHz_1->setGeometry(79, 20, 19, 26);
	RxLabel_kHz_100->setGeometry(102, 20, 19, 26);
	RxLabel_kHz_10->setGeometry(120, 20, 19, 26);
	RxLabel_kHz_1->setGeometry(139, 20, 19, 26);

	QFont F = RxLabel_GHz_1->font();
	F.setFamily("Noto Serif");
	F.setBold(true);
	F.setPointSize(24);
	RxLabel_GHz_1->setFont(F);
	RxLabel_MHz_100->setFont(F);
	RxLabel_MHz_10->setFont(F);
	RxLabel_MHz_1->setFont(F);
	RxLabel_kHz_100->setFont(F);
	RxLabel_kHz_10->setFont(F);
	RxLabel_kHz_1->setFont(F);

	connect(RxLabel_GHz_1, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(RxLabel_MHz_100, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(RxLabel_MHz_10, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(RxLabel_MHz_1, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(RxLabel_kHz_100, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(RxLabel_kHz_10, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(RxLabel_kHz_1, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));

	FRxLabelsList.append(RxLabel_kHz_1);
	FRxLabelsList.append(RxLabel_kHz_10);
	FRxLabelsList.append(RxLabel_kHz_100);
	FRxLabelsList.append(RxLabel_MHz_1);
	FRxLabelsList.append(RxLabel_MHz_10);
	FRxLabelsList.append(RxLabel_MHz_100);
	FRxLabelsList.append(RxLabel_GHz_1);

	i = 0;
	while (i < 7) {
		FFreqLabelDescr = (PFreqLabelDescr)malloc(sizeof(TFreqLabelDescr));
		FFreqLabelDescr->Digit = i;
		if (i == 0) {
			FFreqLabelDescr->Module = 10;
			FFreqLabelDescr->Multiplier = 1;
		} else if (i == 1) {
			FFreqLabelDescr->Module = 100;
			FFreqLabelDescr->Multiplier = 10;
		} else if (i == 2) {
			FFreqLabelDescr->Module = 1000;
			FFreqLabelDescr->Multiplier = 100;
		} else if (i == 3) {
			FFreqLabelDescr->Module = 10000;
			FFreqLabelDescr->Multiplier = 1000;
		} else if (i == 4) {
			FFreqLabelDescr->Module = 100000;
			FFreqLabelDescr->Multiplier = 10000;
		} else if (i == 5) {
			FFreqLabelDescr->Module = 1000000;
			FFreqLabelDescr->Multiplier = 100000;
		} else if (i == 6) {
			FFreqLabelDescr->Module = 10000000;
			FFreqLabelDescr->Multiplier = 1000000;
		};
		FRxLabelsDescr.append(FFreqLabelDescr);
		i++;
	};
}

void TMainWindow::UpdateRxFreqLabels() {
	PFreqLabelDescr F;
	int i;
	TFreqLabel * L;

	for (i = 0; i < FRxLabelsList.count(); i++) {
		L = FRxLabelsList.at(i);
		F = FRxLabelsDescr.at(i);
		F->LabelValue = (FRxFreq % F->Module) / F->Multiplier;
		L->setText(QString::number(F->LabelValue));
	};
}

void TMainWindow::SetTxFreqLabels() {
	PFreqLabelDescr FFreqLabelDescr;
	int i;

	TxLabel_GHz_1 = new TFreqLabel(ui->TxPanel);
	TxLabel_MHz_100 = new TFreqLabel(ui->TxPanel);
	TxLabel_MHz_10 = new TFreqLabel(ui->TxPanel);
	TxLabel_MHz_1 = new TFreqLabel(ui->TxPanel);
	TxLabel_kHz_100 = new TFreqLabel(ui->TxPanel);
	TxLabel_kHz_10 = new TFreqLabel(ui->TxPanel);
	TxLabel_kHz_1 = new TFreqLabel(ui->TxPanel);

	TxLabel_GHz_1->setGeometry(20, 20, 19, 26);
	TxLabel_MHz_100->setGeometry(43, 20, 19, 26);
	TxLabel_MHz_10->setGeometry(61, 20, 19, 26);
	TxLabel_MHz_1->setGeometry(79, 20, 19, 26);
	TxLabel_kHz_100->setGeometry(102, 20, 19, 26);
	TxLabel_kHz_10->setGeometry(120, 20, 19, 26);
	TxLabel_kHz_1->setGeometry(139, 20, 19, 26);

	QFont F = TxLabel_GHz_1->font();
	F.setFamily("Noto Serif");
	F.setBold(true);
	F.setPointSize(24);
	TxLabel_GHz_1->setFont(F);
	TxLabel_MHz_100->setFont(F);
	TxLabel_MHz_10->setFont(F);
	TxLabel_MHz_1->setFont(F);
	TxLabel_kHz_100->setFont(F);
	TxLabel_kHz_10->setFont(F);
	TxLabel_kHz_1->setFont(F);

	connect(TxLabel_GHz_1, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(TxLabel_MHz_100, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(TxLabel_MHz_10, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(TxLabel_MHz_1, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(TxLabel_kHz_100, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(TxLabel_kHz_10, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(TxLabel_kHz_1, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));

	FTxLabelsList.append(TxLabel_kHz_1);
	FTxLabelsList.append(TxLabel_kHz_10);
	FTxLabelsList.append(TxLabel_kHz_100);
	FTxLabelsList.append(TxLabel_MHz_1);
	FTxLabelsList.append(TxLabel_MHz_10);
	FTxLabelsList.append(TxLabel_MHz_100);
	FTxLabelsList.append(TxLabel_GHz_1);

	i = 0;
	while (i < 7) {
		FFreqLabelDescr = (PFreqLabelDescr)malloc(sizeof(TFreqLabelDescr));
		FFreqLabelDescr->Digit = i;
		if (i == 0) {
			FFreqLabelDescr->Module = 10;
			FFreqLabelDescr->Multiplier = 1;
		} else if (i == 1) {
			FFreqLabelDescr->Module = 100;
			FFreqLabelDescr->Multiplier = 10;
		} else if (i == 2) {
			FFreqLabelDescr->Module = 1000;
			FFreqLabelDescr->Multiplier = 100;
		} else if (i == 3) {
			FFreqLabelDescr->Module = 10000;
			FFreqLabelDescr->Multiplier = 1000;
		} else if (i == 4) {
			FFreqLabelDescr->Module = 100000;
			FFreqLabelDescr->Multiplier = 10000;
		} else if (i == 5) {
			FFreqLabelDescr->Module = 1000000;
			FFreqLabelDescr->Multiplier = 100000;
		} else if (i == 6) {
			FFreqLabelDescr->Module = 10000000;
			FFreqLabelDescr->Multiplier = 1000000;
		};
		FTxLabelsDescr.append(FFreqLabelDescr);
		i++;
	};
}

void TMainWindow::UpdateTxFreqLabels() {
	PFreqLabelDescr F;
	int i;
	TFreqLabel * L;

	for (i = 0; i < FTxLabelsList.count(); i++) {
		L = FTxLabelsList.at(i);
		F = FTxLabelsDescr.at(i);
		F->LabelValue = (FTxFreq % F->Module) / F->Multiplier;
		L->setText(QString::number(F->LabelValue));
	};
}






void TMainWindow::FreqLabelWheel(QWheelEvent* event) {
	Q_UNUSED(event);
	TFreqLabel * FSender = (TFreqLabel *)sender();
	int i;
	uint8_t k;

	PFreqLabelDescr F;
	uint64_t FMult;

	QPoint d = event->angleDelta();
	int delta;
	if (d.y() > 0)
		delta = 1;
	else
		delta = -1;

	i = FRxLabelsList.indexOf(FSender);
	if (i != -1) {
		F = FRxLabelsDescr.at(i);
		FMult = 1;
		for (k = 0; k < F->Digit; k++)
			FMult = FMult * 10;
		if (delta > 0)
			FRxFreq += FMult;
		else {
			if (FMult > FRxFreq)
				FRxFreq = 237500;
			else
				FRxFreq -= FMult;
		};
		if (FRxFreq < 237500)
			FRxFreq = 237500;
		if (FRxFreq > 3799999)
			FRxFreq = 3799999;
		UpdateRxFreqLabels();
		SetRxFreq();

		return;
	};

	i = FTxLabelsList.indexOf(FSender);
	if (i != -1) {
		F = FTxLabelsDescr.at(i);
		FMult = 1;
		for (k = 0; k < F->Digit; k++)
			FMult = FMult * 10;
		if (delta > 0)
			FTxFreq += FMult;
		else {
			if (FMult > FTxFreq)
				FTxFreq = 237500;
			else
				FTxFreq -= FMult;
		};
		if (FTxFreq < 237500)
			FTxFreq = 237500;
		if (FTxFreq > 3799999)
			FTxFreq = 3799999;
		UpdateTxFreqLabels();
		SetTxFreq();
	};
}




void TMainWindow::OnConnected(int Status) {
	QString S;

	if (Status != 0) {
		if (ui->DebugBox->isChecked()) {
			S = QString::fromUtf8(bladerf_strerror(Status));
			ui->Memo->appendPlainText("Failed to connect: " + S);
		};
		ui->OpenDevBtn->setEnabled(true);
		return;
	};

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Connected");

	FRxFreq = FSettings->value("Blade_RX/CenterFreq", 435900).toInt();
	UpdateRxFreqLabels();
	SetRxFreq();

	ui->RxSrSlider->setValue(FSettings->value("Blade_RX/SampleRate", 40).toInt());
	GetRxSr();
	ui->RxBwSlider->setValue(FSettings->value("Blade_RX/BandWidth", 40).toInt());
	GetRxBW();
	ui->RxLnaSlider->setValue(FSettings->value("Blade_RX/LNA", 3).toInt());

	ui->RxVga1Slider->setValue(FSettings->value("Blade_RX/VGA1", 5).toInt());
	GetRxVga1();
	ui->RxVga2Slider->setValue(FSettings->value("Blade_RX/VGA2", 0).toInt());
	GetRxVga2();

	FTxFreq = FSettings->value("Blade_TX/CenterFreq", 435900).toInt();
	UpdateTxFreqLabels();
	SetTxFreq();

	ui->TxSrSlider->setValue(FSettings->value("Blade_TX/SampleRate", 40).toInt());
	GetTxSr();
	ui->TxBwSlider->setValue(FSettings->value("Blade_TX/BandWidth", 40).toInt());
	GetTxBW();
	ui->TxVga1Slider->setValue(FSettings->value("Blade_TX/VGA1", 5).toInt());
	GetTxVga1();
	ui->TxVga2Slider->setValue(FSettings->value("Blade_TX/VGA2", 0).toInt());
	GetTxVga2();

	ui->OpenDevBtn->setEnabled(true);
	ui->OpenDevBtn->setText("Disconnect");
	ui->StartRxBtn->setEnabled(true);
	ui->StartTxBtn->setEnabled(true);
	ui->RxPanel->setEnabled(true);
	ui->TxPanel->setEnabled(true);
}

void TMainWindow::OnDisconnected() {

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Disconnected");
	ui->OpenDevBtn->setEnabled(true);
	ui->OpenDevBtn->setText("Connect");
	ui->StartRxBtn->setEnabled(false);
	ui->StartRxBtn->setText("Start RX");
	ui->StartTxBtn->setEnabled(false);
	ui->StartTxBtn->setText("Start TX");
	ui->RxPanel->setEnabled(false);
	ui->TxPanel->setEnabled(false);

	FSettings->setValue("Blade_RX/CenterFreq", (quint32)FRxFreq);
	FSettings->setValue("Blade_RX/SampleRate", ui->RxSrSlider->value());
	FSettings->setValue("Blade_RX/BandWidth", ui->RxBwSlider->value());
	FSettings->setValue("Blade_RX/LNA", ui->RxLnaSlider->value());
	FSettings->setValue("Blade_RX/VGA1", ui->RxVga1Slider->value());
	FSettings->setValue("Blade_RX/VGA2", ui->RxVga2Slider->value());

	FSettings->setValue("Blade_TX/CenterFreq", (quint32)FTxFreq);
	FSettings->setValue("Blade_TX/SampleRate", ui->TxSrSlider->value());
	FSettings->setValue("Blade_TX/BandWidth", ui->TxBwSlider->value());
	FSettings->setValue("Blade_TX/VGA1", ui->TxVga1Slider->value());
	FSettings->setValue("Blade_TX/VGA2", ui->TxVga2Slider->value());
	FSettings->sync();
}

void TMainWindow::OnRxStart(int Status) {
	QString S;

	if (Status == 0) {
		ui->StartRxBtn->setText("Stop RX");
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("RX started");

	} else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(Status));
		ui->Memo->appendPlainText("Failed to start RX: " + S);
	};
	ui->StartRxBtn->setEnabled(true);
}


void TMainWindow::OnRxStop() {

	//StopAudio;
	ui->StartRxBtn->setText("Start RX");
	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("RX stopped");
	ui->StartRxBtn->setEnabled(true);
}

void TMainWindow::OnTxStart(int Status) {
	QString S;

	if (Status == 0) {
		ui->StartTxBtn->setText("Stop TX");
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("TX started");
	} else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(Status));
		ui->Memo->appendPlainText("Failed to start TX: " + S);
	};
	ui->StartTxBtn->setEnabled(true);
}

void TMainWindow::OnTxStop() {
	ui->StartTxBtn->setText("Start TX");
	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("TX stopped");
	ui->StartTxBtn->setEnabled(true);
}


void TMainWindow::OnRxError(int64_t WParam, int64_t LParam) {
	Q_UNUSED(LParam);
	QString S;

	S = QString::fromUtf8(bladerf_strerror(WParam));
	ui->Memo->appendPlainText("Failed receiving Data: " + S);
}

void TMainWindow::OnTxError(int64_t WParam, int64_t LParam) {
	Q_UNUSED(LParam);
	QString S;

	S = QString::fromUtf8(bladerf_strerror(WParam));
	ui->Memo->appendPlainText("Failed transmitting Data: " + S);
}


















void TMainWindow::addRandomGraph() {
	int sample_rate = 44100;
	//int n = 2048; // number of points in graph
	QVector<double> x(sample_rate*10), y(sample_rate*10);
	//double CurrentPhase = 90;
	//double tmp;
	//double _const = 2*M_PI*0.01;
	//int i;
	//double tmp_phase;
	//tmp_phase = CurrentPhase;

//	double freq = 10;
//	double phase = (2*M_PI)/(360/180);
//	double angle;

//	long double interval = (2*M_PI)/(sample_rate/freq);

//	int from, to;
//	bool last = false;

	bool res;
	for (int j = 0; j < 1000; j++) {
		res = WG->GetWave(&x, &y, 2048*j, 2048, 5);
		if (!res)
			break;
	};

	ui->customPlot->graph()->setData(x, y);
	ui->customPlot->replot();
}











void TMainWindow::selectionChanged() {
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.

   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.

   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */

  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
	  ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
	ui->customPlot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
	ui->customPlot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
	  ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
	ui->customPlot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
	ui->customPlot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

	// synchronize selection of graphs with selection of corresponding legend items:
	for (int i = 0; i < ui->customPlot->graphCount(); ++i)   {
		QCPGraph *graph = ui->customPlot->graph(i);
		QCPPlottableLegendItem *item = ui->customPlot->legend->itemWithPlottable(graph);
		if (item->selected() || graph->selected()) 	{
			item->setSelected(true);
			graph->setSelected(true);
		};
	};
}

void TMainWindow::mousePress() {
	// if an axis is selected, only allow the direction of that axis to be dragged
	// if no axis is selected, both directions may be dragged

	if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
		ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->xAxis->orientation());
	else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
		ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->yAxis->orientation());
	else
		ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void TMainWindow::mouseWheel() {
	// if an axis is selected, only allow the direction of that axis to be zoomed
	// if no axis is selected, both directions may be zoomed

	if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
		ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->xAxis->orientation());
	else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
		ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->yAxis->orientation());
	else
		ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}
































void TMainWindow::on_RxSrSlider_valueChanged(int value) {
	Q_UNUSED(value);

	if (FRxSrBlocked) return;
	if (FDevice == NULL) return;

	FRxSR = ui->RxSrSlider->value()*100000;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Setting RX sampling rate to " + QString::number(FRxSR) + " Hz... ");
	uint32_t F;
	int I = FDevice->RxSetSrSync(FRxSR, &F);

	if (I == 0) {
		ui->RxSrLabel->setText(QString::number(F));
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Actual RX sampling rate is " + ui->RxSrLabel->text() + " Hz... ");
	} else if (ui->DebugBox->isChecked()) {
		QString S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to set RX SampleRate: " + S);
	};
}

void TMainWindow::on_RxBwSlider_valueChanged(int value) {
	Q_UNUSED(value);

	int I;
	QString S;

	if (FRxBwBlocked) return;
	if (FDevice == NULL) return;

	FRxBW = ui->RxBwSlider->value()*100000;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Setting RX bandwidth to " + QString::number(FRxBW) + " Hz... ");
	I = FDevice->RxSetBwSync(FRxBW, &FRxBW);
	if (I == 0) {
		FRxBwBlocked = true;
		ui->RxBwSlider->setValue(FRxBW / 100000);
		ui->RxBwLabel->setText(QString::number(FRxBW));
		FRxBwBlocked = false;
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Actual RX bandwidth is " + ui->RxBwLabel->text() + " Hz... ");
	} else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to set RX Bandwidth: " + S);
	};
}

void TMainWindow::on_RxLnaSlider_valueChanged(int value) {
	Q_UNUSED(value);

	int I;
	QString S;

	//if FRxLnaBlocked then Exit;
	if (FDevice == NULL) return;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Setting RX LNA to " + QString::number(ui->RxLnaSlider->value()));
	I = FDevice->RxSetLnaSync((bladerf_lna_gain)ui->RxLnaSlider->value());
	if (I == 0) {
		if (ui->RxLnaSlider->value() == 1)
			ui->RxLnaLabel->setText("0");
		else if (ui->RxLnaSlider->value() == 2)
			ui->RxLnaLabel->setText("3");
		else
			ui->RxLnaLabel->setText("6");
	} else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to set RX LNA: " + S);
	};
}

void TMainWindow::on_RxVga1Slider_valueChanged(int value) {
	Q_UNUSED(value);

	int I;
	QString S;
	int F;

	if (FRxVga1Blocked) return;
	if (FDevice == NULL) return;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Setting RX VGA1 to " + QString::number(ui->RxVga1Slider->value()));
	I = FDevice->RxSetVga1Sync(ui->RxVga1Slider->value(), &F);
	if (I == 0)
		ui->RxVga1Label->setText(QString::number(F));
	else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to set RX VGA1: " + S);
	};
}

void TMainWindow::on_RxVga2Slider_valueChanged(int value) {
	Q_UNUSED(value);

	int I;
	QString S;
	int F;

	if (FRxVga2Blocked) return;
	if (FDevice == NULL) return;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Setting RX VGA2 to " + QString::number(ui->RxVga2Slider->value()));
	I = FDevice->RxSetVga2Sync(ui->RxVga2Slider->value(), &F);
	if (I == 0)
		ui->RxVga2Label->setText(QString::number(F));
	else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to set RX VGA2: " + S);
	};
}




void TMainWindow::on_TxSrSlider_valueChanged(int value) {
	Q_UNUSED(value);

	if (FTxSrBlocked) return;
	if (FDevice == NULL) return;

	FTxSR = ui->TxSrSlider->value()*100000;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Setting TX sampling rate to " + QString::number(FTxSR) + " Hz... ");
	uint32_t F;
	int I = FDevice->TxSetSrSync(FTxSR, &F);

	if (I == 0) {
		ui->TxSrLabel->setText(QString::number(F));
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Actual TX sampling rate is " + ui->TxSrLabel->text() + " Hz... ");
	} else if (ui->DebugBox->isChecked()) {
		QString S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to set TX SampleRate: " + S);
	};
}

void TMainWindow::on_TxBwSlider_valueChanged(int value) {
	Q_UNUSED(value);

	uint32_t F;
	int I;
	QString S;

	if (FTxBwBlocked) return;
	if (FDevice == NULL) return;

	FTxBW = ui->TxBwSlider->value()*100000;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Setting TX bandwidth to " + QString::number(FTxBW) + " Hz... ");
	I = FDevice->TxSetBwSync(FTxBW, &F);
	if (I == 0) {
		ui->TxBwLabel->setText(QString::number(F));
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Actual TX bandwidth is " + ui->TxBwLabel->text() + " Hz... ");
	} else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to set TX Bandwidth: " + S);
	};
}

void TMainWindow::on_TxVga1Slider_valueChanged(int value) {
	Q_UNUSED(value);

	int I;
	QString S;
	int F;

	if (FTxVga1Blocked) return;
	if (FDevice == NULL) return;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Setting TX VGA1 to " + QString::number(ui->TxVga1Slider->value()));
	I = FDevice->TxSetVga1Sync(ui->TxVga1Slider->value(), &F);
	if (I == 0)
		ui->TxVga1Label->setText(QString::number(F));
	else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to set TX VGA1: " + S);
	};
}

void TMainWindow::on_TxVga2Slider_valueChanged(int value) {
	Q_UNUSED(value);

	int I;
	QString S;
	int F;

	if (FTxVga2Blocked) return;
	if (FDevice == NULL) return;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Setting TX VGA2 to " + QString::number(ui->TxVga2Slider->value()));
	I = FDevice->TxSetVga2Sync(ui->TxVga2Slider->value(), &F);
	if (I == 0)
		ui->TxVga2Label->setText(QString::number(F));
	else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to set TX VGA2: " + S);
	};
}

void TMainWindow::on_OpenDevBtn_clicked() {
	if (FDevice == NULL) return;

	int Status;

	ui->OpenDevBtn->setEnabled(false);
	if (ui->OpenDevBtn->text() == "Connect") {
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Connecting to device... ");
		Status = FDevice->Connect();
		OnConnected(Status);
	} else {
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Disconnecting ... ");
		FDevice->Disconnect();
		OnDisconnected();
	};
}

void TMainWindow::on_StartRxBtn_clicked() {
	if (FDevice == NULL) return;

	ui->StartRxBtn->setEnabled(false);
	if (ui->StartRxBtn->text() == "Start RX") {
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Starting RX ... ");
		FDevice->RxStart();
		//emit MessageToDevice(ToDevMsg_RxStart, 0, 0);
	} else {
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Stopping RX ... ");
		FDevice->RxStop();
		//emit MessageToDevice(ToDevMsg_RxStop, 0, 0);
	};
}

void TMainWindow::on_StartTxBtn_clicked() {
	if (FDevice == NULL) return;

	ui->StartTxBtn->setEnabled(false);
	if (ui->StartTxBtn->text() == "Start TX") {
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Starting TX ... ");
		FDevice->TxStart();
		//emit MessageToDevice(ToDevMsg_TxStart, 0, 0);
	} else {
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Stopping TX ... ");
		FDevice->TxStop();
		//emit MessageToDevice(ToDevMsg_TxStop, 0, 0);
	};
}













///////////////////////////////////////////
////////                        ///////////
////////     RX                 ///////////
////////                        ///////////
///////////////////////////////////////////

void TMainWindow::SetRxFreq() {
	int I;
	QString S;

	if (FDevice == NULL) return;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Setting RX frequency... ");
	I = FDevice->RxSetFreqSync(FRxFreq);
	if (I == 0) {
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Set RX Frequency - OK");
//		if (ui->SpectrBtn->isChecked()) {
//			uint32_t tmp = FRxSR / 2;
//			//ui->customPlot->xAxis->setRange(FRxFreq - tmp, FRxFreq + tmp);
//			ui->customPlot->xAxis->scaleRange(2, 1);
//			ui->customPlot->replot();
//		};
	} else {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to set RX Frequency: " + S);
	};
}


void TMainWindow::GetRxSr() {
	int I;
	QString S;
	uint32_t F;

	if (FDevice == NULL) return;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Read RX SampleRate...");
	FRxSrBlocked = true;

	I = FDevice->RxGetSrSync(&F);
	if (I == 0) {
		//RxSrSlider.Value := F div 100000;
		ui->RxSrLabel->setText(QString::number(F));
		FRxSrBlocked = false;
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Actual RX sampling rate is " + QString::number(F) + " Hz");
	} else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to get RX SampleRate: " + S);
	};
}


void TMainWindow::GetRxBW() {
	int I;
	QString S;
	uint32_t F;

	if (FDevice == NULL) return;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Read RX bandwidth...");
	FRxBwBlocked = true;
	I = FDevice->RxGetBwSync(&F);
	if (I == 0) {
		ui->RxBwLabel->setText(QString::number(F));
		//RxBwSlider.Value := F div 100000;
		FRxBwBlocked = false;
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Actual RX bandwidth is " + QString::number(F) + " Hz");
	} else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to get RX Bandwidth: " + S);
	};
}


void TMainWindow::GetRxVga1() {
	int I;
	QString S;
	int32_t F;

	if (FDevice == NULL) return;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Read RX VGA1...");
	FRxVga1Blocked = true;
	I = FDevice->RxGetVga1Sync(&F);
	if (I == 0) {
		ui->RxVga1Label->setText(QString::number(F));
		//RxVga1Slider.Value := F;
		FRxVga1Blocked = false;
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Actual RX VGA1 is " + QString::number(F));
	} else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to get RX VGA1: " + S);
	};
}

void TMainWindow::GetRxVga2() {
	int I;
	QString S;
	int32_t F;

	if (FDevice == NULL) return;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Read RX VGA2...");
	FRxVga2Blocked = true;
	I = FDevice->RxGetVga2Sync(&F);
	if (I == 0) {
		ui->RxVga2Label->setText(QString::number(F));
		//RxVga2Slider.Value := F;
		FRxVga2Blocked = false;
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Actual RX VGA2 is " + QString::number(F));
	} else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to get RX VGA2: " + S);
	};
}





///////////////////////////////////////////
////////                        ///////////
////////     TX                 ///////////
////////                        ///////////
///////////////////////////////////////////
void TMainWindow::SetTxFreq() {
	int I;
	QString S;

	if (FDevice == NULL) return;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Setting TX frequency... ");
	I = FDevice->TxSetFreqSync(FTxFreq);
	if (I == 0) {
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Set TX Frequency - OK");
	} else {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to set TX Frequency: " + S);
	};
}



void TMainWindow::GetTxSr() {
	int I;
	QString S;
	uint32_t F;

	if (FDevice == NULL) return;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Read TX SampleRate...");
	FTxSrBlocked = true;

	I = FDevice->TxGetSrSync(&F);
	if (I == 0) {
		//RxSrSlider.Value := F div 100000;
		ui->TxSrLabel->setText(QString::number(F));
		FTxSrBlocked = false;
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Actual TX sampling rate is " + ui->TxSrLabel->text() + " Hz");
	} else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to get TX SampleRate: " + S);
	};
}


void TMainWindow::GetTxBW() {
	int I;
	QString S;
	uint32_t F;

	if (FDevice == NULL) return;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Read TX bandwidth...");
	FTxBwBlocked = true;
	I = FDevice->TxGetBwSync(&F);
	if (I == 0) {
		ui->TxBwLabel->setText(QString::number(F));
		//RxBwSlider.Value := F div 100000;
		FTxBwBlocked = false;
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Actual TX bandwidth is " + ui->TxBwLabel->text() + " Hz");
	} else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to get TX Bandwidth: " + S);
	};
}

void TMainWindow::GetTxVga1() {
	int I;
	QString S;
	int32_t F;

	if (FDevice == NULL) return;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Read TX VGA1...");
	FTxVga1Blocked = true;
	I = FDevice->TxGetVga1Sync(&F);
	if (I == 0) {
		ui->TxVga1Label->setText(QString::number(F));
		//RxVga1Slider.Value := F;
		FTxVga1Blocked = false;
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Actual TX VGA1 is " + ui->TxVga1Label->text());
	} else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to get TX VGA1: " + S);
	};
}

void TMainWindow::GetTxVga2() {
	int I;
	QString S;
	int32_t F;

	if (FDevice == NULL) return;

	if (ui->AllBox->isChecked())
		ui->Memo->appendPlainText("Read TX VGA2...");
	FTxVga2Blocked = true;
	I = FDevice->TxGetVga2Sync(&F);
	if (I == 0) {
		ui->TxVga2Label->setText(QString::number(F));
		//RxVga2Slider.Value := F;
		FTxVga2Blocked = false;
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Actual TX VGA2 is " + ui->TxVga2Label->text());
	} else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to get TX VGA2: " + S);
	};
}










void TMainWindow::on_DebugBox_clicked() {
	if (!ui->DebugBox->isChecked())
		ui->AllBox->setChecked(false);
	ui->AllBox->setEnabled(ui->DebugBox->isChecked());
}





void TMainWindow::OnRxData(void *Buffer, int BufferSize) {
	int i;
	//uint64_t n;

	TWCplx * FBufferPtr;
	PDCplx FSpectrBufferPtr;

	int samples_len_in;
	int samples_len_out;


//	WBufferPtr : PWCplx;

//	BufferRe : PSmallInt;
//	BufferRePtr : PSmallInt;
//	BufferIm : PSmallInt;
//	BufferImPtr : PSmallInt;

//	BufferDRePtr : PDCplx;
//	BufferDImPtr : PDCplx;

//	HotBufferPtr : PSmallInt;

//	double ReCorrect, ImCorrect;

	FBufferPtr = (TWCplx *)Buffer;
	//BufferPtr := Buffer;
	//BufferTmpPtr := Buffer;



	samples_len_in = BufferSize / 4;
	samples_len_out = samples_len_in / 5;


	if (ui->BsfBox->isChecked()) {
		double * TmpBuf = (double *)malloc(samples_len_in*sizeof(double));
		for (i = 0; i < samples_len_in; i++)
			TmpBuf[i] = FBufferPtr[i].Re;
		IIR_FILTER_I->Process(TmpBuf, samples_len_in);
		for (i = 0; i < samples_len_in; i++)
			FBufferPtr[i].Re = TmpBuf[i];

		for (i = 0; i < samples_len_in; i++)
			TmpBuf[i] = FBufferPtr[i].Im;
		IIR_FILTER_Q->Process(TmpBuf, samples_len_in);
		for (i = 0; i < samples_len_in; i++)
			FBufferPtr[i].Im = TmpBuf[i];

		free(TmpBuf);
	};

	if (ui->SignalBtn->isChecked()) {
		QVector<double> x(samples_len_out), y(samples_len_out);
		if (ui->IBox->isChecked()) {
			for (i = 0; i < samples_len_out; i++) {
				x[i] = i;
				y[i] = FBufferPtr[i].Re;
			};
			ui->customPlot->graph(0)->setData(x, y);
		};
		if (ui->QBox->isChecked()) {
			for (i = 0; i < samples_len_out; i++) {
				x[i] = i;
				y[i] = FBufferPtr[i].Im;
			};
			ui->customPlot->graph(1)->setData(x, y);
		};
		ui->customPlot->replot();
	} else {
		QVector<double> x(2048), y(2048);

//		if (ui->BsfBox->isChecked()) {
//			double * TmpBuf = (double *)malloc(2048*sizeof(double));
//			for (i = 0; i < 2048; i++)
//				TmpBuf[i] = FBufferPtr[i].Re;
//			IIR_FILTER->Process(TmpBuf, 2048);
//			for (i = 0; i < 2048; i++)
//				FBufferPtr[i].Re = TmpBuf[i];
//			for (i = 0; i < 2048; i++)
//				TmpBuf[i] = FBufferPtr[i].Im;
//			IIR_FILTER->Process(TmpBuf, 2048);
//			for (i = 0; i < 2048; i++)
//				FBufferPtr[i].Im = TmpBuf[i];

//			free(TmpBuf);
//		};

		if (FSpectrBuffer == NULL) {
			FSpectrBuffer = (PDCplx)malloc(2048*sizeof(TDCplx));
			HannCoefs = (double *)malloc(2048*sizeof(double));
			for (i = 0; i < 2048; i++)
				HannCoefs[i] = 0.5 * (1 - cos(2*M_PI*i/2047));
		};
		if (ui->HannBox->isChecked()) {
			for (i = 0; i < 2048; i++) {
				//double multiplier = 0.5 * (1 - cos(2*M_PI*i/2047));
				FBufferPtr[i].Re = HannCoefs[i] * FBufferPtr[i].Re;
				FBufferPtr[i].Im = HannCoefs[i] * FBufferPtr[i].Im;
			};
		};

		fft_nip_1((PWCplx)Buffer, FSpectrBuffer, 2048);
		//fft_nip_2(FloatBuf, FSpectrBuffer, 2048);
		FSpectrBufferPtr = FSpectrBuffer + 1024;
		double x_value = FRxFreq - FRxSR / 2 / 1000;
		//double to = FRxFreq + FRxSR / 2 / 1000;
		double delta = FRxSR;
		delta = delta / 2048 / 1000;
		//double x_value = from;
		for (i = 0; i < 1024; i++) {
			x[i] = x_value;
			if (FSpectrBufferPtr->Re < 0)
				FSpectrBufferPtr->Re *= -1;
			if (FSpectrBufferPtr->Im < 0)
				FSpectrBufferPtr->Im *= -1;
			y[i] = 20*log10((FSpectrBufferPtr->Re + FSpectrBufferPtr->Im)); //FSpectrBufferPtr->Re;
			FSpectrBufferPtr++;
			x_value += delta;
		};
		FSpectrBufferPtr = FSpectrBuffer;
		for (i = 1024; i < 2048; i++) {
			x[i] = x_value;
			if (FSpectrBufferPtr->Re < 0)
				FSpectrBufferPtr->Re *= -1;
			if (FSpectrBufferPtr->Im < 0)
				FSpectrBufferPtr->Im *= -1;
			y[i] = 20*log10((FSpectrBufferPtr->Re + FSpectrBufferPtr->Im)); //FSpectrBufferPtr->Re;
			FSpectrBufferPtr++;
			x_value += delta;
		};
		ui->customPlot->graph(0)->setData(x, y);
		//ui->customPlot->xAxis->setRange(from, to);
		ui->customPlot->xAxis->rescale(true);
		ui->customPlot->replot();
	};


	free(Buffer);
	return;

//	WBufferPtr := PWCplx(Buffer);

//	BufferDRePtr := BufferDRe;
//	BufferDImPtr := BufferDIm;

//	i := 0;
//	ReCorrect := Int(ReCorrectionEdit.Value);
//	ImCorrect := Int(ImCorrectionEdit.Value);
//	while i < FFT_WINDOW_SIZE do
//	begin
//		BufferDRePtr^.Re := WBufferPtr^.Re + ReCorrect;
//		BufferDRePtr^.Im := WBufferPtr^.Im + ImCorrect;
//		BufferDImPtr^.Re := WBufferPtr^.Im + ImCorrect;
//		BufferDImPtr^.Im := WBufferPtr^.Re + ReCorrect;

//		inc(BufferDRePtr);
//		inc(BufferDImPtr);
//		inc(WBufferPtr);
//		inc(i);
//	end;

//	if SpectrumRB.Checked then begin
//		inc(FFT_Delay);
//		if FFT_Delay >= 10 then begin
//			fft_nip_1(PDCplx(BufferDRe), PDCplx(SpectrDCplxRe), FFT_WINDOW_SIZE);
//			Fft.ProcessDCplx(BufferDRe, SpectrDCplxRe, FFT_WINDOW_SIZE);
//			//Fft.ProcessDCplx(BufferDIm, SpectrDCplxIm, FFT_WINDOW_SIZE);
//			ReDraw(nil);
//			FFT_Delay := 0;
//		end;
//	end
//	else
//		ReDraw(BufferDRe);


//	//if CB3.Checked then
//	//begin
//	//  FIR_High.Process(PChar(Buffer), samples_len_out);
//	//  FIR_Low1.Process(PChar(Buffer), samples_len_out);
//	//end;

//	if HotBuffer = nil then begin
//		GetMem(HotBuffer, Buffer_Size*2);
//		HotBufferPtr := HotBuffer;
//	end else begin
//		HotBufferPtr := HotBuffer;
//		inc(HotBufferPtr, HotBufferWritten div 2);
//	end;

//	{
//	GetMem(BufferRe, samples_len_in*SizeOf(SmallInt));
//	GetMem(BufferIm, samples_len_in*SizeOf(SmallInt));
//	BufferRePtr := BufferRe;
//	BufferImPtr := BufferIm;
//	SeparateChannels(Buffer, BufferRe, BufferIm, samples_len_in);
//	//FIR_Low1.Process(PChar(BufferIm), samples_len_in);

//	n := 0;
//	//WBufferPtr := PWCplx(Buffer);
//	while n < samples_len_in do begin
//		//CValue := WBufferPtr^;
//		//HotBufferPtr^ := CValue.Re; // + CValue.Im;
//		//HotBufferPtr^ := Round(32767*sin(2*Pi*2000*Phase/48000));
//		//Phase := Phase + 1;
//		//inc(WBufferPtr, 100);

//		HotBufferPtr^ := BufferRePtr^ + BufferImPtr^;
//		inc(BufferRePtr, 100);
//		inc(BufferImPtr, 100);

//		inc(HotBufferPtr);

//		inc(HotBufferWritten, 2);
//		if HotBufferWritten >= Buffer_Size*2 then begin
//			HotList.Add(HotBuffer);
//			HotBufferWritten := 0;
//			GetMem(HotBuffer, Buffer_Size*2);
//			HotBufferPtr := HotBuffer;
//			if HotList.Count > 4 then
//				StartAudio;
//		end;

//		inc(n, 100);
//	end;
//	FreeMem(BufferRe);
//	FreeMem(BufferIm);
//	}

//	FreeMemory(Buffer);

}



void TMainWindow::on_SpectrBtn_clicked() {
	//ui->customPlot->xAxis->setRange(0, 2048);
	QVector<double> x(2048), y(2048);
	double from = FRxFreq - FRxSR / 2 / 1000;
	//double to = FRxFreq + FRxSR / 2 / 1000;
	double delta = FRxSR;
	delta = delta / 2048 / 1000;
	double x_value = from;
	for (int i = 0; i < 2048; i++) {
		x[i] = x_value;
		y[i] = 0;
		x_value += delta;
	};
	ui->customPlot->graph(0)->setData(x, y);
	//ui->customPlot->xAxis->setRange(from, to);
	ui->customPlot->xAxis->rescale(true);
	//ui->customPlot->xAxis->rescale(false);

	ui->customPlot->yAxis->setRange(-5, 200);
	ui->customPlot->graph(1)->setVisible(false);
	ui->customPlot->replot();
}

void TMainWindow::on_SignalBtn_clicked() {
	ui->customPlot->xAxis->setRange(0, 4800);
	ui->customPlot->yAxis->setRange(-2048, 2048);
	ui->customPlot->graph(1)->setVisible(ui->QBox->isChecked());
	ui->customPlot->replot();
}


void TMainWindow::on_IBox_clicked() {
	ui->customPlot->graph(0)->setVisible(ui->IBox->isChecked());
	ui->customPlot->replot();
}

void TMainWindow::on_QBox_clicked() {
	ui->customPlot->graph(1)->setVisible(ui->QBox->isChecked());
	ui->customPlot->replot();
}
