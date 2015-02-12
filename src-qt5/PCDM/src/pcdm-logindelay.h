#ifndef _PCDM_LOGIN_DELAY_H
#define _PCDM_LOGIN_DELAY_H

#include <QDialog>
#include <QTimer>
#include <QString>
#include <QList>

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
	QList<QWidget*> screens;

	void fillScreens();

private slots:
	void updateTimer();
	void cancelLogin();
	void startLogin();
};

#endif
