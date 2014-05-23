#ifndef _APPCAFE_SCREENSHOT_DIALOG_H
#define _APPCAFE_SCREENSHOT_DIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QLabel>

namespace Ui{
	class SSDialog;
};

class SSDialog : public QDialog{
	Q_OBJECT
public:
	SSDialog(QWidget* parent = 0, QPixmap screen = QPixmap());
	~SSDialog();

private:
	Ui::SSDialog *ui;
	QLabel *pix; //widget for the pixmap
	
private slots:
	void on_push_close_clicked();

};

#endif
