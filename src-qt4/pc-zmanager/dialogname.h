#ifndef DIALOGNAME_H
#define DIALOGNAME_H

#include <QDialog>

namespace Ui {
class DialogName;
}

class DialogName : public QDialog
{
    Q_OBJECT
    
    QStringList forbidden;
    bool isValid;

public:
    void setForbiddenList(QStringList& a);
    void setTitle(QString t);
    void setName(QString t);
    void showOptions(bool show);
    bool importReadOnly();
    bool importForce();
    bool importAutomount();
    bool importSetAltRoot();
    QString getAltRoot();

    QString getName();

    explicit DialogName(QWidget *parent = 0);
    ~DialogName();

public slots:
    virtual void done(int result);

    
private slots:
    void on_nameEdit_textChanged(const QString &arg1);

    void on_checkAltRoot_toggled(bool checked);

private:
    Ui::DialogName *ui;
};

#endif // DIALOGNAME_H
