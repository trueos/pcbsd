#ifndef MAILLIST_H
#define MAILLIST_H

#include <QWidget>

namespace Ui {
    class maillist;
}

class maillist : public QWidget
{
    Q_OBJECT

public:
    explicit maillist(QWidget *parent = 0);
    ~maillist();

	void setup(QString Name, QString URL, QString EMail, bool canLaunch=true);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::maillist *ui;

private slots:
	void on_EMailButton_clicked();
 void on_BrowserButton_clicked();
};

#endif // MAILLIST_H
