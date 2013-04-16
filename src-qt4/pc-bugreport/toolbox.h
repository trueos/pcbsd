#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QWidget>

namespace Ui {
    class Toolbox;
}

class Toolbox : public QWidget
{
    Q_OBJECT

public:
    explicit Toolbox(QWidget *parent = 0);
    ~Toolbox();

protected:
    void changeEvent(QEvent *e);

	QString mScreenshotCommand;

private:
    Ui::Toolbox *ui;

private slots:
	void on_actionUname_triggered();
 void on_actionXorg_version_triggered();
 void on_ScreenshotButton_clicked();
 void on_actionPCIConf_triggered();
	void on_actionDiagnostic_report_triggered();
	void on_actionDmesg_triggered();
	void on_actionXorg_log_file_triggered();
};

#endif // TOOLBOX_H
