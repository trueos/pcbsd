#ifndef WEBURL_H
#define WEBURL_H

#include <QWidget>
#include <QPicture>
namespace Ui {
    class WebURL;
}

class WebURL : public QWidget
{
    Q_OBJECT

public:
    explicit WebURL(QWidget *parent = 0);
    ~WebURL();

	void setup(const QString& URL, const char* PictureFile = NULL);
    void setup(const QString& URL, const QIcon *ico);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WebURL *ui;

private slots:
	void on_LaunchBrowserButton_clicked();
};

#endif // WEBURL_H
