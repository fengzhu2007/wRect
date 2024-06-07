#include "w_window_resizer.h"
#include <QMouseEvent>
#include <QDebug>


    class wWindowResizerPrivate {
    public:
        wWindowResizer::Region region;

        bool moving = false;
        int offsetX = 0;
        int offsetY = 0;
    };

    wWindowResizer::wWindowResizer(QWidget* parent,Region region)
        :QFrame(parent)
    {
        d = new wWindowResizerPrivate;
        d->region = region;
        switch(d->region){
        case Left:
        case Right:
            this->setCursor(Qt::SizeHorCursor);
            break;
        case Top:
        case Bottom:
            this->setCursor(Qt::SizeVerCursor);
            break;
        case LeftTop:
        case RightBottom:
            this->setCursor(Qt::SizeFDiagCursor);
            break;
        case LeftBottom:
        case RightTop:
            this->setCursor(Qt::SizeBDiagCursor);
            break;
        }
        //this->setAttribute(Qt::WA_TranslucentBackground);


    }

    wWindowResizer::~wWindowResizer()
    {
        delete d;
    }

    void wWindowResizer::setRegion(Region region)
    {
        d->region = region;
        switch(d->region){
        case Left:
        case Right:
            this->setCursor(Qt::SizeHorCursor);
            break;
        case Top:
        case Bottom:
            this->setCursor(Qt::SizeVerCursor);
            break;
        case LeftTop:
        case RightBottom:
            this->setCursor(Qt::SizeFDiagCursor);
            break;
        case LeftBottom:
        case RightTop:
            this->setCursor(Qt::SizeBDiagCursor);
            break;
        }
    }

    wWindowResizer::Region wWindowResizer::region()
    {
        return d->region;
    }

    void wWindowResizer::mouseMoveEvent(QMouseEvent *event)
    {
        QWidget::mouseMoveEvent(event);
        if(d->moving){
            QWidget* parent = (QWidget*)parentWidget();
            if(parent!=nullptr){
                    int x = event->x() ;
                    int y = event->y();
                    x -= d->offsetX;
                    y -= d->offsetY;
                    QPoint pos = parent->pos();
                    pos.rx() += x;
                    pos.ry() += y;
                    QRect rc = parent->geometry();
                    if(d->region==wWindowResizer::Left || d->region==wWindowResizer::LeftTop || d->region==wWindowResizer::LeftBottom){
                        rc.setX(rc.x() + x);
                    }
                    if(d->region==wWindowResizer::Right || d->region==wWindowResizer::RightTop || d->region==wWindowResizer::RightBottom){
                        rc.setWidth(rc.width() + x);
                    }
                    if(d->region==wWindowResizer::Top || d->region==wWindowResizer::LeftTop || d->region==wWindowResizer::RightTop){
                        rc.setY(rc.y() + y);
                    }
                    if(d->region==wWindowResizer::Bottom || d->region==wWindowResizer::LeftBottom || d->region==wWindowResizer::RightBottom){
                        rc.setHeight(rc.height() + y);
                    }
                    parent->setGeometry(rc);
                    //parent->updateResizer();
            }
        }
    }

    void wWindowResizer::mousePressEvent(QMouseEvent *e)
    {
        QWidget::mousePressEvent(e);
        d->moving = true;
        d->offsetX = e->x();
        d->offsetY = e->y();
    }

    void wWindowResizer::mouseReleaseEvent(QMouseEvent *e)
    {
        QWidget::mouseReleaseEvent(e);
        d->moving = false;
    }

