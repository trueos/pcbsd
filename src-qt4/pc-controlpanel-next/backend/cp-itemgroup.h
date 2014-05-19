#ifndef CPITEMGROUP_H
#define CPITEMGROUP_H

#include "cp-item.h"
#include <QVector>

class CItemGroup: public QObject
{
    Q_OBJECT
public:
    CItemGroup(QString path, QString name);

    bool read();
    void readAssync();

    QVector<CControlPanelItem> items(QStringList enabled_de, QString filter=QString());
    int itemsCount(QStringList enabled_de, QString filter=QString());

    QString path();
    QString name();

signals:
    void itemsReady();

private:
    QVector<CControlPanelItem> mItems;
    QString mName;
    QString mPath;
};

#endif // CPITEMGROUP_H
