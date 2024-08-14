#ifndef WPROGRESSBAR_H
#define WPROGRESSBAR_H

#include "global.h"
#include <QWidget>
class wProgressBarPrivate;
class WRECT_EXPORT wProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit wProgressBar(QWidget* parent=nullptr);
    ~wProgressBar();
    const QString text();
    void setText(const QString& text);
    float value();//
    void setValue(float value);
    bool infinite();
    void setInfinite(bool infinite);
    void start();
    void stop();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

private:
    wProgressBarPrivate* d;
};

#endif // WPROGRESSBAR_H
