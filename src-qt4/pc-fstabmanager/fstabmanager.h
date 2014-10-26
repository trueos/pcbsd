#ifndef FSTABMANAGER_H
#define FSTABMANAGER_H

#include <QMainWindow>

namespace Ui {
class FstabManager;
}

class FstabManager : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit FstabManager(QWidget *parent = 0);
    ~FstabManager();
    
private slots:
    void on_pushButton_3_clicked();

private:
    Ui::FstabManager *ui;
};

#endif // FSTABMANAGER_H
