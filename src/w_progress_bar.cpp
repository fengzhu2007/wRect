#include "w_progress_bar.h"
#include <QTimer>
#include <QFrame>
#include <QPropertyAnimation>
#include <QDebug>

class wProgressBarPrivate{
public:
    bool infinite;
    float value;
    int barHeight=2;
    QString text;
    QFrame* bar;
    QPropertyAnimation *animation;
};

wProgressBar::wProgressBar(QWidget* parent)
    :QWidget(parent)
{
    d = new wProgressBarPrivate;
    d->bar = new QFrame(this);
    d->bar->setObjectName(QString::fromUtf8("bar"));
    //d->bar->setStyleSheet("background:red");
    d->bar->raise();
    d->bar->hide();
    d->animation = new QPropertyAnimation(d->bar,"geometry");
    d->animation->setDuration(1000);
    d->animation->setLoopCount(-1);

    this->setMinimumHeight(d->barHeight);
}

wProgressBar::~wProgressBar(){
    delete d;
}

const QString wProgressBar::text(){
    return d->text;
}

void wProgressBar::setText(const QString& text){
    d->text = text;
}

float wProgressBar::value(){
    return d->value;
}

void wProgressBar::setValue(float value){
    d->value = value;
}

bool wProgressBar::infinite(){
    return d->infinite;
}

void wProgressBar::setInfinite(bool infinite){
    d->infinite = infinite;
}

void wProgressBar::start(){
    d->bar->show();
    d->animation->start();

}

void wProgressBar::stop(){
    d->animation->stop();
    d->bar->hide();
}

void wProgressBar::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    auto rect = this->geometry();
    //auto rc = d->bar->geometry();
    int top = qAbs(d->barHeight-rect.height());
    int width = rect.width() / 5;//bar width;
    d->bar->setGeometry({0,top,width,d->barHeight});
    auto start = QRect{-width,top,width,d->barHeight};
    auto end = QRect{rect.width(),top,width,d->barHeight};
    d->animation->setStartValue(start);
    d->animation->setEndValue(end);
}

void wProgressBar::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
}
