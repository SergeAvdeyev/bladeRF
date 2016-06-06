
#include "tx_thread.h"

#define NUM_TAPS        40     /* Number of Filter TAPS */
#define BUFFER_SIZE     1000   /* The size of the buffer */
#define CARRIER_SIZE    8   /* The size of the carrier for each period*/
#define HALF_BUFF_SIZE  500   /* Half of the message buffer*/
#define NUM_SAMPLES     16   /* The number of samples*/
#define SAMPLE_RANGE    8000   /* The size of modulated signal*/

#define multFact  23170       /*   SQRT(2) / 2   in Q15 format */

/* The RRC filter coefficients */
const short _bcoeff[] = {
	184,
	278,
	331,
	320,
	231,
	67,
	-156,
	-403,
	-629,
	-778,
	-798,
	-648,
	-307,
	223,
	912,
	1703,
	2520,
	3278,
	3892,
	4292,
	4431,
	4292,
	3892,
	3278,
	2520,
	1703,
	912,
	223,
	-307,
	-648,
	-798,
	-778,
	-629,
	-403,
	-156,
	67,
	231,
	320,
	331,
	278,
	184
};

short _Q15(int a) {
	if (a >= 0)
		return (a >> 15) ? (a >> 15) : a;
	else
		return (-a >> 15) ? -(-a >> 15) : a;
}

void _fir_filter(short signal[], int size) {
	short R_in[NUM_TAPS]; 		/* Input samples R_in[0] most recent, R_in[NUM_TAPS-1]  oldest */
	int acc;
	int prod;
	int i, j;

	for (j = 0; j < NUM_TAPS; ++j)
		R_in[j] = 0;

	for (j = 0; j < size; ++j) {
		R_in[0] =  signal[j] ;         		/* Update most recent sample */

		acc = 0;

		for (i = 0; i < NUM_TAPS; i++) {
			prod = (int)_bcoeff[i] * R_in[i];
			acc += prod;
		};

		signal[j] = acc >> 15;

		for (i = NUM_TAPS-1; i > 0; i--)         	/* Shift delay samples */
			R_in[i]=R_in[i-1];
	};
}

void _fir_filter_2(TWCplx * signal, int size) {
	short R_in_Re[NUM_TAPS]; 		/* Input samples R_in[0] most recent, R_in[NUM_TAPS-1]  oldest */
	short R_in_Im[NUM_TAPS]; 		/* Input samples R_in[0] most recent, R_in[NUM_TAPS-1]  oldest */
	int acc_Re;
	int acc_Im;
	int prod_Re;
	int prod_Im;
	int i, j;

	for (j = 0; j < NUM_TAPS; ++j) {
		R_in_Re[j] = 0;
		R_in_Im[j] = 0;
	};

	for (j = 0; j < size; ++j) {
		R_in_Re[0] = signal[j].Re;         		/* Update most recent sample */
		R_in_Im[0] = signal[j].Im;         		/* Update most recent sample */

		acc_Re = 0;
		acc_Im = 0;

		for (i = 0; i < NUM_TAPS; i++) {
			prod_Re = (int)_bcoeff[i] * R_in_Re[i];
			acc_Re += prod_Re;

			prod_Im = (int)_bcoeff[i] * R_in_Im[i];
			acc_Im += prod_Im;
		};

		signal[j].Re = acc_Re >> 15;
		signal[j].Im = acc_Im >> 15;

		for (i = NUM_TAPS-1; i > 0; i--) {         	/* Shift delay samples */
			R_in_Re[i] = R_in_Re[i - 1];
			R_in_Im[i] = R_in_Im[i - 1];
		};
	};
}

