#ifndef KEYBOARDSETTINGS_H
#define KEYBOARDSETTINGS_H

#include <QString>
#include <QVector>
#include <QStringList>

namespace keyboard
{
    typedef struct SLayout
    {
        QString layout_id;
        QString variant_id;
    }Layout;

    typedef QVector<Layout> LayoutsVector;

    typedef struct SOption
    {
        QString group_name;
        QString option;
    }Option;

    typedef QVector<Option> OptionsVector;

    const char* const LAYOUT_SWITCH_ALT_SHIFT = "alt_shift_toggle";
    const char* const LAYOUT_SWITCH_CTRL_SHIFT = "ctrl_shift_toggle";

    QString model();
    bool setModel();

    QString modelDescription(QString model);
    QString layoutDescription(QString id);
    QString variantDescription(QString layout_id, QString variant_id);
    QString optionGroupDescription(QString grp_id);
    QString optionDescription(QString grp_id, QString option_id);

    LayoutsVector layouts();
    bool setLayouts(LayoutsVector layout);

    LayoutsVector currentLeayouts();

    OptionsVector currentOptions();

    QStringList possibleLayouts();
    QStringList possibleVariants(QString layout_id);
    QStringList possibleModels();
    QStringList possibleOptionGroups();
    QStringList possibleOptions(QString group_id);

    bool setLayoutSwithKeys(QString value);
    QString layoutSwithKeys();
    bool setAllowTerminateX(bool isTerminate);
    bool allowTerminateX();

    bool apply();
    bool save(QString root=QString());
}

#endif // KEYBOARDSETTINGS_H
