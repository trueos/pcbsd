#include "keyboardlayout.h"
#include "ui_keyboardlayout.h"

#include <QFile>
#include <QStringList>

const char* const LAYOUTS_LIST_FILE = "/usr/local/share/X11/xkb/rules/base.lst";

KeyboardLayout::KeyboardLayout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyboardLayout)
{
    ui->setupUi(this);
    loadLayouts();
}

KeyboardLayout::~KeyboardLayout()
{
    delete ui;
}

void KeyboardLayout::loadLayouts()
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

    //Parse keyboard models
    while (it != lines.end())
    {
        line = *it++;
        if (line == QString("! model"))
        {
            continue;
        }
        if (line == "! layout")
        {
            break;
        }

        SName entry;
        entry.id = line.split(" ")[0];
        entry.description = line.right(line.length() - line.indexOf(" ")).trimmed();
        keyboardModels.push_back(entry);
    }

    //Parse keyboard layouts
    while (it != lines.end())
    {
        line = *it++;
        if (line == "! variant")
        {
            break;
        }

        SKeyboardLayout entry;
        entry.description = line.right(line.length() - line.indexOf(" ")).trimmed();

        keyboardLayouts[ line.split(" ")[0] ] = entry;
    }

    //Parse variants
    while (it != lines.end())
    {
        line = *it++;
        QString name = line.split(" ")[0];
        QString key = line.split(' ', QString::SkipEmptyParts)[1].replace(":","");
        QString description = line.right(line.length() - line.indexOf(':')-1).trimmed();
        SName entry;
        entry.id = name;
        entry.description = description;
        if (keyboardLayouts.contains(key))
        {
            keyboardLayouts[key].variants.push_back(entry);
        }
    }

}
