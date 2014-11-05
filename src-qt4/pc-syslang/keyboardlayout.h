#ifndef KEYBOARDLAYOUT_H
#define KEYBOARDLAYOUT_H

#include <QWidget>
#include <QVector>
#include <QMap>

namespace Ui {
class KeyboardLayout;
}

class KeyboardLayout : public QWidget
{
    Q_OBJECT
    
public:
    explicit KeyboardLayout(QWidget *parent = 0);
    ~KeyboardLayout();

    typedef struct _SName
    {
        QString id;
        QString description;
    }SName;

    typedef struct _SKeyboardLayout
    {
        QString description;
        QVector<SName> variants;
    }SKeyboardLayout;
    
private:
    Ui::KeyboardLayout *ui;
    QVector<SName> keyboardModels;
    QMap<QString, SKeyboardLayout> keyboardLayouts;

    void loadLayouts();
};

#endif // KEYBOARDLAYOUT_H
