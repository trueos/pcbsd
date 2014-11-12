#include "keyboardsettings.h"

#include <QMap>
#include <QDebug>
#include <pcbsd-utils.h>

using namespace pcbsd::keyboard;

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

const char* const CURRENT_XKB_SETTINGS_COMMAND = "setxkbmap -query";
const char* const LAYOUTS_LIST_FILE = "/usr/local/share/X11/xkb/rules/base.lst";

///////////////////////////////////////////////////////////////////////////////
static void parseKbModel(QString line)
{
    // Example:
    //    pc101           Generic 101-key PC    

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

    QString layout_key = line.split(" ")[0];
    QString key = line.split(' ', QString::SkipEmptyParts)[1].replace(":","");
    QString description = line.right(line.length() - line.indexOf(':')-1).trimmed();

    if (keyboardLayouts.contains(key))
    {
        keyboardLayouts[key].variants[layout_key] = description;
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

    keyboardLayouts.clear();
    keyboardModels.clear();
    keyboardOptions.clear();

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
            currSection = eKbVariant;
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
QStringList pcbsd::keyboard::possibleLayouts()
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
QStringList pcbsd::keyboard::possibleVariants(QString layout_id)
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
QStringList pcbsd::keyboard::possibleModels()
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
QString pcbsd::keyboard::modelDescription(QString model)
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
QString pcbsd::keyboard::layoutDescription(QString id)
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
QString pcbsd::keyboard::variantDescription(QString layout_id, QString variant_id)
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
QStringList pcbsd::keyboard::possibleOptionGroups()
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
QStringList pcbsd::keyboard::possibleOptions(QString group_id)
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
QString pcbsd::keyboard::optionGroupDescription(QString grp_id)
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
QString pcbsd::keyboard::optionDescription(QString grp_id, QString option_id)
{
    if (!keyboardOptions.size())
    {
        loadAllLayouts();
    }

    QString out;
    if (keyboardOptions.contains(grp_id))
    {
        SOptionGroupDescr grp = keyboardOptions[grp_id];
        if (grp.options.contains(option_id))
        {
            out = grp.options[option_id];
        }//if option exists
    }//if group exist
    return out;
}

///////////////////////////////////////////////////////////////////////////////
KeyboardSettings pcbsd::keyboard::currentSettings()
{
    KeyboardSettings ret;

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
        QString val = out[i].right(out[i].length() - out[i].indexOf(" ")).trimmed();

        if (name == "model")
        {
            ret.setKeyboardModel(val);
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

    for (int i=0; i<layouts.size(); i++)
    {
        Layout entry;
        entry.layout_id = layouts[i];
        if (variants.size()>i)
            entry.variant_id = variants[i];
        ret.addLayout(entry);
    }

    for (int i=0; i<options.size(); i++)
    {
        Option entry;
        QString item = options[i];
        entry.group_name = item.split(":")[0];
        entry.option = item.split(":")[1];
        ret.addOption(entry);
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
void pcbsd::keyboard::applySettings(KeyboardSettings cs)
{
    QProcess::startDetached(QString("setxkbmap ") + cs.xkbString());
}

///////////////////////////////////////////////////////////////////////////////
// --------- KeyboardSettings class -------
//////////////////////////////////////////////////////////////////////////////////
QString KeyboardSettings::keyboardModel()
{
    return mKbmodel;
}

///////////////////////////////////////////////////////////////////////////////
void KeyboardSettings::setKeyboardModel(QString kb_model)
{
    mKbmodel = kb_model;
}

///////////////////////////////////////////////////////////////////////////////
int KeyboardSettings::layoutsCount()
{
    return mLayouts.size();
}

///////////////////////////////////////////////////////////////////////////////
Layout KeyboardSettings::layout(int idx)
{
    Layout out;
    if (idx<mLayouts.size())
        out = mLayouts[idx];
    return out;
}

///////////////////////////////////////////////////////////////////////////////
Layout KeyboardSettings::layout(QString layout_id)
{
    Layout out;
    for (int i=0; i<mLayouts.size(); i++)
    {
        if (mLayouts[i].layout_id == layout_id)
        {
            out = mLayouts[i];
        }
    }
    return out;
}

///////////////////////////////////////////////////////////////////////////////
bool KeyboardSettings::hasLayout(Layout l)
{
    return mLayouts.contains(l);
}

///////////////////////////////////////////////////////////////////////////////
bool KeyboardSettings::hasLayout(QString layout_id)
{
    for (int i=0; i<mLayouts.size(); i++)
    {
        if (mLayouts[i].layout_id == layout_id)
        {
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
void KeyboardSettings::clearLayouts()
{
    mLayouts.clear();
}

///////////////////////////////////////////////////////////////////////////////
void KeyboardSettings::addLayout(Layout l)
{
    mLayouts.push_back(l);
}

///////////////////////////////////////////////////////////////////////////////
void KeyboardSettings::removeLayout(QString layout_id)
{
    LayoutsVector::iterator it = mLayouts.begin();
    while ( it!= mLayouts.end())
    {
        if ((*it).layout_id == layout_id)
            it = mLayouts.erase(it);
        else
            ++it;
    }
}

///////////////////////////////////////////////////////////////////////////////
void KeyboardSettings::removeLayout(Layout l)
{
    int idx = mLayouts.indexOf(l);
    if (idx>0)
        mLayouts.remove(idx);
}

///////////////////////////////////////////////////////////////////////////////
void KeyboardSettings::setLayouts(LayoutsVector lv)
{
    mLayouts = lv;
}

///////////////////////////////////////////////////////////////////////////////
int KeyboardSettings::optionsCount()
{
    return mOptions.size();
}

///////////////////////////////////////////////////////////////////////////////
Option KeyboardSettings::option(int idx)
{
    Option out;
    if (idx<mOptions.size())
    {
        out = mOptions[idx];
    }
    return out;
}

///////////////////////////////////////////////////////////////////////////////
bool KeyboardSettings::hasOption(Option opt)
{
    return mOptions.contains(opt);
}

///////////////////////////////////////////////////////////////////////////////
void KeyboardSettings::clearOptions()
{
    mOptions.clear();
}

///////////////////////////////////////////////////////////////////////////////
void KeyboardSettings::addOption(Option opt)
{
    if (!mOptions.contains(opt))
        mOptions.push_back(opt);
}

///////////////////////////////////////////////////////////////////////////////
void KeyboardSettings::removeOption(Option opt)
{
    int idx = mOptions.indexOf(opt);
    if (idx>0)
        mOptions.remove(idx);
}

///////////////////////////////////////////////////////////////////////////////
QString KeyboardSettings::layoutsAsString()
{
    QString ret;
    for(int i=0;i<mLayouts.size();i++)
    {
        if (i) ret+=',';
        ret+=mLayouts[i].layout_id;
    }
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
QString KeyboardSettings::variantsAsString()
{
    bool variantFound = false;
    QString ret;
    for (int i=0;i<mLayouts.size();i++)
    {
        if (i)
            ret+=",";
        if (mLayouts[i].variant_id.length())
        {
            ret+=mLayouts[i].variant_id;
            variantFound = true;
        }
    }
    if (!variantFound)
        ret.clear();
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
QString KeyboardSettings::optionsAsString()
{
    QString ret;
    for(int i=0;i<mOptions.size();i++)
    {
        if (i) ret+=',';
        ret+=mOptions[i].fullName();
    }
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
QString KeyboardSettings::xkbString()
{
    QString ret;
    if (mKbmodel.length())
        ret+=QString("-model ")+mKbmodel;
    if (mLayouts.size())
    {
        ret+=QString(" -layout \"")+layoutsAsString()+"\"";
        if (variantsAsString().length())
            ret+=QString(" -variant \"")+variantsAsString()+"\"";
    }

    if (mOptions.size())
        ret+=QString(" -option \"")+optionsAsString()+"\"";
    return ret;
}


