#ifndef _LP_MESSAGES_H
#define _LP_MESSAGES_H

#include <QDialog>
#include <QStringList>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QCloseEvent>

namespace Ui{
	class LPMessages;
};

class LPMessages : public QDialog{
	Q_OBJECT
public:
	LPMessages(QWidget *parent=0);
	~LPMessages();

	void setMessages(QString msg, QString err);
	void setLogFiles(QStringList logs);

private:
	Ui::LPMessages *ui;
	bool logsAvailable;
	void displayLogFile(QString logfile);

private slots:
	void slotChangeLog();
	void closeDialog();

protected:
	void closeEvent(QCloseEvent*);

};
#endif
