#include "keyboardsettings.h"

#include <QMap>
#include <pcbsd-utils.h>

using namespace keyboard;

typedef struct _SKeyboardLayoutDescr
{
    QString description;
    QMap<QString,QString> variants;
}SKeyboardLayoutDescr;

typedef struct _SOptionGroupDescr
{
    QString description;
    QMap<QString,QString> options;
}SOptionGroupDescr;

static QMap<QString,QString> keyboardModels;
static QMap<QString, SKeyboardLayoutDescr> keyboardLayouts;
static QMap<QString, SOptionGroupDescr> keyboardOptions;

static QString currentKbModel;
static LayoutsVector currentKbLayouts;
static OptionsVector currentKbOptions;

const char* const CURRENT_XKB_SETTINGS_COMMAND = "setxkbmap -query";
const char* const LAYOUTS_LIST_FILE = "/usr/local/share/X11/xkb/rules/base.lst";

///////////////////////////////////////////////////////////////////////////////
static void parseKbModel(QString line)
{
    // Example:
    //    pc101           Generic 101-key PC

    keyboardModels.clear();

    QString key = line.split(" ")[0].trimmed();
    keyboardModels[key] = line.right(line.length() - line.indexOf(" ")).trimmed();
}

///////////////////////////////////////////////////////////////////////////////
static void parseKbLayout(QString line)
{
   // Example:
   //     us              English (US)

   SKeyboardLayoutDescr entry;
   entry.description = line.right(line.length() - line.indexOf(" ")).trimmed();
   QString key =line.split(" ")[0].trimmed();
   keyboardLayouts[key] = entry;
}

///////////////////////////////////////////////////////////////////////////////
static void parseKbVariant(QString line)
{
    // Example:
    //      winkeys         ua: Ukrainian (WinKeys)
    //                      ^^ <- layout name

    QString key = line.split(" ")[0];
    QString layout_key = line.split(' ', QString::SkipEmptyParts)[1].replace(":","");
    QString description = line.right(line.length() - line.indexOf(':')-1).trimmed();

    if (keyboardLayouts.contains(key))
    {
        keyboardLayouts[layout_key].variants[key] = description;
    }
}

///////////////////////////////////////////////////////////////////////////////
static void parseKbOption(QString line)
{
    // Example:
    //        grp                  Switching to another layout
    //  --- OR ---
    //        grp:alt_shift_toggle Alt+Shift
    //        ^^^ <-- group name
    QString key = line.split(" ")[0].trimmed();
    if (key.contains(":"))
    {
        QString name = key.split(":")[1];
        key = key.split(":")[0];
        QString description = line.right(line.length() - line.indexOf(" ")).trimmed();
        if (keyboardOptions.contains(key))
        {
            keyboardOptions[key].options[name] = description;
        }
    }//group option
    else
    {
        // new group
        SOptionGroupDescr new_entry;
        new_entry.description = line.right(line.length() - line.indexOf(" ")).trimmed();
        keyboardOptions[key] = new_entry;
    }//new group
}

