#ifndef W_WINDOW_NCLIENT_H
#define W_WINDOW_NCLIENT_H
#include "global.h"
#include <QFrame>
#include <QMenuBar>
class wMainWindow;
class wDialog;
class wWindow;
class wWindowNClientPrivate;
class WRECT_EXPORT wWindowNClient : public QFrame
{
    Q_OBJECT
public:
    enum Mode {
        MainWindow=0,
        Dialog,
        Window
    };
    enum IndexOrder{
        Logo = 1000,
        Title,
        Stretch,
    };

    explicit wWindowNClient(wMainWindow* parent);
    explicit wWindowNClient(wDialog* parent);
    explicit wWindowNClient(wWindow* parent);
    void addMenuBar(QMenuBar* menuBar);
    void showMaximizedMode();
    void showNormalMode();
    void resetState();
    QMenuBar* menuBar();
    Mode mode();
    void setWindowTitle(const QString& title);
    void setWindowIcon(const QString& pathname);
    void buttonsReset();
    int widgetCount();
    void addWidget(int index,QWidget* widget);
    int findWidget(int name);
    int findWidget(QWidget* widget);
    void setStretch(QWidget* stretch);


public slots:
    void showContextMenu(const QPoint &pos);

    void onSystemClose();
    void onSystemMinimize();
    void onSystemMaximize();
    void onSystemRestore();
    void onSystemMove();
    void onSystemResize();



protected:
    virtual void paintEvent(QPaintEvent *e) override;
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void mouseMoveEvent(QMouseEvent *e) override;
    virtual void mouseReleaseEvent(QMouseEvent *e) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    void initView(Mode mode);

private:
    wWindowNClientPrivate* d;
};

#endif // W_WINDOW_NCLIENT_H
