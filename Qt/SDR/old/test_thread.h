#ifndef TEST_THREAD_H
#define TEST_THREAD_H

#include <QApplication>

#include <QThread>

class TTestThread : public QThread {
	Q_OBJECT
	public:
		explicit TTestThread(QObject *parent = Q_NULLPTR);
		~TTestThread();

	protected:
		virtual void run();

};


#endif // TEST_THREAD_H
