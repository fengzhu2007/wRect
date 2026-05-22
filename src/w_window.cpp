#include "w_window.h"
#include "w_window_resizer.h"
#include "w_window_nclient.h"
#include "w_qss.h"
#include <QMenuBar>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPushButton>
#include <QLabel>


class wWindowPrivate {
public:
    wWindowResizer* regions[8];
    int resizer_size = 6;
    bool moving = false;
    QGraphicsDropShadowEffect* effect;
    wWindowNClient* nclient = nullptr;
    QFrame* background;
    bool maximize=false;


};

wWindow::wWindow(QWidget *parent)
    : QWidget{parent,Qt::Window}
{
    d = new wWindowPrivate();
    setWindowFlags(Qt::FramelessWindowHint);
    QWidget::setStyleSheet(wQSS::global());

    initResizer();
    //setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setContentsMargins(d->resizer_size,d->resizer_size,d->resizer_size,d->resizer_size);
    this->resetupUi();
}


void wWindow::setCentralWidget(QWidget* widget){
    //QMainWindow::setCentralWidget(widget);
}

wWindow::~wWindow(){
    delete d->effect;
    delete d;
}

void wWindow::initResizer()
{
    for(int i=0;i<8;i++){
        wWindowResizer::Region region = (wWindowResizer::Region)i;
        d->regions[i] = new wWindowResizer(this,region);
        d->regions[i]->raise();
    }
    updateResizer();
}

void wWindow::initNClient(){
    /*QMenuBar* menuBar = QMainWindow::menuBar();
    if(menuBar!=nullptr){
        d->nclient = new wWindowNClient(this);
        //menuBar->raise();
        //d->nclient->raise();
        d->nclient->addMenuBar(menuBar);
    }
    QMenuBar* placehoder = new QMenuBar(this);
    placehoder->setObjectName("menu_placeholder");//qss set height
    this->setMenuBar(placehoder);
    placehoder->lower();*/
    d->nclient = new wWindowNClient(this);
}

void wWindow::initBackground(){
    d->background = new QFrame(this);
    d->background->lower();
    d->background->setObjectName("background");
    d->effect = new QGraphicsDropShadowEffect();
    d->effect->setOffset(0, 0);
    d->effect->setColor(Qt::darkGray);
    d->effect->setBlurRadius(10);
    d->background->setGraphicsEffect(d->effect);
}


void wWindow::updateResizer()
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

void wWindow::updateNClient(){
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

void wWindow::resetupUi(){
    this->initNClient();
    this->initBackground();
}

void wWindow::showMinimized(){

    d->nclient->resetState();
    QWidget::showMinimized();
}

void wWindow::showMaximized(){
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

void wWindow::showNormal(){
    d->maximize = false;
    setContentsMargins(d->resizer_size,d->resizer_size,d->resizer_size,d->resizer_size);
    QWidget::showNormal();
    d->background->show();
    d->nclient->showNormalMode();
    for(int i=0;i<8;i++){
        d->regions[i]->show();
    }
}

void wWindow::setStyleSheet(const QString& stylesheet){
    QWidget::setStyleSheet(wQSS::global() + stylesheet);
}

void wWindow::setWindowIcon(const QString& pathname){
    if(d->nclient){
        d->nclient->setWindowIcon(pathname);
    }
}

wWindowNClient* wWindow::nClient(){
    return d->nclient;
}

void wWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateResizer();
    updateNClient();
    if(d->background!=nullptr){
        QRect rc = this->rect();
        d->background->setGeometry(rc.x()+d->resizer_size,rc.y()+d->resizer_size,rc.width()-2*d->resizer_size,rc.height()-2*d->resizer_size);
    }
}

void wWindow::paintEvent(QPaintEvent *event){
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
