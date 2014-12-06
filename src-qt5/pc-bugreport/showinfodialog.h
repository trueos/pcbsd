#ifndef SHOWINFODIALOG_H
#define SHOWINFODIALOG_H

#include <QDialog>

namespace Ui {
    class ShowInfoDialog;
}

class ShowInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowInfoDialog(QWidget *parent = 0);

	bool show(const QString &program, const QStringList &arguments);
	bool show(const QString &filename);
	bool show(const char* filename)
	{
		QString str(filename);
		return show(str);
	}

	~ShowInfoDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ShowInfoDialog *ui;

private slots:
	void on_textBrowser_copyAvailable(bool b);
	void on_ClipboardButton_clicked();
};

#endif // SHOWINFODIALOG_H
