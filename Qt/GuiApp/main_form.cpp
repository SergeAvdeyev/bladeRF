#include <windows.h>

#include "main_form.h"
#include "ui_main_form.h"

TMyApplication::TMyApplication(int &argc, char **argv) : QApplication(argc, argv) {

}


TPaintLabel::TPaintLabel(QWidget *parent) : QLabel(parent) {
	/*...*/

}

void TPaintLabel::paintEvent(QPaintEvent * event) {
	Q_UNUSED(event)
	/* paint somehting on your label*/
	//int w = width();
	//int h = height();

	//QPainter *painter = new QPainter(&pm);
	QPainter painter(this);
	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	painter.fillRect(rect(), Qt::black);

	QColor barColor(51, 204, 102);
//	QColor clipColor(255, 255, 0);

	// Draw the outline
	const QColor gridColor = barColor.darker();
	QPen gridPen(gridColor);
	painter.setPen(gridPen);
	painter.drawLine(rect().topLeft(), rect().topRight());
	painter.drawLine(rect().topRight(), rect().bottomRight());
	painter.drawLine(rect().bottomRight(), rect().bottomLeft());
	painter.drawLine(rect().bottomLeft(), rect().topLeft());

	QVector<qreal> dashes;
	dashes << 2 << 2;
	gridPen.setDashPattern(dashes);
	painter.setPen(gridPen);

	// Draw vertical lines between bars
	const int numHorizontalSections = 32;
	QLine VLine(rect().topLeft(), rect().bottomLeft());
	for (int i = 0; i < numHorizontalSections; ++i) {
		VLine.translate(rect().width()/numHorizontalSections, 0);
		painter.drawLine(VLine);
	};

	// Draw horizontal lines
	const int numVerticalSections = 10;
	QLine HLine(rect().topLeft(), rect().topRight());
	for (int i = 0; i < numVerticalSections; ++i) {
		HLine.translate(0, rect().height()/numVerticalSections);
		painter.drawLine(HLine);
	};

	barColor.setAlphaF(0.75);
	const QColor GraphColor = barColor.lighter();
	QPen GraphPen(GraphColor);
	painter.setPen(GraphPen);
	//clipColor.setAlphaF(0.75);

	int x = 2;
	int i = 1;
	while (x < 510) {
		painter.drawLine(x - 2, height() - FSpectr[i - 1], x, height() - FSpectr[i]);
		i++;
		x += 2;
	};
	painter.end();
	QLabel::paintEvent(event);
}



TMainForm::TMainForm(TMyApplication * ApplicationPtr, QWidget *parent) : QMainWindow(parent), ui(new Ui::TMainForm) {
	FApplicationPtr = ApplicationPtr;
	ui->setupUi(this);
	FFocusedWidget = NULL;

	FPaintLabel = new TPaintLabel(this);
	FPaintLabel->resize(512, 400);
	FPaintLabel->move(300, 100);
	FPaintLabel->setText("");
	this->layout()->addWidget(FPaintLabel);

	FRxFreq = 345678; // Frequency in kHz
	SetRxFreqLabels();
	UpdateRxFreqLabels();

	FTxFreq = 445678; // Frequency in kHz
	SetTxFreqLabels();
	UpdateTxFreqLabels();

	idx = 0;
	FTimer = new QTimer;
	FTimer->setInterval(50);
	connect(FTimer, SIGNAL(timeout()), this, SLOT(OnTimer()));
	FTimer->start();
}

TMainForm::~TMainForm() {
	delete ui;
}

bool TMainForm::eventFilter(QObject *obj, QEvent *event) {
	(void)obj;
	(void)event;
	bool FResult = false;
//	int FEventType = event->type();
//	if (obj == ui->RxLabel_MHz_100) {
//		QMouseEvent *mouseEvent = NULL;
//		switch (FEventType) {
//			case QEvent::MouseMove:
//				mouseEvent = static_cast<QMouseEvent*>(event);
//				ui->DebugLabel_1->setText(QString::number(mouseEvent->pos().x()));
//				FResult = true;
//				break;
//			case QEvent::FocusIn:
//				RxLabelMhz_100_focus = true;
//				FResult = true;
//				break;
//			case QEvent::FocusOut:
//				RxLabelMhz_100_focus = false;
//				FResult = true;
//				break;
//			default:
//				break;
//		};
//	};
	return FResult;
}


