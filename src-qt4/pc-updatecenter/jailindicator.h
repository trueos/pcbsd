#ifndef JAILINDICATOR_H
#define JAILINDICATOR_H

#include <QWidget>

namespace Ui {
class JailIndicator;
}

class JailIndicator : public QWidget
{
    Q_OBJECT
    
public:
    explicit JailIndicator(QWidget *parent = 0);
    ~JailIndicator();

    void setJailName(QString name);
    
private:
    Ui::JailIndicator *ui;
};

#endif // JAILINDICATOR_H
