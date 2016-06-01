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

	FDeviceThread = NULL;

	ui->StartRxBtn->setEnabled(false);
	ui->StartTxBtn->setEnabled(false);

	SetRxFreqLabels();
	UpdateRxFreqLabels();
	SetTxFreqLabels();
	UpdateTxFreqLabels();





	ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
								  QCP::iSelectLegend | QCP::iSelectPlottables);
	//ui->customPlot->xAxis->setRange(-8, 8);
	ui->customPlot->xAxis->setRange(0, 441000);
	ui->customPlot->xAxis->setNumberFormat("f");
	ui->customPlot->xAxis->setNumberPrecision(0);
	ui->customPlot->xAxis->setTickLabelRotation(60);
	ui->customPlot->yAxis->setRange(-5, 5);
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

	addRandomGraph();

	// connect slot that ties some axis selections together (especially opposite axes):
	connect(ui->customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
	// connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
	connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
	connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

	// make bottom and left axes transfer their ranges to top and right axes:
	connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

	// connect some interaction slots:
	connect(ui->customPlot, SIGNAL(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)), this, SLOT(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)));
	connect(ui->customPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
	connect(ui->customPlot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));

	// connect slot that shows a message in the status bar when a graph is clicked:
	connect(ui->customPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*)));

	// setup policy and connect slot for context menu popup:
	ui->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

	FDevice = new TBladeRfDevice(this);
	connect(this, SIGNAL(MessageToDevice(uint16_t,int64_t,int64_t)), FDevice, SLOT(MessageToDevice(uint16_t,int64_t,int64_t)));
	connect(FDevice, SIGNAL(MessageFromDevice(uint16_t,int64_t,int64_t)), this, SLOT(MessageFromDevice(uint16_t,int64_t,int64_t)));

	readSettings();

//	FTestThread = new TTestThread(this);
//	FTestThread->start();

//	if (FDeviceThread == NULL) {
//		FDeviceThread = new QThread(FDevice);

//		connect(FDeviceThread, SIGNAL(started()), FDevice, SLOT(Execute()));
//		connect(FDeviceThread, SIGNAL(finished()), this, SLOT(OnDeviceThreadFinished()));
//		FDevice->moveToThread(FDeviceThread);
//		FDeviceThread->start();
//	};

}

TMainWindow::~TMainWindow() {

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
	//QSettings settings("Moose Soft", "Clipper");

	FSettings->beginGroup("MainWindow");
	FSettings->setValue("size", size());
	FSettings->setValue("pos", pos());
	FSettings->endGroup();
	FSettings->sync();
}