bool TMainForm::nativeEvent(const QByteArray &eventType, void *message, long *result) {
	(void)result;
	(void)eventType;
	MSG * FMessage = (MSG *)message;
	//unsigned short fwKeys;
	short zDelta;
	//unsigned short X;
	//unsigned short Y;
//	QWidget * FWidget = NULL;
//	QRegExp FRegExp;
//	QString FMultiplier;
//	QString FObjectName;
	bool FResult = false;

	PFreqLabelDescr F;
	//int FDigit;
	int i;
	u_int64 FMult;

	switch (FMessage->message) {
		case WM_MOUSEWHEEL:
			zDelta = GET_WHEEL_DELTA_WPARAM(FMessage->wParam);
			F = FRxLabels.value((QLabel *)FFocusedWidget);
			if (F != NULL) {
				FMult = 1;
				for (i = 0; i < F->Digit; i++)
					FMult *= 10;
				if (zDelta > 0)
					FRxFreq += FMult;
				else
					FRxFreq -= FMult;
				if (FRxFreq < 300000)
					FRxFreq = 300000;
				if (FRxFreq > 2600000)
					FRxFreq = 2600000;
				UpdateRxFreqLabels();
				return true;
			};
			F = FTxLabels.value((QLabel *)FFocusedWidget);
			if (F != NULL) {
				FMult = 1;
				for (i = 0; i < F->Digit; i++)
					FMult *= 10;
				if (zDelta > 0)
					FTxFreq += FMult;
				else
					FTxFreq -= FMult;
				if (FTxFreq < 300000)
					FTxFreq = 300000;
				if (FTxFreq > 2600000)
					FTxFreq = 2600000;
				UpdateTxFreqLabels();
				return true;
			};

			break;
		case WM_MOUSEMOVE:
			FFocusedWidget = QWidget::childAt(GET_X_LPARAM(FMessage->lParam), GET_Y_LPARAM(FMessage->lParam));
			if (QWidget::childAt(GET_X_LPARAM(FMessage->lParam), GET_Y_LPARAM(FMessage->lParam)) == ui->RxLabel_MHz_100)
				ui->DebugLabel_1->setText("111");
			else
				ui->DebugLabel_1->setText("222");
			break;
		default:
			break;
	};
	return FResult;
}

//void TMainForm::paintEvent(QPaintEvent * event) {
//	(void)event;
//	QRect R = event->rect();
//	ui->DebugLabel_1->setText(QString::number(R.left()));
//	ui->DebugLabel_2->setText(QString::number(ui->label_5->geometry().left()));
//	if (R.left() == ui->label_5->geometry().left())
//		ui->label_5->clear();
//}

