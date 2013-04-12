#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWizard>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWizard
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

	virtual int nextId() const;

	void setupButtons(bool isLast);
	void fillDEList();
    void fillPBIList();

private:
    Ui::MainWindow *ui;

private slots:
	void on_MainWindow_currentIdChanged(int id);
};

#endif // MAINWINDOW_H
