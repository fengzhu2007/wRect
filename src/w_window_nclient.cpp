#include "w_window_nclient.h"
#include "w_system_button.h"
#include "w_main_window.h"
#include "w_dialog.h"
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QLabel>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QApplication>
#include <QAction>
#include <QMenuBar>
#ifdef Q_OS_WIN
#include <windows.h>
#endif


class wWindowNClientPrivate{

public:
    wSystemButton* close = nullptr;
    wSystemButton* minimize = nullptr;
    wSystemButton* maximize = nullptr;
    QLabel* logo;
    QLabel* title;
    QMenuBar* menuBar = nullptr;
    int offsetX;
    int offsetY;
    bool moving = false;

    QAction* sysRestore = nullptr;
    QAction* sysMove = nullptr;
    QAction* sysResize = nullptr;
    QAction* sysMinimize = nullptr;
    QAction* sysMaximize = nullptr;
    QAction* sysClose = nullptr;



    //wMainWindow* parent = nullptr;
    wWindowNClient::Mode mode;

};




wWindowNClient::wWindowNClient(wMainWindow* parent)
    :QFrame(parent)
{
    //button size 46*32
    //button image size 10*10
    //setAttribute(Qt::WA_TranslucentBackground);
    this->initView(Window);

}

wWindowNClient::wWindowNClient(wDialog* parent)
:QFrame(parent){
    this->initView(Dialog);
}

