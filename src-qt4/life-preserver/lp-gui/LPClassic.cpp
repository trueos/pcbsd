#include "LPClassic.h"
#include "ui_LPClassic.h"

#include <QTreeView>

//====PUBLIC=====
LPClassic::LPClassic(QWidget *parent) : QDialog(parent), ui(new Ui::LPClassic){
  ui->setupUi(this); //load the designer file
  //initialize the file check timer
  fCheck = new QTimer(this);
    fCheck->setInterval(2000); //2 second update interval
    fCheck->setSingleShot(true);
    connect(fCheck, SIGNAL(timeout()), this, SLOT(updateFCheck()) );
  //initialize the tar process
  tarProc = new QProcess(this);
    tarProc->setProcessEnvironment( QProcessEnvironment::systemEnvironment());
    connect(tarProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotTarDone()) );
  //initialize the QFileInfo
  tarInfo = new QFileInfo();
	tarInfo->setCaching(false);
  //set the initial stopped flag
  stopped = false;
  running = false;
  ui->list_exclude->clear(); //make sure the exclude list is cleared (possibly different base-dir)
  //Make sure the initial state of the UI is correct
  on_list_exclude_itemSelectionChanged();
}

LPClassic::~LPClassic(){
	
}

void LPClassic::setHomeDir(QString home){
  //Generate the recommended filename
  tarDir = home.section("/",-1);
  tarBaseDir = home;
	tarBaseDir.chop(tarDir.length() +1);
  ui->line_filename->setText(tarDir+"-"+QDateTime::currentDateTime().toString("yyyyMMdd-hhmm"));
  //Now set the process to start in the parent directory
  tarProc->setWorkingDirectory(tarBaseDir);
  //Now make sure we start on the right page
  ui->stackedWidget->setCurrentWidget(ui->page_setup);
}

//====PRIVATE====
void LPClassic::updateFCheck(){
  //Get the current size of the file
  tarInfo->refresh();
  double size = tarInfo->size();
  QStringList labels; labels << "B" << "KB" << "MB" << "GB" << "TB" << "PB" << "EB";
  int i=0;
  while( size > 1024 && i < labels.length() ){
    size = size/1024; 
    i++;
  }
  //Round to 2 decimel places if GB or larger
  if(i>2){ size = int(size*100)/100.0; }
  else{ size = int(size); }
  //Now display that size
  ui->label_size->setText( QString::number(size)+" "+labels[i] );
  if(!stopped){ fCheck->start(); }
}

void LPClassic::slotTarDone(){
  running = false;
  fCheck->stop();
  updateFCheck(); //Final update for the file size
  if(stopped){
    qDebug() << "Home-Dir Package Cancelled";
    ui->label_status->setText(tr("Cancelled"));
    //Now try to delete the partial file
    QString file = tarBaseDir+"/"+tarFile;
    qDebug() << " - Removing partial archive file:" << file;
    QFile::remove(file);
  }else{
    qDebug() << "Home-Dir Package Finished";
    ui->label_status->setText(tr("FINISHED"));
  }
  ui->push_stop->setVisible(false);
  ui->push_finished->setVisible(true);
  //Set the progress bar as finished
  ui->progressBar->setRange(0,1);
  ui->progressBar->setValue(1);
}

//Exclude list controls
void LPClassic::on_tool_rmexclude_clicked(){
  delete ui->list_exclude->takeItem( ui->list_exclude->currentRow() );
}

void LPClassic::on_tool_addexcludefile_clicked(){
  QStringList paths = QFileDialog::getOpenFileNames(this, tr("Exclude Files"), tarBaseDir+"/"+tarDir, tr("All Files (*)"));
  for(int i=0; i<paths.length(); i++){
    if(!paths[i].startsWith(tarBaseDir+"/"+tarDir)){ continue; } //invalid file
    ui->list_exclude->addItem(paths[i]);
  }
}

