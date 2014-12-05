/**************************************************************************
*   Copyright (C) 2014 by Yuri Momotyuk                                   *
*   yurkis@pcbsd.org                                                      *
*                                                                         *
*   Permission is hereby granted, free of charge, to any person obtaining *
*   a copy of this software and associated documentation files (the       *
*   "Software"), to deal in the Software without restriction, including   *
*   without limitation the rights to use, copy, modify, merge, publish,   *
*   distribute, sublicense, and/or sell copies of the Software, and to    *
*   permit persons to whom the Software is furnished to do so, subject to *
*   the following conditions:                                             *
*                                                                         *
*   The above copyright notice and this permission notice shall be        *
*   included in all copies or substantial portions of the Software.       *
*                                                                         *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
*   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
*   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
*   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
*   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
*   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
*   OTHER DEALINGS IN THE SOFTWARE.                                       *
***************************************************************************/

#ifndef KEYBOARDSETTINGS_H
#define KEYBOARDSETTINGS_H

#include <QString>
#include <QVector>
#include <QStringList>

namespace pcbsd{
namespace keyboard
{
    //! Keyboard layout definition
    typedef struct SLayout
    {
        QString layout_id; ///< Layout identificator
        QString variant_id; ///< Variant identificator

        SLayout(){;}
        SLayout(QString layout, QString variant):layout_id(layout),variant_id(variant){;}

        bool operator==(const SLayout& l)
        {
            return (layout_id == l.layout_id) && (variant_id == l.variant_id);
        }

        //! Get full layout name in format 'layout(variant)'
        QString fullName()
        {
            return (variant_id.length())?
                        layout_id+"("+variant_id+")"
                      : layout_id;
        }

        //! True if not initialized
        bool isEmpty()
        {
            return (!layout_id.length() && variant_id.length());
        }
    }Layout;

    //! Vector of layouts type
    typedef QVector<Layout> LayoutsVector;

    //! Keyboard option definition
    typedef struct SOption
    {
        QString group_name; ///< Options group name
        QString option;     ///< Option name


        SOption(){;}
        SOption(QString grp, QString opt):group_name(grp),option(opt){;}

        //! Initializer. You may use format 'group:option' string
        SOption(QString fullName)
        {
            group_name = fullName.split(":")[0];
            option = fullName.split(":")[1];
        }
        bool operator==(const SOption& opt)
        {
            return ((group_name == opt.group_name)&&(option == opt.option));
        }

        //! True if not initialized
        bool isEmpty()
        {
            return !(group_name.length() & option.length());
        }

        //! Get full option name in format 'group:option'
        QString fullName()
        {
            return group_name + ":" + option;
        }

    }Option;

    //! Vector of keyboard option type
    typedef QVector<Option> OptionsVector;

    //! Class that holds complete keyboard settings
    class KeyboardSettings
    {
      public:
        //! Get keyboard model name
        QString keyboardModel();
        //! Set keyboard model name
        void setKeyboardModel(QString kb_model);

        //! Get cont of layouts
        int layoutsCount();
        //! Get layout by index
        Layout layout(int idx);
        //! Get layout by string ID
        Layout layout(QString layout_id);
        //! True if settings has some layout
        bool hasLayout(Layout l);
        //! True if settings has some string layout ID
        bool hasLayout(QString layout_id);
        //! Remove all layouts
        void clearLayouts();
        //! Add layout
        void addLayout(Layout l);
        //! Remove layout by string layout ID
        void removeLayout(QString layout_id);
        //! Remove layout
        void removeLayout(Layout l);
        //! Set layouts directly
        void setLayouts(LayoutsVector lv);

        //! Get cont of options in this settings
        int optionsCount();
        //! Get option by index
        Option option(int idx);
        //! True if settings has option
        bool hasOption(Option opt);
        //! Remove all options
        void clearOptions();
        //! Add new option to the settings
        void addOption(Option opt);
        //! Remove option from settings
        void removeOption(Option opt);
        //! Set options directly
        void setOptions(OptionsVector ov);

        //! Get string for setxkbmap -layout flag
        QString layoutsAsString();
        //! Get string fro setxkbmap -variant flag
        QString variantsAsString();
        //! Get string fro setxkbmap -option flag
        QString optionsAsString();
        //! Get complete options string for setxkbmap calling
        QString xkbString();

      protected:
        LayoutsVector mLayouts;
        OptionsVector mOptions;
        QString       mKbmodel;
    };

    //! 'Changing layout by Alt + Shift' option
    const Option OPTION_SWITCH_ALT_SHIFT ("grp", "alt_shift_toggle");
    //! 'Changing layout by Ctrl + Shift' option
    const Option OPTION_SWITCH_CTRL_SHIFT ("grp", "ctrl_shift_toggle");
    //! 'Changing layout by Alt + Space' option
    const Option OPTION_SWITCH_ALT_SPACE ("grp", "alt_space_toggle");
    //! 'Allow terminate X by pressing Ctrl+Alt+Backspace' option
    const Option OPTION_ALLOW_TERMINATE_X ("terminate", "ctrl_alt_bksp");

    //! Get current keyboard settings
    KeyboardSettings currentSettings();

    //! Apply (but not save) keyboard settings
    void applySettings(KeyboardSettings cs);

    //! Get all of possible keyboard layout id
    QStringList possibleLayouts();
    //! Get all of possible variant id for some layout
    QStringList possibleVariants(QString layout_id);
    //! Get all of possible keyboard model id
    QStringList possibleModels();
    //! Get all of possible option group id
    QStringList possibleOptionGroups();
    //! Get all of possible option id for some group
    QStringList possibleOptions(QString group_id);

    //! Get keyboard model description by keyboard model id (\see possibleModels() \see KeyboardSettings::keyboardModel())
    QString modelDescription(QString model);
    //! Get layout description by layout id
    QString layoutDescription(QString id);
    //! Get variant description by layout id and variant id
    QString variantDescription(QString layout_id, QString variant_id);
    //! Get options group description by options group id
    QString optionGroupDescription(QString grp_id);
    //! Get option description by options group id and option id
    QString optionDescription(QString grp_id, QString option_id);

}}

#endif // KEYBOARDSETTINGS_H
