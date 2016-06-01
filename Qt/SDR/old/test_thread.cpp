
#include "test_thread.h"

TTestThread::TTestThread(QObject *parent) : QThread(parent) {

}

TTestThread::~TTestThread() {

}


void TTestThread::run() {
	while (1) {
		msleep(100);
	};
}
