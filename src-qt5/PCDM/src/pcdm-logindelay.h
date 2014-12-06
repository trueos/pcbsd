#ifndef _PCDM_LOGIN_DELAY_H
#define _PCDM_LOGIN_DELAY_H

#include <QDialog>
#include <QTimer>
#include <QString>


namespace Ui{
	class loginDelay;
};

class loginDelay : public QDialog{
	Q_OBJECT
public:
	loginDelay(int seconds, QString username);
	~loginDelay();

	void start();

	bool continueLogin;

private:
	Ui::loginDelay *ui; //designer file
	QTimer *delay;

private slots:
	void updateTimer();
	void cancelLogin();
	void startLogin();
};

#endif
