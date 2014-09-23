#ifndef _EASYPBI_SERVICE_CONFIG_DIALOG_H
#define _EASYPBI_SERVICE_CONFIG_DIALOG_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include <QStringList>
#include <QInputDialog>

class ServiceOption{
public:
	QString cfgfile, type, desc, longdesc, key, delim, defaultv, min, max, maxlen, quotes;
	QStringList options; // "<File Option>::::<Text Visible>"

	static QStringList convertToText(ServiceOption opt, int num){
	  QStringList out;
	  QString line = "$appConfig["+QString::number(num)+"][\'%1\'] = \"%2\";";
	  out << line.arg("cfgfile", opt.cfgfile);
	  out << line.arg("type", opt.type.toUpper());
	  out << line.arg("desc", opt.desc);
	  out << line.arg("longdesc", opt.longdesc.replace("\n","<br>"));
	  out << line.arg("key", opt.key);
	  out << line.arg("delim", opt.delim);
	  out << line.arg("default", opt.defaultv);
	  if(!opt.quotes.isEmpty()){ out << line.arg("quotes", opt.quotes); }
	  //Now the type-specific values
	  if(opt.type.toUpper()=="COMBOBOX"){
	    for(int i=0; i<opt.options.length(); i++){
	      out << line.arg("option"+QString::number(i+1), opt.options[i]);
	    }
	  }else if(opt.type.toUpper()=="NUMBERBOX"){
	    out << line.arg("max", opt.max);
	    out << line.arg("min", opt.min);
	  }else{ //string/password box
	    out << line.arg("maxlen", opt.maxlen);
	  }
	  return out;
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
	ServiceOption *optOut;

private:
	Ui::SConfigDialog *ui;

	void loadOption();
	void saveOption();

private slots:
	void checkGUI();
	void generateSample(bool go = true);

	void cancelClicked();
	void applyClicked();
};

#endif