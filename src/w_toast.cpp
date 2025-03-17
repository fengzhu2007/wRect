#include "w_toast.h"
#include <cmath>
#include <QApplication>
#include <QTimer>
#include <QPainter>
#include <QStyleOption>
#include <QTextLayout>
#include <QDebug>
wToastManager* wToastManager::instance = nullptr;


class wToastManagerPrivate{
public:
    QList<wToast*> list;
    QWidget* container = nullptr;
};


wToastManager::wToastManager(QWidget* container){
    d = new wToastManagerPrivate;
    d->container = container;
}



wToastManager* wToastManager::getInstance(){
    if(instance==nullptr){
        wToastManager::init();
    }
    return instance;
}

void wToastManager::init(QWidget* parent){
    if(instance==nullptr){
        instance = new wToastManager(parent);
    }
}

void wToastManager::destory(){
    if(instance!=nullptr){
        delete instance;
        instance = nullptr;
    }
}

wToastManager::~wToastManager(){
    delete d;
}

void wToastManager::addToast(wToast* toast){

}

void wToastManager::removeToast(wToast* toast){

}

QWidget* wToastManager::container(){
    return d->container;
}




class wToastPrivate{
public:
    //QString text;
    QTimer timer;
};

wToast::wToast(const QString& text,QWidget *parent)
    : QLabel{text,parent}
{

    this->setGraphicsEffect(nullptr);
    setWindowFlags((Qt::FramelessWindowHint | Qt::ToolTip));
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("background-color:black;color:white;");
    this->setAlignment(Qt::AlignCenter);
    this->setContentsMargins(8,0,8,0);

    d = new wToastPrivate;
    connect(&d->timer,&QTimer::timeout,this,&wToast::close);
    this->setMinimumSize(QSize(80,28));
    this->setMaximumWidth(320);
    this->setWordWrap(true);
    //this->adjustSize();
}

wToast::~wToast(){
    delete d;
}

void wToast::autoClose(int msecond){
    d->timer.start(msecond);
}


void wToast::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(0, 0, 0,180));
    painter.setPen(Qt::NoPen);

    QString text = this->text();
    QFont font = this->font();
    QFontMetrics metrics(font);

    QRect textRect = metrics.boundingRect({0,0,320,320},Qt::TextWordWrap,text);
    int textWidth = textRect.width() + 16;
    int textHeight = textRect.height() + 16;

    QRect rc = this->geometry();

    if(rc.width()<textWidth){
        rc.setX(rc.x() - (textWidth - rc.width()) / 2);
        rc.setWidth(textWidth);
    }

    rc.setY(rc.y() - textHeight);
    rc.setHeight(textHeight);
    this->setGeometry(rc);
    painter.drawRoundedRect(QRect(0,0,rc.width(),rc.height()),3,3);
    QLabel::paintEvent(e);
}


void wToast::showText(const QString& text){

    QWidget* widget = wToastManager::getInstance()->container();
    if(widget==nullptr){
        widget = qApp->activeWindow();
    }
    auto toast = new wToast(text,widget);
    QRect rect = widget->geometry();
    QPoint pos = widget->pos();
    QSize size = toast->sizeHint();
    size.setHeight(36);
   // qDebug()<<"size:"<<size;
    int x = (rect.width() - size.width()) / 2 + pos.x();
    int y = (rect.height() -  100) + pos.y();
    auto result = QRect(x,y,size.width(),size.height());
    toast->setGeometry(result);
    toast->move(QPoint(x,y));
    toast->autoClose(wToast::timeout);
    toast->show();
}
