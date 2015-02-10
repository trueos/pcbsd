/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*/

/*
 Sub-classed widget for a login entry box
*/

#ifndef LOGIN_WIDGET_H
#define LOGIN_WIDGET_H

#include <QWidget>
#include <QFile>
#include <QPushButton>
#include <QToolButton>
#include <QFormLayout>
#include <QPixmap>
#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QKeyEvent>
#include <QAction>
#include <QGroupBox>
#include <QListWidget>
#include <QAbstractItemView>

class LoginWidget : public QGroupBox
{
	Q_OBJECT

  public:
	LoginWidget(QWidget* parent = 0);
	~LoginWidget();

	//Get the currently selected items
	QString currentUsername();
	QString currentPassword();
	QString currentDevicePassword();
	QString currentDE();
	void setCurrentUser(QString);
	void setCurrentDE(QString);
	void setUsernames(QStringList);
	void setDesktops(QStringList, QStringList, QStringList);
        void displayHostName(QString);
	void changeButtonIcon(QString button, QString iconFile, QSize iconSize);
	void setDesktopIconSize(QSize);
	//Change the style sheet for all widget items (see QtStyle options for more)
	void changeStyleSheet(QString item, QString style);
        
        void keyPressEvent(QKeyEvent *e);
        void retranslateUi();
        void resetFocus(QString item="");
        void allowPasswordView(bool);
	void allowUserSelection(bool);
  
  private:
  	QComboBox* listUsers;
	QComboBox* listDE;
  	QListWidget* listUserBig;
  	QLineEdit *linePassword, *lineDevPassword, *lineUsername;
	QToolButton* pushLogin;
	QToolButton* pushViewPassword;
	QToolButton *pushUserIcon, *userIcon;
	QLabel *deIcon, *devIcon;
	bool updating;

	QStringList idL, desktopIcons, desktopInfo;
	QSize desktopIconSize;
        QString hostName;
	bool userSelected, pwVisible, allowPWVisible, showUsers;

	void updateWidget();

  private slots:
	void slotUserActivated();
	//void slotUserClicked(QListWidgetItem*);
	void slotUserHighlighted(int);
	void slotUserSelected();
	void slotUserUnselected();
  	void slotTryLogin();
  	void slotChooseUser(int);
  	void slotChangePWView();
	void slotDesktopChanged(int);
	
  signals:
	//Emits these signals whenever a login request is detected
	void loginRequested(QString user, QString password);
	void UserSelected(QString user);
	void UserChanged(QString user);
	void escapePressed();

};
#endif