void _encodeQPSK(short message[], short signal[], int size) {
//void encodeQPSK(QVector<short> &message, QVector<short> &signal, int size) {
	short Ik[HALF_BUFF_SIZE], Qk[HALF_BUFF_SIZE];
	short Isam[SAMPLE_RANGE], Qsam[SAMPLE_RANGE];
	int i, j, halfsize, Sample_Range;
	int oldI, oldQ, newI, newQ;
	//int replicate_carrier;
	short tbc1, tbc2, t1 = 0, t2 = 0;

	halfsize     = size >> 1;
	Sample_Range = NUM_SAMPLES * halfsize;

	short tmp;

	// Remap to {-1, 1}
	for (i = 0; i < size; ++i) {
		tmp = message[i];
		tmp = tmp << 1;
		tmp -= 1;
		message[i] = tmp;
		//message[i] = (message[i] << 1) - 1;
	};

	oldI = 1;
	oldQ = 0;

	// Returns the phase shifts for pi/4 DQPSK from the input binary data stream
	for (i = 0; i < halfsize; ++i) {
		tbc1 = message[i * 2];
		tbc2 = message[i * 2 + 1];

		newI = _Q15( (oldI * tbc1 - oldQ * tbc2) * multFact );
		newQ = _Q15( (oldQ * tbc1 + oldI * tbc2) * multFact );

		oldI = newI;
		oldQ = newQ;

		Ik[i] = newI;
		Qk[i] = newQ;
	};

	j = 0;

	for (i = 0; i < Sample_Range; ++i) {
		if (i % NUM_SAMPLES == 0) {
			Isam[i] = Ik[j];
			Qsam[i] = Qk[j];
			j = j + 1;
		} else
			Isam[i] = Qsam[i] = 0;
		//signal[i] = Isam[i];
	};

	_fir_filter( Isam, Sample_Range );
	_fir_filter( Qsam, Sample_Range );

	// Upmix with carrier
	for (j = 0; j < Sample_Range; ++j) {
		//replicate_carrier = j % CARRIER_SIZE;
		//t1 = Q15(Isam[j] * Icarrier[replicate_carrier]);
		//t2 = Q15(Qsam[j] * Qcarrier[replicate_carrier]);
		signal[j] =  t1 + t2;
	};
}









TTxThread::TTxThread(struct bladerf * Device) : QObject(0) {
	FDevice = Device;
	FTerminated = false;
	IIR_FILTER_I = new TButtFilter;
	IIR_FILTER_Q = new TButtFilter;
	IIR_FILTER_I->InitLowPass(4, 2000000, 20000);
	IIR_FILTER_Q->InitLowPass(4, 2000000, 20000);

	float phase_offset = M_PI/30;
	float frequency_offset = 2*M_PI*((double)100000/2000000); // 0.1f;

	Modem = modem_create(LIQUID_MODEM_QPSK);

	nco_tx = nco_crcf_create(LIQUID_VCO);
	nco_crcf_set_phase(nco_tx, phase_offset);
	nco_crcf_set_frequency(nco_tx, frequency_offset);
}

TTxThread::~TTxThread() {
	PTxDataItem F;
	while (DataList.count() > 0) {
		F = DataList.takeFirst();
		free(F->Buffer);
		free(F);
	};
}

void TTxThread::Lock() {
	FMutex.lock();
}

void TTxThread::Unlock() {
	FMutex.unlock();
}

int TTxThread::TxInitSync() {
	uint32_t num_buffers;
	uint32_t buffer_size;  // Must be a multiple of 1024
	uint32_t num_transfers;
	uint32_t timeout_ms;

	num_buffers   = 32;     // 16
	buffer_size   = 32768;  // 4096
	num_transfers = 16;     // num_buffers / 2
	timeout_ms    = 3500;

	int Result = bladerf_sync_config(FDevice,
									 BLADERF_MODULE_TX,
									 BLADERF_FORMAT_SC16_Q11,
									 num_buffers,
									 buffer_size,
									 num_transfers,
									 timeout_ms);
	return Result;
}

bool TTxThread::CanSend() {
	int Result;

	Lock();
	Result = DataList.count();
	Unlock();
	return Result < 10;
}

int TTxThread::AddBuffer(void * Buffer, int BufferSize) {
	PTxDataItem FDataItem;

	Lock();
//	if (DataList.count() >= 10) {
//		Unlock();
//		return 1;
//	};
	FDataItem = (PTxDataItem)malloc(sizeof(TTxDataItem));
	FDataItem->Buffer = Buffer;
	FDataItem->BufferSize = BufferSize;
	DataList.append(FDataItem);
	Unlock();
	return 0;
}