void TMainForm::SetRxFreqLabels() {
	PFreqLabelDescr FFreqLabelDescr;

	int i = 0;
	while (i < 7) {
		FFreqLabelDescr = new TFreqLabelDescr;
		FFreqLabelDescr->Digit = i;
		if (i == 0) {
			FFreqLabelDescr->Module = 10;
			FFreqLabelDescr->Multiplier = 1;
			FRxLabels.insert(ui->RxLabel_kHz_1, FFreqLabelDescr);
		} else if (i == 1) {
			FFreqLabelDescr->Module = 100;
			FFreqLabelDescr->Multiplier = 10;
			FRxLabels.insert(ui->RxLabel_kHz_10, FFreqLabelDescr);
		} else if (i == 2) {
			FFreqLabelDescr->Module = 1000;
			FFreqLabelDescr->Multiplier = 100;
			FRxLabels.insert(ui->RxLabel_kHz_100, FFreqLabelDescr);
		} else if (i == 3) {
			FFreqLabelDescr->Module = 10000;
			FFreqLabelDescr->Multiplier = 1000;
			FRxLabels.insert(ui->RxLabel_MHz_1, FFreqLabelDescr);
		} else if (i == 4) {
			FFreqLabelDescr->Module = 100000;
			FFreqLabelDescr->Multiplier = 10000;
			FRxLabels.insert(ui->RxLabel_MHz_10, FFreqLabelDescr);
		} else if (i == 5) {
			FFreqLabelDescr->Module = 1000000;
			FFreqLabelDescr->Multiplier = 100000;
			FRxLabels.insert(ui->RxLabel_MHz_100, FFreqLabelDescr);
		} else if (i == 6) {
			FFreqLabelDescr->Module = 10000000;
			FFreqLabelDescr->Multiplier = 1000000;
			FRxLabels.insert(ui->RxLabel_GHz_1, FFreqLabelDescr);
		};
		i++;
	};

}

void TMainForm::UpdateRxFreqLabels() {
	PFreqLabelDescr F;

	foreach (QLabel * L, FRxLabels.keys()) {
		F = FRxLabels.value(L);
		F->LabelValue = (FRxFreq % F->Module) / F->Multiplier;
		L->setText(QString::number(F->LabelValue));
	};
}



void TMainForm::SetTxFreqLabels() {
	PFreqLabelDescr FFreqLabelDescr;

	int i = 0;
	while (i < 7) {
		FFreqLabelDescr = new TFreqLabelDescr;
		FFreqLabelDescr->Digit = i;
		if (i == 0) {
			FFreqLabelDescr->Module = 10;
			FFreqLabelDescr->Multiplier = 1;
			FTxLabels.insert(ui->TxLabel_kHz_1, FFreqLabelDescr);
		} else if (i == 1) {
			FFreqLabelDescr->Module = 100;
			FFreqLabelDescr->Multiplier = 10;
			FTxLabels.insert(ui->TxLabel_kHz_10, FFreqLabelDescr);
		} else if (i == 2) {
			FFreqLabelDescr->Module = 1000;
			FFreqLabelDescr->Multiplier = 100;
			FTxLabels.insert(ui->TxLabel_kHz_100, FFreqLabelDescr);
		} else if (i == 3) {
			FFreqLabelDescr->Module = 10000;
			FFreqLabelDescr->Multiplier = 1000;
			FTxLabels.insert(ui->TxLabel_MHz_1, FFreqLabelDescr);
		} else if (i == 4) {
			FFreqLabelDescr->Module = 100000;
			FFreqLabelDescr->Multiplier = 10000;
			FTxLabels.insert(ui->TxLabel_MHz_10, FFreqLabelDescr);
		} else if (i == 5) {
			FFreqLabelDescr->Module = 1000000;
			FFreqLabelDescr->Multiplier = 100000;
			FTxLabels.insert(ui->TxLabel_MHz_100, FFreqLabelDescr);
		} else if (i == 6) {
			FFreqLabelDescr->Module = 10000000;
			FFreqLabelDescr->Multiplier = 1000000;
			FTxLabels.insert(ui->TxLabel_GHz_1, FFreqLabelDescr);
		};
		i++;
	};

}

void TMainForm::UpdateTxFreqLabels() {
	PFreqLabelDescr F;

	foreach (QLabel * L, FTxLabels.keys()) {
		F = FTxLabels.value(L);
		F->LabelValue = (FTxFreq % F->Module) / F->Multiplier;
		L->setText(QString::number(F->LabelValue));
	};
}



void TMainForm::OnTimer() {
	int i;
	for (i = 0; i < 256; i++) {
		if (i == idx)
			FPaintLabel->FSpectr[i] = 200;
		else
			FPaintLabel->FSpectr[i] = 2;
	};

	idx++;
	if (idx >= 256)
		idx = 0;

	FPaintLabel->update();
}

