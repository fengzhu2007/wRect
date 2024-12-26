#ifndef W_WINDOW_H
#define W_WINDOW_H
#include "global.h"
#include <QWidget>
class wWindowPrivate;
class WRECT_EXPORT wWindow : public QWidget
{
    Q_OBJECT
public:
    explicit wWindow(QWidget *parent = nullptr);
    ~wWindow();
    void setCentralWidget(QWidget* widget);
    void resetupUi();

    void updateResizer();
    void updateNClient();
    void showMinimized();
    void showMaximized();
    void showNormal();
    void setStyleSheet(const QString& stylesheet);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

private:
    void initResizer();
    void initNClient();
    void initBackground();


private:
    wWindowPrivate* d;
signals:
};

#endif // W_WINDOW_H