PTxDataItem TTxThread::GetBuffer() {
	PTxDataItem FDataItem;

	Lock();
	if (DataList.count() == 0) {
		Unlock();
		return NULL;
	};
	FDataItem = DataList.takeFirst();
	Unlock();
	return FDataItem;
}


int TTxThread::Process() {
	int Status;
//	PTxDataItem FDataItem;

//	FDataItem = GetBuffer();
//	if (FDataItem == NULL) {
//		QThread::msleep(10);
//		return 0;
//	};
//	Status = bladerf_sync_tx(FDevice, FDataItem->Buffer, TxSamplesLen, NULL, 5000);
//	free(FDataItem->Buffer);
//	free(FDataItem);
//	return Status;

	//char * FBufferTmp;

	liquid_float_complex x, r;

	for (int i = 0; i < TxSamplesLen; i++) {  // Fill 1 second of signal
		if (i % 200 == 0) {
			// modulate
			//sym_out = 0;
			modem_modulate(Modem, sym_out, &x);
			sym_out = (sym_out + 1) % 4;
		};
		nco_crcf_mix_up(nco_tx, x, &r);
		nco_crcf_step(nco_tx);

		FTxBuffer[i].Re = r.real()*200;
		FTxBuffer[i].Im = r.imag()*200;
	};


//	bzero(FTxBufferRe, TxSamplesLen*sizeof(double));
//	bzero(FTxBufferIm, TxSamplesLen*sizeof(double));
//	int i = 0;
//	int16_t a = 1024;
//	while (i < TxSamplesLen) {
//		FTxBufferRe[i] = a;
//		FTxBufferIm[i] = -a;
//		a *= -1;
//		i += 50;
//	};
//	//IIR_FILTER_I->Process(FTxBufferRe, FTxSamplesLen);
//	//IIR_FILTER_Q->Process(FTxBufferIm, FTxSamplesLen);
//	i = 0;
//	while (i < TxSamplesLen) {
//		FTxBuffer[i].Re = FTxBufferRe[i];
//		FTxBuffer[i].Im = FTxBufferIm[i];
//		i++;
//	};

	//_fir_filter_2(FTxBuffer, FTxSamplesLen);
	Status = bladerf_sync_tx(FDevice, FTxBuffer, TxSamplesLen, NULL, 5000);
	if (Status != 0) {
		return Status;
		//emit MessageFromDevice(FromDev_RxProcessError, Status, 0);
	};
	//FBufferTmp = (char *)malloc(FTxBufferSize);
	//memcpy(FBufferTmp, FTxBuffer, FTxBufferSize);
	//emit NewRxData(FBufferTmp, FRxBufferSize);
	//FBladeRfDevice->OnRxData(FBufferTmp, FRxBufferSize);
	return 0;
}

void TTxThread::Terminate() {
	Lock();
	FTerminated = true;
	Unlock();
}

bool TTxThread::IsTerminated() {
	bool Result;
	Lock();
	Result = FTerminated;
	Unlock();

	return Result;
}


void TTxThread::Execute() {

	int Status = TxInitSync();
	if (Status != 0) {
		emit TxStart(Status);
		return;
	};
	Status = bladerf_enable_module(FDevice, BLADERF_MODULE_TX, true);
	if (Status != 0) {
		emit TxStart(Status);
		return;
	};
	sym_out = 0;

	//FTxSamplesLen = 20*1000; // May be any (reasonable) size
	FTxBufferSize = TxSamplesLen*sizeof(int16_t)*2; // May be any (reasonable) size

	FTxBuffer = (TWCplx *)malloc(FTxBufferSize);
	FTxBufferRe = (double *)malloc(TxSamplesLen*sizeof(double));
	FTxBufferIm = (double *)malloc(TxSamplesLen*sizeof(double));

	FTxActive = true;
	emit TxStart(0);

	while (!IsTerminated()) {
		if (Process() != 0)
			break;
	};
	bladerf_enable_module(FDevice, BLADERF_MODULE_TX, false);
	free(FTxBuffer);
	emit TxStop();
}

void TTxThread::Finished() {
	emit TxFinished();
}

