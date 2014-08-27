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

#define BASEWEBURL QString("http://127.0.0.1:8885")

class MainUI : public QMainWindow{
	Q_OBJECT
public:
	MainUI();
	~MainUI();

private:
	bool DEBUG;
	QWebView *webview;
	QProgressBar *progressBar;
	QAction *backA, *forA, *refA, *stopA, *progA;
	
private slots:
	void slotSingleInstance();
	void LinkClicked(const QUrl &url);
	void PageStartLoading();
	void PageLoadProgress(int);
	void PageDoneLoading(bool);
	void StatusTextChanged(const QString&);

	//Button Actions
	void GoBack();
	void GoForward();
	void GoRefresh();
	void GoStop();
};

#endif