#ifndef DIALOGFSCREATE_H
#define DIALOGFSCREATE_H

#include <QDialog>

namespace Ui {
class DialogfsCreate;
}

class DialogfsCreate : public QDialog
{
    Q_OBJECT
    
public:
    bool isValid;
    QString fsRoot;
    void setRootPath(QString path, QString text);
    void changeTitle(QString title);
    void setPropsVisible(bool p);
    void checkValidPath();
    void nameOnlyMode();
    bool hasValidPath();
    QString getPath();
    QStringList getOptions();
    void done(int result);


    explicit DialogfsCreate(QWidget *parent = 0);
    ~DialogfsCreate();
    
private slots:
    void on_fsName_textEdited(const QString &arg1);

    void on_checkMountpt_stateChanged(int arg1);

    void on_comboMountpt_currentIndexChanged(int index);

    void on_checkNormalization_stateChanged(int arg1);

    void on_checkCopies_stateChanged(int arg1);

    void on_checkCompression_stateChanged(int arg1);

private:
    Ui::DialogfsCreate *ui;
};

#endif // DIALOGFSCREATE_H
