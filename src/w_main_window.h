#ifndef W_MAIN_WINDOW_H
#define W_MAIN_WINDOW_H
#include "global.h"
#include <QMainWindow>

class wMainWindowPrivate;
class WRECT_EXPORT wMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit wMainWindow(QWidget *parent = nullptr);
    ~wMainWindow();
    void setCentralWidget(QWidget* widget);
    void resetupUi();

    void updateResizer();
    void updateNClient();
    void showMinimized();
    void showMaximized();
    void showNormal();

    QMenuBar* menuBar();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

private:
    void initResizer();
    void initNClient();
    void initBackground();



private:
    wMainWindowPrivate* d;


signals:

};

#endif // W_MAIN_WINDOW_H