void TMainWindow::readSettings() {
	//QSettings settings("Moose Soft", "Clipper");

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

	connect(RxLabel_GHz_1, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(RxLabel_MHz_100, SIGNAL(mouseMove(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(RxLabel_MHz_10, SIGNAL(mouseMove(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(RxLabel_MHz_1, SIGNAL(mouseMove(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(RxLabel_kHz_100, SIGNAL(mouseMove(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(RxLabel_kHz_10, SIGNAL(mouseMove(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(RxLabel_kHz_1, SIGNAL(mouseMove(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));


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

	connect(TxLabel_GHz_1, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(TxLabel_MHz_100, SIGNAL(mouseMove(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(TxLabel_MHz_10, SIGNAL(mouseMove(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(TxLabel_MHz_1, SIGNAL(mouseMove(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(TxLabel_kHz_100, SIGNAL(mouseMove(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(TxLabel_kHz_10, SIGNAL(mouseMove(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));
	connect(TxLabel_kHz_1, SIGNAL(mouseMove(QWheelEvent*)), this, SLOT(FreqLabelWheel(QWheelEvent*)));


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



// Handle messages from Device thread
void TMainWindow::MessageFromDevice(uint16_t Msg, int64_t WParam, int64_t LParam) {
	switch (Msg) {
		case FromDev_Connect:
			OnConnected(WParam, LParam);
			break;
		case FromDev_Disconnect:
			OnDisconnected(WParam, LParam);
			break;
		case FromDev_RxStart:
			OnRxStart(WParam, LParam);
			break;
		case FromDev_RxStop:
			OnRxStop(WParam, LParam);
			break;

		case FromDev_RxProcessData:
			OnRxData(WParam, LParam);
			break;
		default:
			break;
	};
}



void TMainWindow::OnConnected(int64_t WParam, int64_t LParam) {
	Q_UNUSED(LParam);
	QString S;

	if (WParam != 0) {
		if (ui->DebugBox->isChecked()) {
			S = QString::fromUtf8(bladerf_strerror(WParam));
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
	GetRxLna();
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

void TMainWindow::OnDisconnected(int64_t WParam, int64_t LParam) {
	Q_UNUSED(WParam);
	Q_UNUSED(LParam);

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

void TMainWindow::OnRxStart(int64_t WParam, int64_t LParam) {
	Q_UNUSED(LParam);
	QString S;

	if (WParam == 0) {
		ui->StartRxBtn->setText("Stop RX");
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("RX started");

//		if (FDeviceThread == NULL) {
//			FDeviceThread = new QThread(NULL);

//			connect(FDeviceThread, SIGNAL(started()), FDevice, SLOT(Execute()));
//			connect(FDeviceThread, SIGNAL(finished()), this, SLOT(OnDeviceThreadFinished()));
//			FDevice->moveToThread(FDeviceThread);
//			FDeviceThread->start();
//		};
	} else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(WParam));
		ui->Memo->appendPlainText("Failed to start RX: " + S);
	};
	ui->StartRxBtn->setEnabled(true);
}


void TMainWindow::OnRxStop(int64_t WParam, int64_t LParam) {
	Q_UNUSED(LParam);
	QString S;

	if (WParam == 0) {
		//StopAudio;
		ui->StartRxBtn->setText("Start RX");
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("RX stopped");
	} else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(WParam));
		ui->Memo->appendPlainText("Failed to stop RX: " + S);
	};
}

void TMainWindow::OnTxStart(int64_t WParam, int64_t LParam) {
	Q_UNUSED(LParam);
	QString S;

	if (WParam == 0) {
		ui->StartTxBtn->setText("Stop TX");
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("TX started");
		//ui->Timer1.Enabled = true;
	} else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(WParam));
		ui->Memo->appendPlainText("Failed to start TX: " + S);
	};
}

void TMainWindow::OnTxStop(int64_t WParam, int64_t LParam) {
	Q_UNUSED(LParam);
	QString S;

	if (WParam == 0) {
		ui->StartTxBtn->setText("Start TX");
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("TX stopped");
	} else if (ui->DebugBox->isChecked()) {
		S = QString::fromUtf8(bladerf_strerror(WParam));
		ui->Memo->appendPlainText("Failed to stop TX: " + S);
	};
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















void TMainWindow::titleDoubleClick(QMouseEvent* event, QCPPlotTitle* title) {
	Q_UNUSED(event)
	// Set the plot title by double clicking on it
	bool ok;
	QString newTitle = QInputDialog::getText(this, "QCustomPlot example", "New plot title:", QLineEdit::Normal, title->text(), &ok);
	if (ok) {
		title->setText(newTitle);
		ui->customPlot->replot();
	};
}

void TMainWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part) {
	// Set an axis label by double clicking on it
	if (part == QCPAxis::spAxisLabel) { // only react when the actual axis label is clicked, not tick label or axis backbone
		bool ok;
		QString newLabel = QInputDialog::getText(this, "QCustomPlot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
		if (ok) {
			axis->setLabel(newLabel);
			ui->customPlot->replot();
		};
	};
}

void TMainWindow::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item) {
	// Rename a graph by double clicking on its legend item
	Q_UNUSED(legend)
	if (item) { // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
		QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
		bool ok;
		QString newName = QInputDialog::getText(this, "QCustomPlot example", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
		if (ok) {
			plItem->plottable()->setName(newName);
			ui->customPlot->replot();
		};
	};
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
  for (int i=0; i<ui->customPlot->graphCount(); ++i) {
	QCPGraph *graph = ui->customPlot->graph(i);
	QCPPlottableLegendItem *item = ui->customPlot->legend->itemWithPlottable(graph);
	if (item->selected() || graph->selected()) {
	  item->setSelected(true);
	  graph->setSelected(true);
	}
  }
}

void TMainWindow::mousePress()
{
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

void TMainWindow::addRandomGraph() {
	int sample_rate = 44100;
	//int n = 2048; // number of points in graph
	QCPScatterStyle ss;
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

//	for (int j = 0; j < 1000; j++) {
//		from = 2048*j;
//		to = 2048*j + 2048;
//		if (to > sample_rate*10) {
//			to = sample_rate*10;
//			last = true;
//		};
//		for (i = from; i < to; i++) {
//			x[i] = i;

//			angle = i % sample_rate;
//			y[i] = 5*sin((interval*angle) + phase);
//		};
//		if (last) break;
//	};

//	angle = (i - 1) % n;


	ui->customPlot->addGraph();
	ui->customPlot->graph()->setName(QString("New graph %1").arg(ui->customPlot->graphCount() - 1));
	ui->customPlot->graph()->setData(x, y);
	//ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)(rand()%5 + 1));
	ui->customPlot->graph()->setLineStyle(QCPGraph::lsLine);
	ss = ui->customPlot->graph()->scatterStyle();
	//if (rand()%100 > 50)
	//	ui->customPlot->graph()->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(rand()%14 + 1)));
	QPen graphPen;
	graphPen.setColor(QColor(rand()%245 + 10, rand()%245 + 10, rand()%245 + 10));
	//graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
	graphPen.setWidthF(1);
	//ui->customPlot->set
	ui->customPlot->graph()->setPen(graphPen);
	ui->customPlot->replot();
}

void TMainWindow::removeSelectedGraph() {
	if (ui->customPlot->selectedGraphs().size() > 0) {
		ui->customPlot->removeGraph(ui->customPlot->selectedGraphs().first());
		ui->customPlot->replot();
	};
}

void TMainWindow::removeAllGraphs() {
	ui->customPlot->clearGraphs();
	ui->customPlot->replot();
}

void TMainWindow::contextMenuRequest(QPoint pos) {
	QMenu *menu = new QMenu(this);
	menu->setAttribute(Qt::WA_DeleteOnClose);

	if (ui->customPlot->legend->selectTest(pos, false) >= 0) {// context menu on legend requested
		menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
		menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
		menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
		menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
		menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
	} else { // general context menu on graphs requested
		menu->addAction("Add random graph", this, SLOT(addRandomGraph()));
		if (ui->customPlot->selectedGraphs().size() > 0)
			menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
		if (ui->customPlot->graphCount() > 0)
		menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
	};

	menu->popup(ui->customPlot->mapToGlobal(pos));
}

void TMainWindow::moveLegend() {
	if (QAction* contextAction = qobject_cast<QAction*>(sender())) { // make sure this slot is really called by a context menu action, so it carries the data we need
		bool ok;
		int dataInt = contextAction->data().toInt(&ok);
		if (ok) {
			ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
			ui->customPlot->replot();
		};
	};
}

void TMainWindow::graphClicked(QCPAbstractPlottable *plottable) {
	ui->statusBar->showMessage(QString("Clicked on graph '%1'.").arg(plottable->name()), 1000);
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

	ui->OpenDevBtn->setEnabled(false);
	if (ui->OpenDevBtn->text() == "Connect") {
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Connecting to device... ");
		emit MessageToDevice(ToDevMsg_Connect, 0, 0);
	} else {
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Disconnecting ... ");
		emit MessageToDevice(ToDevMsg_Disconnect, 0, 0);
	};
}

void TMainWindow::on_StartRxBtn_clicked() {
	if (FDevice == NULL) return;

	ui->StartRxBtn->setEnabled(false);
	if (ui->StartRxBtn->text() == "Start RX") {
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Starting RX ... ");
		emit MessageToDevice(ToDevMsg_RxStart, 0, 0);
//		if (FDeviceThread == NULL) {
//			FDeviceThread = new QThread(FDevice);

//			connect(FDeviceThread, SIGNAL(started()), FDevice, SLOT(Execute()));
//			connect(FDeviceThread, SIGNAL(finished()), this, SLOT(OnDeviceThreadFinished()));
//			FDevice->moveToThread(FDeviceThread);
//			FDeviceThread->start();
//		};
	} else {
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Stopping RX ... ");
		emit MessageToDevice(ToDevMsg_RxStop, 0, 0);
	};
}

void TMainWindow::on_StartTxBtn_clicked() {
	if (FDevice == NULL) return;

	ui->StartTxBtn->setEnabled(false);
	if (ui->StartTxBtn->text() == "Start TX") {
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Starting TX ... ");
		emit MessageToDevice(ToDevMsg_TxStart, 0, 0);
	} else {
		if (ui->AllBox->isChecked())
			ui->Memo->appendPlainText("Stopping TX ... ");
		emit MessageToDevice(ToDevMsg_TxStop, 0, 0);
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
	} else {
		S = QString::fromUtf8(bladerf_strerror(I));
		ui->Memo->appendPlainText("Failed to set RX Frequency: " + S);
	};
}

/*
{
procedure TForm1.GetRxFreq;
var I : Integer;
	S : String;
	F : Integer;
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
	Memo.Lines.Add('Read RX frequency... ');

  I := FDeviceThread.RxGetFreqSync(F);

  if I = 0 then begin
	FRxFreq := F;
	UpdateRxFreqLabels;
	if AllBox.Checked then
	  Memo.Lines.Add('Actual RX Frequency is ' + IntToStr(FRxFreq) + ' êÃö')
  end
  else if DebugBox.Checked then begin
	S := StrPas(bladerf_strerror(I));
	Memo.Lines.Add('Failed to get RX Frequency: ' + S);
  end;
end;
}
*/


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

void TMainWindow::GetRxLna() {
//	int I;
//	QString S;
//	uint32_t F;

	return;
//	if FDeviceThread = nil then Exit;

//	if AllBox.Checked then
//	Memo.Lines.Add('Read RX LNA...');
//	FRxLnaBlocked := True;
//	I := FDeviceThread.RxGetLnaSync(F);
//	if I = 0 then begin
//	RxLnaLabel.Value := F;
//	//RxBwSlider.Value := F div 100000;
//	FRxLnaBlocked := False;
//	if AllBox.Checked then
//	Memo.Lines.Add('Actual RX LNA is ' + IntToStr(F) + ' dB');
//	end
//	else if DebugBox.Checked then begin
//	S := StrPas(bladerf_strerror(I));
//	Memo.Lines.Add('Failed to get RX LNA: ' + S);
//	end;
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

/*
{
procedure TForm1.GetTxFreq;
var I : Integer;
	S : String;
	F : Integer;
begin
  if FDeviceThread = nil then Exit;

  if AllBox.Checked then
	Memo.Lines.Add('Read TX frequency... ');

  I := FDeviceThread.TxGetFreqSync(F);

  if I = 0 then begin
	FTxFreq := F;
	UpdateTxFreqLabels;
	if AllBox.Checked then
	  Memo.Lines.Add('Actual TX Frequency is ' + IntToStr(FTxFreq) + ' êÃö')
  end
  else if DebugBox.Checked then begin
	S := StrPas(bladerf_strerror(I));
	Memo.Lines.Add('Failed to get TX Frequency: ' + S);
  end;
end;
}
*/

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





void TMainWindow::OnRxData(int64_t WParam, int64_t LParam) {
	//int i;
	//uint64_t n;

	int16_t * Buffer;

	//uint32_t samples_len_in;

//	WBufferPtr : PWCplx;

//	BufferRe : PSmallInt;
//	BufferRePtr : PSmallInt;
//	BufferIm : PSmallInt;
//	BufferImPtr : PSmallInt;

//	BufferDRePtr : PDCplx;
//	BufferDImPtr : PDCplx;

//	HotBufferPtr : PSmallInt;

//	double ReCorrect, ImCorrect;

	Buffer = (int16_t *)(void *)WParam;
	//BufferPtr := Buffer;
	//BufferTmpPtr := Buffer;

	free(Buffer);
	return;


	//samples_len_in = LParam / 4;
	//samples_len_out := samples_len_in div 2;

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




void TMainWindow::OnDeviceThreadFinished() {
	if (FDeviceThread == NULL) return;

	delete FDeviceThread;
	FDeviceThread = NULL;
}
