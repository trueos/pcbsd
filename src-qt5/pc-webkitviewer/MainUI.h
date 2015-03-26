#ifndef _PCBSD_APPCAFE_MAIN_WEB_UI_H
#define _PCBSD_APPCAFE_MAIN_WEB_UI_H

#include <QMainWindow>
#include <QWebView>
#include <QUrl>
#include <QProcess>
#include <QDebug>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QToolBar>
#include <QAction>
#include <QWebHistory>
#include <QFile>
#include <QTextStream>
#include <QNetworkReply>
#include <QMenu>
#include <QToolButton>
#include <QFrame>
#include <QLineEdit>
#include <QToolButton>
#include <QShortcut>

#include <pcbsd-utils.h>

class MainUI : public QMainWindow{
	Q_OBJECT
public:
	MainUI(bool debugmode = false, QString fileURL = "");
	~MainUI();

private:
	bool DEBUG, AUTHCOMPLETE;
	QString baseURL;
	QWebView *webview;
	QProgressBar *progressBar;
	QMenu *listMenu;
	QAction *backA, *forA, *refA, *stopA, *progA;
	QFrame *group_search;
	QLineEdit *line_search;
	QToolButton *tool_search;
	QString lastsearch;
	QShortcut *ctrlF, *esc;
	
private slots:
	void slotSingleInstance();
	void LinkClicked(const QUrl &url);
	void PageStartLoading();
	void PageLoadProgress(int);
	void PageDoneLoading(bool);
	void authenticate(QNetworkReply*);
	void StatusTextChanged(const QString&);
	void loadHomePage();

	//Button Actions
	void GoBack();
	void GoForward();
	void GoRefresh();
	void GoStop();
	void GoClose();
	void GoSearch();
	void openSearch();
	void closeSearch();
};

#endif