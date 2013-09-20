#ifndef _LP_GUTILS_H
#define _LP_GUTILS_H

#include <QMessageBox>
#include <QDialog>
#include <QStringList>
#include <QString>

#include "LPBackend.h"
#include "LPContainers.h"

class LPGUtils{
public:
	static LPDataset loadPoolData(QString zpool); //Load backend data into container

};

#endif