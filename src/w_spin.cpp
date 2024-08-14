#include "w_spin.h"
#include <QWidget>
#include <QSvgWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QDebug>
class wSpinPrivate{
public:
    QWidget* parent;
    QSvgWidget* spin = nullptr;
    QFrame* mask = nullptr;
    QPropertyAnimation * animation=nullptr;
    QGraphicsOpacityEffect *effect = nullptr;
};

wSpin::wSpin(QWidget* parent)
{
    d = new wSpinPrivate;
    d->parent = parent;
}


wSpin::~wSpin(){
    delete d;
}

void wSpin::showLoading(){
    if(d->spin==nullptr){
        d->mask = new QFrame(d->parent);
        QVBoxLayout* layout = new QVBoxLayout();
        d->mask->setObjectName("mask");
        d->mask->setStyleSheet("QFrame#mask{background-color:#44000000}");
        d->spin = new QSvgWidget(d->parent);
        d->spin->load(QString::fromUtf8(":/images/Loading_16x.svg"));
        d->spin->setMinimumSize({32,32});
        //d->spin->setWindowFlags( Qt::FramelessWindowHint| Qt::ToolTip);
        //d->spin->setAttribute(Qt::WA_TranslucentBackground, true);
        d->spin->setStyleSheet("background-color: transparent;");
        layout->addWidget(d->spin, 0, Qt::AlignCenter);
        d->mask->setLayout(layout);

        d->effect = new QGraphicsOpacityEffect(d->mask);
        d->mask->setGraphicsEffect(d->effect);

        d->animation = new QPropertyAnimation(d->effect, "opacity");
        d->animation->setDuration(300);
        d->animation->setStartValue(0.0);
        d->animation->setEndValue(1.0);
    }
    d->animation->start();
    d->mask->show();
    d->mask->raise();
    d->spin->show();
    d->spin->raise();
    this->resize();
}

void wSpin::hideLoading(){
    d->spin->hide();
    d->mask->hide();
}

void wSpin::resize(){
     if(d->spin!=nullptr){
        auto rect = d->parent->geometry();
        d->mask->setGeometry(rect);
    }

}
