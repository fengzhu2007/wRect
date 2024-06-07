#include "w_main_window.h"
#include "w_window_resizer.h"
#include "w_window_nclient.h"
#include "qss.h"
#include <QMenuBar>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QDebug>


class wMainWindowPrivate {
public:
    wWindowResizer* regions[8];
    int resizer_size = 6;
    bool moving = false;
    QGraphicsDropShadowEffect* effect;
    wWindowNClient* nclient = nullptr;
    QFrame* background;
    bool maximize=false;


};

wMainWindow::wMainWindow(QWidget *parent)
    : QMainWindow{parent}
{
    d = new wMainWindowPrivate();
    setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet(QSS::global());

    initResizer();
    //setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setContentsMargins(d->resizer_size,d->resizer_size,d->resizer_size,d->resizer_size);
}


void wMainWindow::setCentralWidget(QWidget* widget){
    QMainWindow::setCentralWidget(widget);
}

wMainWindow::~wMainWindow(){
    delete d->effect;
    delete d;
}

void wMainWindow::initResizer()
{
    for(int i=0;i<8;i++){
        wWindowResizer::Region region = (wWindowResizer::Region)i;
        d->regions[i] = new wWindowResizer(this,region);
        d->regions[i]->raise();
    }
    updateResizer();
}

void wMainWindow::initNClient(){
    QMenuBar* menuBar = QMainWindow::menuBar();
    if(menuBar!=nullptr){
        d->nclient = new wWindowNClient(this);
        //menuBar->raise();
        //d->nclient->raise();
        d->nclient->addMenuBar(menuBar);
    }
    QMenuBar* placehoder = new QMenuBar(this);
    placehoder->setObjectName("menu_placeholder");//qss set height
    this->setMenuBar(placehoder);
    placehoder->lower();
}

void wMainWindow::initBackground(){
    d->background = new QFrame(this);
    d->background->lower();
    d->background->setObjectName("background");
    d->effect = new QGraphicsDropShadowEffect();
    d->effect->setOffset(0, 0);
    d->effect->setColor(Qt::darkGray);
    d->effect->setBlurRadius(10);
    d->background->setGraphicsEffect(d->effect);
}


void wMainWindow::updateResizer()
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

void wMainWindow::updateNClient(){
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

void wMainWindow::resetupUi(){
    this->initNClient();
    this->initBackground();
}

void wMainWindow::showMinimized(){

    d->nclient->resetState();
    QWidget::showMinimized();
}

void wMainWindow::showMaximized(){
    d->maximize = true;
    setContentsMargins(0,0,0,0);
    QWidget::showMaximized();
    d->background->hide();
    d->nclient->showMaximizedMode();
    //hide all siderbar
    for(int i=0;i<8;i++){
        d->regions[i]->hide();
    }
}

void wMainWindow::showNormal(){
    d->maximize = false;
    setContentsMargins(d->resizer_size,d->resizer_size,d->resizer_size,d->resizer_size);
    QWidget::showNormal();
    d->background->show();
    d->nclient->showNormalMode();
    for(int i=0;i<8;i++){
        d->regions[i]->show();
    }
}

QMenuBar* wMainWindow::menuBar(){
    if(d->nclient!=nullptr){
        return d->nclient->menuBar();
    }else{
        return QMainWindow::menuBar();
    }
}

void wMainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateResizer();
    updateNClient();
    if(d->background!=nullptr){
        QRect rc = this->rect();
        d->background->setGeometry(rc.x()+d->resizer_size,rc.y()+d->resizer_size,rc.width()-2*d->resizer_size,rc.height()-2*d->resizer_size);
    }
}

void wMainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(0, 0, 0, 0));
    painter.setPen(Qt::NoPen);
    //painter.drawRect(this->rect());
    QRect rc = this->rect();
    //painter.fillRect(rc,QColor(255,255,0,100));
    //painter.fillRect(d->resizer_size,0,rc.width() - d->resizer_size,d->resizer_size,QColor(255,255,0,100));
    //painter.fillRect(d->resizer_size,0,rc.width() - d->resizer_size,d->resizer_size,QColor(255,255,0,100));
    if(d->maximize){
        painter.fillRect(0,0,rc.width(),rc.height(), Qt::white);
    }else{
        painter.fillRect(d->resizer_size,d->resizer_size,rc.width() - 2*d->resizer_size,rc.height() - 2*d->resizer_size, Qt::white);
        /*QPen pen = Qt::SolidLine;
        pen.setWidth(6);
        pen.setColor(QColor("#019fb9ff"));
        painter.setPen(pen);
        painter.drawLine(0,0,0,rc.height());*/
        /*QPen pen = Qt::SolidLine;
        pen.setWidthF(0.5);
        pen.setColor(QColor("#000"));
        painter.setPen(pen);
        painter.drawLine(d->resizer_size-1,d->resizer_size-1,d->resizer_size-1,rc.height() - 1*(d->resizer_size));
        painter.drawLine(d->resizer_size,d->resizer_size-1,rc.width()-d->resizer_size,d->resizer_size-1);
        painter.drawLine(rc.width() - d->resizer_size + 1,d->resizer_size-1,rc.width()-d->resizer_size + 1,rc.height() - d->resizer_size);
        painter.drawLine(d->resizer_size-1,rc.height()-d->resizer_size + 1,rc.width()-d->resizer_size,rc.height()-d->resizer_size + 1);*/
    }
    //border color #9b9fb9
}

