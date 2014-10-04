#ifndef _EASYPBI_SERVICE_CONFIG_DIALOG_H
#define _EASYPBI_SERVICE_CONFIG_DIALOG_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include <QStringList>
#include <QInputDialog>
#include <QDebug>

class ServiceOption{
public:
	QString cfgfile, type, desc, longdesc, key, delim, defaultv, min, max, maxlen, quotes;
	QStringList options; // "<File Option>::::<Text Visible>"

	QStringList convertToText(int num){
	  QStringList out;
	  QString line = "$appConfig["+QString::number(num)+"][\'%1\'] = \"%2\";";
	  out << line.arg("cfgfile", cfgfile);
	  out << line.arg("type", type.toUpper());
	  out << line.arg("desc", desc);
	  out << line.arg("longdesc", longdesc.replace("\n","<br>"));
	  out << line.arg("key", key);
	  out << line.arg("delim", delim);
	  out << line.arg("default", defaultv);
	  if(!quotes.isEmpty()){ out << line.arg("quotes", quotes); }
	  //Now the type-specific values
	  if(type.toUpper()=="COMBOBOX"){
	    for(int i=0; i<options.length(); i++){
	      out << line.arg("option"+QString::number(i+1), options[i]);
	    }
	  }else if(type.toUpper()=="NUMBERBOX"){
	    out << line.arg("max", max);
	    out << line.arg("min", min);
	  }else{ //string/password box
	    out << line.arg("maxlen", maxlen);
	  }
	  return out;
	}
	
	bool isValid(){
	  return !(cfgfile.isEmpty() || type.isEmpty() || key.isEmpty());
	}
	
	void LoadFromText(QStringList fileContents, int num){
	  QStringList items = fileContents.filter("$appConfig["+QString::number(num)+"]");
	  //qDebug() << "Items:" << num << items;
	  for(int i=0; i<items.length(); i++){
	    QString var = items[i].section("\'",1,1);
	    QString val = items[i].section(" = \"",1,50);
	      val.chop( val.section("\"",-1).length()+1 ); //remove the "; from the end of the line
	    //qDebug() << "var:" << var << "val:" << val;
	    if(var=="cfgfile"){cfgfile = val;}
	    else if(var=="type"){type = val;}
	    else if(var=="desc"){desc = val;}
	    else if(var=="longdesc"){longdesc = val;}
	    else if(var=="key"){key = val;}
	    else if(var=="delim"){ delim = val; }
	    else if(var=="default"){defaultv = val;}
	    else if(var=="min"){min = val;}
	    else if(var=="max"){max = val;}
	    else if(var=="maxlen"){maxlen = val;}
	    else if(var=="quotes"){quotes = val;}
	    else if(var.startsWith("option")){options << val;}
	  }
	}
	
};

namespace Ui{
	class SConfigDialog;
};

class SConfigDialog : public QDialog{
	Q_OBJECT
public:
	SConfigDialog(QWidget *parent=0, ServiceOption *opt = 0, bool newopt = true);
	~SConfigDialog();

	bool cancelled;
	ServiceOption optOut;

private:
	Ui::SConfigDialog *ui;

	void loadOption();
	void saveOption();

private slots:
	void checkGUI();
	void generateSample(bool go = true);

	void cancelClicked();
	void applyClicked();

	void on_tool_optadd_clicked();
	void on_tool_optrem_clicked();
	void on_tool_optup_clicked();
	void on_tool_optdown_clicked();
};

#endif