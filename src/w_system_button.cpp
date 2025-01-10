#include "w_system_button.h"
#include "w_main_window.h"
#include "w_dialog.h"
#include "w_window.h"
#include "w_window_nclient.h"
#include "w_qss.h"
#include <QMouseEvent>
#include <QDebug>
class wSystemButtonPrivate{
public:
    wSystemButton::Mode mode;
    wSystemButton::State state = wSystemButton::Normal;
};


wSystemButton::wSystemButton(QWidget* parent,Mode mode)
    :QPushButton(parent)
{
    d = new wSystemButtonPrivate;
    this->setMode(mode);
    connect(this,&QPushButton::clicked,this,&wSystemButton::onClicked);
}


void wSystemButton::setMode(Mode mode){
    d->mode = mode;
    updateIcon();
}

wSystemButton::Mode wSystemButton::mode(){
    return d->mode;
}

void wSystemButton::setState(State state){
    d->state = state;
    updateIcon();
}

wSystemButton::State wSystemButton::state(){
    return d->state;
}

void wSystemButton::updateIcon(){
    QString mode;
    QString state;
    if(d->mode==wSystemButton::Close){
        mode = "Close";
    }else if(d->mode==wSystemButton::Minimize){
        mode = "Minimize";
    }else if(d->mode==wSystemButton::Maximize){
        mode = "Maximize";
    }else if(d->mode==wSystemButton::Restore){
        mode = "Restore";
    }
    if(d->state==wSystemButton::Normal){
        state = "";
    }else if(d->state==wSystemButton::Hover){
        state = "Hover";
    }else if(d->state==wSystemButton::Active){
        state = "Active";
    }
    //dark theme
    if(wQSS::theme()==wQSS::Dark){
        setIcon(QIcon(QString::fromUtf8(":/images/%1%2_16x.svg").arg(mode).arg("Active")));
    }else{
        setIcon(QIcon(QString::fromUtf8(":/images/%1%2_16x.svg").arg(mode).arg(state)));
    }
}

void wSystemButton::onClicked(){
    wWindowNClient* nclient = (wWindowNClient*)parentWidget();
    wWindowNClient::Mode mode = nclient->mode();
    if(mode==wWindowNClient::MainWindow){
        wMainWindow* window = (wMainWindow*)nclient->parentWidget();
        if(d->mode==wSystemButton::Close){
            window->close();
        }else if(d->mode==wSystemButton::Minimize){
            window->showMinimized();
        }else if(d->mode==wSystemButton::Maximize){
            window->showMaximized();
        }else if(d->mode==wSystemButton::Restore){
            window->showNormal();
        }
    }else if(mode==wWindowNClient::Dialog){
        wDialog* window = (wDialog*)nclient->parentWidget();
        if(d->mode==wSystemButton::Close){
            window->close();
        }else if(d->mode==wSystemButton::Minimize){
            window->showMinimized();
        }else if(d->mode==wSystemButton::Maximize){
            window->showMaximized();
        }else if(d->mode==wSystemButton::Restore){
            window->showNormal();
        }
    }else if(mode==wWindowNClient::Window){
        wWindow* window = (wWindow*)nclient->parentWidget();
        if(d->mode==wSystemButton::Close){
            window->close();
        }else if(d->mode==wSystemButton::Minimize){
            window->showMinimized();
        }else if(d->mode==wSystemButton::Maximize){
            window->showMaximized();
        }else if(d->mode==wSystemButton::Restore){
            window->showNormal();
        }
    }

}

void wSystemButton::enterEvent(QEvent *event){
    QPushButton::enterEvent(event);
    setState(wSystemButton::Hover);

}

void wSystemButton::leaveEvent(QEvent *event){
    QPushButton::leaveEvent(event);
    setState(wSystemButton::Normal);
}

void wSystemButton::mousePressEvent(QMouseEvent *event){
    QPushButton::mousePressEvent(event);
    setState(wSystemButton::Active);
}

void wSystemButton::mouseMoveEvent(QMouseEvent *event){
    QPushButton::mouseMoveEvent(event);
    QPoint pos = event->pos();
    int x = pos.x();
    int y = pos.y();
    QRect rect = this->rect();
    if(x<=0 || y<=0 || x>rect.width() || y>rect.height()){
        //out
        setState(wSystemButton::Hover);
    }
    event->accept();
}

void wSystemButton::mouseReleaseEvent(QMouseEvent *event){
    QPushButton::mouseReleaseEvent(event);
    setState(wSystemButton::Normal);
}

