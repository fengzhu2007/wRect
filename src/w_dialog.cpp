#include "w_dialog.h"
#include "w_window_resizer.h"
#include "w_window_nclient.h"
#include "qss.h"
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

class wDialogPrivate {
public:
    wWindowResizer* regions[8];
    int resizer_size = 6;
    bool moving = false;
    QGraphicsDropShadowEffect* effect;
    wWindowNClient* nclient = nullptr;
    QFrame* background;
    bool maximize=false;
};

wDialog::wDialog(QWidget* parent)
    :QDialog(parent)
{
    d = new wDialogPrivate();
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->setStyleSheet(QSS::global());
    //initResizer();
    setAttribute(Qt::WA_TranslucentBackground);
    setContentsMargins(d->resizer_size,d->resizer_size,d->resizer_size,d->resizer_size);
}

wDialog::~wDialog(){
    delete d->effect;
    delete d;
}

void wDialog::resetupUi(){
    QLayout* layout = this->layout();
    if(layout!=nullptr){
        QMargins margins = layout->contentsMargins();
        if(!margins.isNull()){
            margins.setTop(margins.top()+32);
            layout->setContentsMargins(margins);
        }else{
            QMargins margins = {0,32,0,0};
            layout->setContentsMargins(margins);
        }
    }else{
        qDebug()<<"layout is null";
    }
    this->initNClient();
    this->initBackground();
}

void wDialog::initResizer()
{
    for(int i=0;i<8;i++){
        wWindowResizer::Region region = (wWindowResizer::Region)i;
        d->regions[i] = new wWindowResizer(this,region);
        d->regions[i]->raise();
    }
    updateResizer();
}

void wDialog::initNClient(){
    d->nclient = new wWindowNClient(this);
    d->nclient->setWindowTitle(this->windowTitle());
}

void wDialog::initBackground(){
    d->background = new QFrame(this);
    d->background->lower();
    d->background->setObjectName("background");
    d->effect = new QGraphicsDropShadowEffect();
    d->effect->setOffset(0, 0);
    d->effect->setColor(Qt::darkGray);
    d->effect->setBlurRadius(10);
    d->background->setGraphicsEffect(d->effect);
}

void wDialog::updateResizer()
{
    QRect rc = geometry();
    for(int i=0;i<8;i++){

        wWindowResizer::Region region = (wWindowResizer::Region)i;
        if(region==wWindowResizer::Left){
            d->regions[i]->setGeometry(0,d->resizer_size,d->resizer_size,rc.height() - 2*d->resizer_size);
        }else if(region==wWindowResizer::Right){
            d->regions[i]->setGeometry(rc.width() - d->resizer_size,d->resizer_size,d->resizer_size,rc.height() - 2*d->resizer_size);
        }else if(region==wWindowResizer::Top){
            d->regions[i]->setGeometry(d->resizer_size,0,rc.width() - 2*d->resizer_size,d->resizer_size);
        }else if(region==wWindowResizer::Bottom){
            d->regions[i]->setGeometry(d->resizer_size,rc.height() - d->resizer_size,rc.width() - 2*d->resizer_size,d->resizer_size);
        }else if(region==wWindowResizer::LeftTop){
            d->regions[i]->setGeometry(0,0,d->resizer_size,d->resizer_size);
        }else if(region==wWindowResizer::LeftBottom){
            d->regions[i]->setGeometry(0,rc.height() - d->resizer_size,d->resizer_size,d->resizer_size);
        }else if(region==wWindowResizer::RightTop){
            d->regions[i]->setGeometry(rc.width() - d->resizer_size,0,d->resizer_size,d->resizer_size);
        }else if(region==wWindowResizer::RightBottom){
            d->regions[i]->setGeometry(rc.width() - d->resizer_size,rc.height() - d->resizer_size,d->resizer_size,d->resizer_size);
        }
    }
}

void wDialog::updateNClient(){
    if(d->nclient!=nullptr){
        QRect rc = geometry();
        QRect rect = d->nclient->geometry();
        if(d->maximize){
            d->nclient->setGeometry(QRect(0,0,rc.width(),rect.height()));
        }else{
            d->nclient->setGeometry(QRect(d->resizer_size,d->resizer_size,rc.width() - d->resizer_size * 2,rect.height()));
        }
    }
}

void wDialog::setWindowTitle(const QString& title){
    d->nclient->setWindowTitle(title);
}

void wDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    //updateResizer();
    updateNClient();
    if(d->background!=nullptr){
        QRect rc = this->rect();
        d->background->setGeometry(rc.x()+d->resizer_size,rc.y()+d->resizer_size,rc.width()-2*d->resizer_size,rc.height()-2*d->resizer_size);
    }
}

void wDialog::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(0, 0, 0, 0));
    painter.setPen(Qt::NoPen);
    QRect rc = this->rect();
    if(d->maximize){
        painter.fillRect(0,0,rc.width(),rc.height(), Qt::white);
    }else{
        painter.fillRect(d->resizer_size,d->resizer_size,rc.width() - 2*d->resizer_size,rc.height() - 2*d->resizer_size, Qt::white);
    }
}