void wWindowNClient::initView(Mode mode){
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &wWindowNClient::showContextMenu);
    d = new wWindowNClientPrivate;
    d->title = nullptr;
    d->mode = mode;
    d->logo = new QLabel(this);
    d->logo->setObjectName("system_logo");
    //d->logo->setText(tr("LOGO"));
    d->logo->setPixmap(QPixmap(QString::fromUtf8(":/images/logo.png")).scaled(QSize(20,20),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    //d->logo->setGeometry(QRect(0,0,32,32));
    if(mode==Window){
        d->close = new wSystemButton(this,wSystemButton::Close);
        d->minimize = new wSystemButton(this,wSystemButton::Minimize);
        d->maximize = new wSystemButton(this,wSystemButton::Maximize);
    }else if(mode==Dialog){
        d->close = new wSystemButton(this,wSystemButton::Close);
        ///d->minimize = new wSystemButton(this,wSystemButton::Minimize);
       // d->maximize = new wSystemButton(this,wSystemButton::Maximize);
        d->minimize = nullptr;
        d->maximize = nullptr;
    }

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addSpacing(6);
    layout->addWidget(d->logo);
    layout->addStretch(1);
    if(d->minimize!=nullptr)
    layout->addWidget(d->minimize);
    if(d->maximize!=nullptr)
    layout->addWidget(d->maximize);
    layout->addWidget(d->close);
    this->setLayout(layout);
}

void wWindowNClient::addMenuBar(QMenuBar* menuBar){
    menuBar->setParent(this);
    d->menuBar = menuBar;
    QHBoxLayout* layout = (QHBoxLayout*)this->layout();
    if(d->title==nullptr){
        layout->insertWidget(2,menuBar);
    }else{
        layout->insertWidget(3,menuBar);
    }

}

void wWindowNClient::showMaximizedMode(){
    d->maximize->setMode(wSystemButton::Restore);
}

void wWindowNClient::showNormalMode(){
    d->maximize->setMode(wSystemButton::Maximize);
}

void wWindowNClient::resetState(){
    //d->close->setState(wSystemButton::Normal);
   // d->minimize->setState(wSystemButton::Normal);
    //d->maximize->setState(wSystemButton::Normal);
    //QEvent leaveEvent(QEvent::Leave);
    //QCoreApplication::sendEvent(d->minimize,&leaveEvent);
}

QMenuBar* wWindowNClient::menuBar(){
    return d->menuBar;
}

wWindowNClient::Mode wWindowNClient::mode(){
    return d->mode;
}

void wWindowNClient::setWindowTitle(const QString& title){
    if(d->title==nullptr){
        d->title = new QLabel(this);
        d->title->setObjectName("system_title");
        QHBoxLayout* layout = (QHBoxLayout*)this->layout();
        layout->insertWidget(2,d->title);
    }
    d->title->setText(title);
}

void wWindowNClient::showContextMenu(const QPoint &pos){
    QMenu contextMenu(this);
    if(d->sysClose==nullptr){
        //init system menu
        QStyle* style =  QApplication::style();
        //QString translatedText = QApplication::translate("QMessageBox", "OK");
        d->sysClose = new QAction(style->standardIcon(QStyle::SP_TitleBarCloseButton).pixmap(10,10),tr("Close(&C)\tAlt+F4"),this);
        d->sysMinimize = new QAction(style->standardIcon(QStyle::SP_TitleBarMinButton).pixmap(10,10),tr("Minimize(&N)"),this);
        d->sysMaximize = new QAction(style->standardIcon(QStyle::SP_TitleBarMaxButton).pixmap(10,10),tr("Maximize(&X)"),this);
        d->sysRestore = new QAction(style->standardIcon(QStyle::SP_TitleBarNormalButton).pixmap(10,10),tr("Restore(&R)"),this);
        d->sysMove = new QAction(tr("Move(&M)"),this);
        d->sysResize = new QAction(tr("Resize(&S)"),this);
        connect(d->sysClose,&QAction::triggered,this,&wWindowNClient::onSystemClose);
        connect(d->sysMinimize,&QAction::triggered,this,&wWindowNClient::onSystemMinimize);
        connect(d->sysMaximize,&QAction::triggered,this,&wWindowNClient::onSystemMaximize);
        connect(d->sysRestore,&QAction::triggered,this,&wWindowNClient::onSystemRestore);
        connect(d->sysMove,&QAction::triggered,this,&wWindowNClient::onSystemMove);
        connect(d->sysResize,&QAction::triggered,this,&wWindowNClient::onSystemResize);
    }

    if(parentWidget()->isMaximized()){
        d->sysMove->setEnabled(false);
        d->sysMaximize->setEnabled(false);
        d->sysResize->setEnabled(false);
        d->sysRestore->setEnabled(true);
    }else{
        d->sysMove->setEnabled(true);
        d->sysMaximize->setEnabled(true);
        d->sysResize->setEnabled(true);
        d->sysRestore->setEnabled(false);
    }

    if(d->mode==Dialog){
        d->sysResize->setEnabled(false);
        d->sysMaximize->setEnabled(false);
        d->sysMinimize->setEnabled(false);
    }
    contextMenu.addAction(d->sysRestore);
    contextMenu.addAction(d->sysMove);
    contextMenu.addAction(d->sysResize);
    contextMenu.addAction(d->sysMinimize);
    contextMenu.addAction(d->sysMaximize);
    contextMenu.addSeparator();
    contextMenu.addAction(d->sysClose);
    contextMenu.exec(mapToGlobal(pos));
}

void wWindowNClient::onSystemClose(){
    if(d->mode==Window){
        wMainWindow* window = (wMainWindow*)parentWidget();
        window->close();
    }else if(d->mode==Dialog){
        wDialog* window = (wDialog*)parentWidget();
        window->close();
    }
}

void wWindowNClient::onSystemMinimize(){
    if(d->mode==Window){
        wMainWindow* window = (wMainWindow*)parentWidget();
        window->showMinimized();
    }else if(d->mode==Dialog){

    }
}

void wWindowNClient::onSystemMaximize(){
    if(d->mode==Window){
        wMainWindow* window = (wMainWindow*)parentWidget();
        window->showMaximized();
    }else if(d->mode==Dialog){

    }
}

void wWindowNClient::onSystemRestore(){
    if(d->mode==Window){
        wMainWindow* window = (wMainWindow*)parentWidget();
        window->showNormal();
    }else if(d->mode==Dialog){

    }
}

void wWindowNClient::onSystemMove(){
    QWidget *window = parentWidget();
    QPoint globalPos = window->mapToGlobal(QPoint(window->width() / 2, this->height() / 2));
    QCursor::setPos(globalPos);
    //window->setCursor(Qt::SizeAllCursor);
    //QApplication::setOverrideCursor(QCursor(Qt::SizeAllCursor));
}

void wWindowNClient::onSystemResize(){


    QWidget *window = parentWidget();
    QPoint globalPos = window->mapToGlobal(QPoint(window->width() / 2, window->height() / 2));
    QCursor::setPos(globalPos);
    //window->setCursor(Qt::SizeAllCursor);
    //QApplication::setOverrideCursor(QCursor(Qt::SizeAllCursor));
}

void wWindowNClient::paintEvent(QPaintEvent *e)
{
    //QFrame::paintEvent(e);
    Q_UNUSED(e);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Frame, &opt, &p, this);
}

void wWindowNClient::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
    if(d->moving){

        //QPoint cursorPos = e->pos();
        int x = e->x() ;
        int y = e->y();
        x -= d->offsetX;
        y -= d->offsetY;
        //wMainWindow* window = (wMainWindow*)topLevelWidget();
        QWidget* window = parentWidget();
        QPoint pos = window->pos();
        pos.rx() += x;
        pos.ry() += y;
        window->move(pos);
    }else{
        if(parentWidget()->isMaximized()==false){
            if(abs(d->offsetX - e->x())>3 || abs(d->offsetY-e->y())>3){
                d->moving = true;
            }
        }
        parentWidget()->unsetCursor();
    }
}

void wWindowNClient::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
    //d->moving = true;
    d->offsetX = e->x();
    d->offsetY = e->y();
}

void wWindowNClient::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);
    d->moving = false;
}

void wWindowNClient::mouseDoubleClickEvent(QMouseEvent *event){
    if(d->mode==Window){
        wMainWindow* window = (wMainWindow*)parentWidget();
        if(window->isMaximized()){
            window->showNormal();
        }else{
            window->showMaximized();
        }
    }else if(d->mode==Dialog){

    }

}
