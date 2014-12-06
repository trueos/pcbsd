#include "ssDialog.h"
#include "ui_ssDialog.h"

SSDialog::SSDialog(QWidget* parent, QPixmap screen) : QDialog(parent), ui(new Ui::SSDialog()){
  ui->setupUi(this);
  pix = new QLabel(this);
    pix->setPixmap(screen);
    pix->setAlignment(Qt::AlignCenter);
  ui->scrollArea->setBackgroundRole(QPalette::Dark);
  ui->scrollArea->setWidget(pix);
}

SSDialog::~SSDialog(){
	
}
		
void SSDialog::on_push_close_clicked(){
  this->close();
}