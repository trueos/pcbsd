#include "dialogConfig.h"
#include "ui_dialogConfig.h"
#include <pcbsd-utils.h>

void dialogConfig::programInit()
{

  // Load the default package set value
  if ( pcbsd::Utils::getValFromPCBSDConf("PACKAGE_SET") == "EDGE" ) {
     radioEdge->setChecked(true);
     radioProduction->setChecked(false);
  } else {
     radioProduction->setChecked(true);
     radioEdge->setChecked(false);
  }


  connect( pushOK, SIGNAL(clicked()), this, SLOT(slotOK()) );
  connect( pushCancel, SIGNAL(clicked()), this, SLOT(slotCancel()) );
}

void dialogConfig::slotCancel()
{
  close();
}

void dialogConfig::slotOK()
{
    
  // Save package set
  if ( radioProduction->isChecked() )
    pcbsd::Utils::setValPCBSDConf("PACKAGE_SET", "PRODUCTION");
  else
    pcbsd::Utils::setValPCBSDConf("PACKAGE_SET", "EDGE");
 
  // Extract the ports overlay to grab a new package set repo config
  QProcess::execute(QString("pc-extractoverlay"), QStringList() << "ports");

  emit ok();
  close();
}
