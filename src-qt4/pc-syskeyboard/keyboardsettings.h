#ifndef KEYBOARDSETTINGS_H
#define KEYBOARDSETTINGS_H

#include <QString>
#include <QVector>
#include <QStringList>

namespace pcbsd{
namespace keyboard
{
    typedef struct SLayout
    {
        QString layout_id;
        QString variant_id;

        SLayout(){;}
        SLayout(QString layout, QString variant):layout_id(layout),variant_id(variant){;}

        bool operator==(const SLayout& l)
        {
            return (layout_id == l.layout_id) && (variant_id == l.variant_id);
        }
        QString fullName()
        {
            return (variant_id.length())?
                        layout_id+"("+variant_id+")"
                      : layout_id;
        }

        bool isEmpty()
        {
            return (!layout_id.length() && variant_id.length());
        }
    }Layout;

    typedef QVector<Layout> LayoutsVector;

    typedef struct SOption
    {
        QString group_name;
        QString option;        


        SOption(){;}
        SOption(QString grp, QString opt):group_name(grp),option(opt){;}
        SOption(QString fullName)
        {
            group_name = fullName.split(":")[0];
            option = fullName.split(":")[1];
        }
        bool operator==(const SOption& opt)
        {
            return ((group_name == opt.group_name)&&(option == opt.option));
        }

        bool isEmpty()
        {
            return !(group_name.length() & option.length());
        }

        QString fullName()
        {
            return group_name + ":" + option;
        }

    }Option;

    typedef QVector<Option> OptionsVector;

    class KeyboardSettings
    {
      public:
        QString keyboardModel();
        void setKeyboardModel(QString kb_model);

        int layoutsCount();
        Layout layout(int idx);
        Layout layout(QString layout_id);
        bool hasLayout(Layout l);
        bool hasLayout(QString layout_id);
        void clearLayouts();
        void addLayout(Layout l);
        void removeLayout(QString layout_id);
        void removeLayout(Layout l);
        void setLayouts(LayoutsVector lv);

        int optionsCount();
        Option option(int idx);
        bool hasOption(Option opt);
        void clearOptions();
        void addOption(Option opt);
        void removeOption(Option opt);
        void setOptions(OptionsVector ov);

        QString layoutsAsString();
        QString optionsAsString();
        QString xkbString();

      protected:
        LayoutsVector mLayouts;
        OptionsVector mOptions;
        QString       mKbmodel;
    };

    const Option OPTION_SWITCH_ALT_SHIFT ("grp", "alt_shift_toggle");
    const Option OPTION_SWITCH_CTRL_SHIFT ("grp", "ctrl_shift_toggle");

    KeyboardSettings currentSettings();

    void applySettings(KeyboardSettings cs);

    QStringList possibleLayouts();
    QStringList possibleVariants(QString layout_id);
    QStringList possibleModels();
    QStringList possibleOptionGroups();
    QStringList possibleOptions(QString group_id);

    QString modelDescription(QString model);
    QString layoutDescription(QString id);
    QString variantDescription(QString layout_id, QString variant_id);
    QString optionGroupDescription(QString grp_id);
    QString optionDescription(QString grp_id, QString option_id);    

}}

#endif // KEYBOARDSETTINGS_H
