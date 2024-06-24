#ifndef WTOAST_H
#define WTOAST_H
#include "global.h"
#include <QLabel>

class wToast;
class wToastManagerPrivate;
class WRECT_EXPORT wToastManager {
public:
    static wToastManager* getInstance();
    static void init(QWidget* parent=nullptr);
    static void destory();
    ~wToastManager();

    void addToast(wToast* toast);
    void removeToast(wToast* toast);
    QWidget* container();

private:
    wToastManager(QWidget* container);

private:
    static wToastManager* instance;
    wToastManagerPrivate* d;
};

class wToastPrivate;
class WRECT_EXPORT wToast : public QLabel
{
    Q_OBJECT
public:
    explicit wToast(const QString& text, QWidget *parent = nullptr);
    ~wToast();
    void autoClose(int msecond);

    static void showText(const QString& text);

protected:
    virtual void paintEvent(QPaintEvent *e) override;
    //virtual void enterEvent(QEvent *event) override;
    //virtual void leaveEvent(QEvent *event) override;
signals:


public:
    static const int timeout = 3000;
private:
    wToastPrivate *d;

};

#endif // WTOAST_H