void LPClassic::on_tool_addexcludedir_clicked(){
  //Do some special modifications to allow selecting multiple directories
  QFileDialog dlg(this);
  dlg.setFileMode(QFileDialog::DirectoryOnly);
  QListView *l = dlg.findChild<QListView*>("listView");
  if(l){ l->setSelectionMode(QAbstractItemView::MultiSelection); }
  QTreeView *t = dlg.findChild<QTreeView*>();
  if(t){ t->setSelectionMode(QAbstractItemView::MultiSelection); }
  dlg.setDirectory(tarBaseDir+"/"+tarDir);
  dlg.setWindowTitle( tr("Exclude Directories") );
  if(dlg.exec()){
    //Success
    QStringList paths = dlg.selectedFiles();
    for(int i=0; i<paths.length(); i++){
      //Save each path to the exclude list
      if(paths[i].isEmpty() || !paths[i].startsWith(tarBaseDir+"/"+tarDir) ){ continue; }
      if(!paths[i].endsWith("/")){ paths[i].append("/"); } //make sure there is a / on the end of directories
      ui->list_exclude->addItem(paths[i]);
    }
  }
  
  //QString path = QFileDialog::getExistingDirectory(this, , tarBaseDir+"/"+tarDir);
  /*if(path.isEmpty() || !path.startsWith(tarBaseDir+"/"+tarDir) ){ return; }
  if(!path.endsWith("/")){ path.append("/"); } //make sure there is a / on the end of directories
  ui->list_exclude->addItem(path); */
}

void LPClassic::on_list_exclude_itemSelectionChanged(){
  ui->tool_rmexclude->setEnabled( ui->list_exclude->currentItem() != 0 );
}


//Main Buttons
void LPClassic::on_push_cancel_clicked(){
  this->close(); //Just close this window
}

void LPClassic::on_push_start_clicked(){
  //Read the UI to create the exclude list
  stopped = false;
  running = true;
  tarFile = ui->line_filename->text()+".home.tar.gz";
  tarInfo->setFile(tarBaseDir+"/"+tarFile);
  //Generate the command
  QStringList args;
  args << "-czf" << tarBaseDir+"/"+tarFile << "-C" << tarBaseDir << tarDir;
  //Create the exclude list
  QStringList excludes;
    excludes << "*flashplayer*"; //Always exclude the flashplayer library
    excludes << "*/.zfs/*"; //Always exclude the hidden zfs  directoriess
  if(ui->check_pbidesktop->isChecked()){ excludes  << "*/PBI-*.desktop"; } //Don't include PBI entries
  if(ui->check_userbin->isChecked()){ excludes << "*/bin/*"; } //exclude "bin" directories
  for( int i=0; i<ui->list_exclude->count(); i++){
    QString path = ui->list_exclude->item(i)->text();
    //Remove the base homedir path
    path.remove(0,tarBaseDir.length()+tarDir.length()+1);
    if(path.endsWith("/")){ path.append("*"); } //don't put the asterisk on the end of files
    excludes << "*"+path;
  }
  //Now convert the exclude list into additional arguments
  for(int i=0; i<excludes.length(); i++){
    args.prepend(excludes[i]); args.prepend("--exclude");
  }
  //qDebug() << "Package command:" << "tar" << args;
  //Start the process
  tarProc->start("tar",args); //complex command, use args individually
  tarProc->waitForStarted(1000);
  //Start the file watcher
  fCheck->start();
  //Now show the proper page with correct elements
  ui->label_fullfilename->setText(tarBaseDir+"/"+tarFile);
  ui->push_finished->setVisible(false);
  ui->push_stop->setVisible(true);
  ui->label_status->setText(tr("Packaging Home Directory...") );
  ui->progressBar->setRange(0,0);
  ui->progressBar->setValue(-1);
  ui->stackedWidget->setCurrentWidget(ui->page_running);
}

void LPClassic::on_push_stop_clicked(){
  stopped = true;
  tarProc->kill();
  tarProc->waitForFinished(1000); //wait a second to let it die
  if(tarProc->state() != QProcess::NotRunning){ 
    tarProc->terminate(); //force kill it
    tarProc->waitForFinished(300);
  } 
}

void LPClassic::on_push_finished_clicked(){
  this->close(); //Just close this window
}