///////////////////////////////////////////////////////////////////////////////
static void loadAllLayouts()
{
    keyboardModels.clear();

    // Read base.lst file
    QStringList lines;
    QFile file(LAYOUTS_LIST_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    while (!file.atEnd())
    {
        QString line = file.readLine();
        line = line.trimmed();
        if (!line.length())
            continue;
        lines.append(line);
    }

    QStringList::iterator it = lines.begin();
    QString line;

    typedef enum{
        eKbType,
        eKbLayout,
        eKbVariant,
        eKbOption
    }ECurrSection;

    ECurrSection currSection = eKbType;

    while (it != lines.end())
    {
        line = *it++;
        if (line == QString("! model"))
        {
            currSection = eKbType;
            continue;
        }
        if (line == QString("! layout"))
        {
            currSection = eKbLayout;
            continue;
        }
        if (line == QString("! variant"))
        {
            currSection = eKbLayout;
            continue;
        }
        if (line == QString("! option"))
        {
            currSection = eKbOption;
            continue;
        }

        switch(currSection)
        {
            case eKbType:
                parseKbModel(line); break;
            case eKbLayout:
                parseKbLayout(line); break;
            case eKbVariant:
                parseKbVariant(line); break;
            case eKbOption:
                parseKbOption(line); break;

        }
    }// for all lines
}

///////////////////////////////////////////////////////////////////////////////
static void getCurrentSettings()
{
    QStringList out = pcbsd::Utils::runShellCommand(CURRENT_XKB_SETTINGS_COMMAND);

    /* Example out:
     * rules:      base
     * model:      pc101
     * layout:     us,ua,ru
     * variant:    ,winkeys,
     * options:    terminate:ctrl_alt_bksp,grp:alt_shift_toggle
     */


    QStringList layouts,variants,options;
    for (int i=0; i<out.size();i++)
    {
        QStringList spline = out[i].split(":");
        QString name = spline[0].trimmed();
        QString val = spline[1].trimmed();

        if (name == "model")
        {
            currentKbModel = val;
        }
        if (name == "layout")
        {
            layouts = val.split(",");
        }
        if (name == "variant")
        {
            variants = val.split(",");
        }
        if (name == "options")
        {
            options = val.split(",");
            continue;
        }
    }

    currentKbLayouts.clear();
    for (int i=0; i<layouts.size(); i++)
    {
        Layout entry;
        entry.layout_id = layouts[i];
        if (variants.size()>i)
            entry.variant_id = variants[i];
        currentKbLayouts.push_back(entry);
    }

    currentKbOptions.clear();
    for (int i=0; i<layouts.size(); i++)
    {
        Option entry;
        QString item = options[i];
        entry.group_name = item.split(":")[0];
        entry.option = item.split(":")[1];
    }
}

///////////////////////////////////////////////////////////////////////////////
QString keyboard::model()
{
    if (!currentKbModel.length())
    {
        getCurrentSettings();
    }
    return currentKbModel;
}

///////////////////////////////////////////////////////////////////////////////
QStringList possibleLayouts()
{
    if (!keyboardLayouts.size())
    {
        loadAllLayouts();
    }

    QStringList out;

    QMap<QString, SKeyboardLayoutDescr>::const_iterator it = keyboardLayouts.constBegin();
    while (it != keyboardLayouts.constEnd())
    {
        out<<it.key();
        ++it;
    }

    return out;
}

///////////////////////////////////////////////////////////////////////////////
QStringList possibleVariants(QString layout_id)
{
    if (!keyboardLayouts.size())
    {
        loadAllLayouts();
    }

    QStringList out;

    if (keyboardLayouts.contains(layout_id))
    {
        SKeyboardLayoutDescr layout = keyboardLayouts[layout_id];
        QMap<QString, QString>::const_iterator it = layout.variants.constBegin();
        while ( it != layout.variants.constEnd())
        {
            out<<it.key();
            ++it;
        }//for all variants
    }// if layout found

    return out;
}

///////////////////////////////////////////////////////////////////////////////
QStringList possibleModels()
{
    if (!keyboardLayouts.size())
    {
        loadAllLayouts();
    }

    QStringList out;

    QMap<QString, QString>::const_iterator it = keyboardModels.constBegin();
    while (it != keyboardModels.constEnd())
    {
        out<<it.key();
        ++it;
    }
    return out;
}

///////////////////////////////////////////////////////////////////////////////
QString modelDescription(QString model)
{
    if (!keyboardLayouts.size())
    {
        loadAllLayouts();
    }

    QString out;
    if (keyboardModels.contains(model))
    {
        out = keyboardModels[model];
    }
    return out;
}

///////////////////////////////////////////////////////////////////////////////
QString layoutDescription(QString id)
{
    if (!keyboardLayouts.size())
    {
        loadAllLayouts();
    }

    QString out;
    if (keyboardLayouts.contains(id))
    {
        out = keyboardLayouts[id].description;
    }
    return out;
}

///////////////////////////////////////////////////////////////////////////////
QString variantDescription(QString layout_id, QString variant_id)
{
    if (!keyboardLayouts.size())
    {
        loadAllLayouts();
    }

    QString out;
    if (keyboardLayouts.contains(layout_id))
    {
        SKeyboardLayoutDescr layout = keyboardLayouts[layout_id];
        if (layout.variants.contains(variant_id))
        {
            out = layout.variants[variant_id];
        }
    }//if layout_id exist
    return out;
}

///////////////////////////////////////////////////////////////////////////////
QStringList possibleOptionGroups()
{
    if (!keyboardLayouts.size())
    {
        loadAllLayouts();
    }

    QStringList out;

    QMap<QString, SOptionGroupDescr>::const_iterator it = keyboardOptions.constBegin();
    while (it != keyboardOptions.constEnd())
    {
        out<<it.key();
        ++it;
    }

    return out;
}

///////////////////////////////////////////////////////////////////////////////
QStringList possibleOptions(QString group_id)
{
    if (!keyboardLayouts.size())
    {
        loadAllLayouts();
    }

    QStringList out;

    if (keyboardOptions.contains(group_id))
    {
        SOptionGroupDescr group = keyboardOptions[group_id];
        QMap<QString, QString>::const_iterator it = group.options.constBegin();
        while ( it != group.options.constEnd())
        {
            out<<it.key();
            ++it;
        }//for all options
    }// if group found

    return out;
}

///////////////////////////////////////////////////////////////////////////////
QString optionGroupDescription(QString grp_id)
{
    if (!keyboardOptions.size())
    {
        loadAllLayouts();
    }

    QString out;
    if (keyboardOptions.contains(grp_id))
    {
        out = keyboardOptions[grp_id].description;
    }
    return out;
}

///////////////////////////////////////////////////////////////////////////////
QString optionDescription(QString grp_id, QString option_id)
{
    if (!keyboardOptions.size())
    {
        loadAllLayouts();
    }

    QString out;
    if (keyboardOptions.contains(grp_id))
    {
        SOptionGroupDescr grp = keyboardOptions[grp_id];
        if (grp.options.contains(grp_id))
        {
            out = grp.options[option_id];
        }//if option exists
    }//if group exist
    return out;
}

///////////////////////////////////////////////////////////////////////////////
OptionsVector currentOptions()
{

    return currentKbOptions;
}

///////////////////////////////////////////////////////////////////////////////
LayoutsVector currentLeayouts()
{
    return currentKbLayouts;
}
