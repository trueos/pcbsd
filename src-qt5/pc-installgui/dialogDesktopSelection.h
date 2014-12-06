
#ifndef DIALOGDESK_H
#define DIALOGDESK_H

#include "ui_dialogDesktopSelection.h"
#include "dialogInfoBox.h"
#include <QMenu>
#include <QString>


class desktopSelection : public QWidget, private Ui::desktopSelection
{
	Q_OBJECT

public:
        desktopSelection() : QWidget()
        {
          setupUi(this);
        }

    void programInit(QList<QStringList>, QStringList);


public slots:

protected:

private slots:
    void slotClose();
    void slotSave();
    void slotShowLabelDesc(QTreeWidgetItem *aItem, int aCol);
    void slotDeskPkgsChanged(QTreeWidgetItem *aItem, int aCol);
    void slotMetaRightClick();
    void slotMetaViewPkgs();

private:
    // Functions
    void initDesktopPkgs();
    void addTreeItems(QString parent);
    bool haveMetaDesktopChecked();
    bool allChildrenPkgsChecked(QString parent);
    bool allChildrenPkgsUnchecked(QString parent);
    void checkAllChildrenPkgs(QString parent);
    void uncheckAllChildrenPkgs(QString parent);
    QStringList getDeskPkgList();


    QList<QStringList> listDeskPkgs;
    QStringList selectedPkgs;
    // Infobox
    dialogInfoBox *dIB;
    QMenu *popup;

signals:
    void saved(QStringList);

} ;
#endif // DIALOGINFO_H
