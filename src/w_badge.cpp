#include "w_badge.h"
#include <QResizeEvent>
#include <QStylePainter>
#include <QDebug>

class CircleText : public QWidget{
public:
    explicit CircleText(QWidget* parent):QWidget(parent){
        m_count = 0;
    }

    void setCount(int count){
        m_count = count;
        this->update();
    }

    int count(){
        return m_count;
    }

protected:
    void paintEvent(QPaintEvent* e) override{
        QWidget::paintEvent(e);
        if(m_count>0){
            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(Qt::red);
            painter.setBrush(QBrush(Qt::red));
            auto size = this->size();
            int x = size.width() - 12;
            QRect rect;
            if(m_count>9){
                rect = {x - 6, 0, 18, 12};
            }else{
                rect = {x, 0, 12, 12};
            }
            painter.drawRoundedRect(rect, 6, 6);
            QFont font = this->font();
            font.setPointSize(8);
            this->setFont(font);
            painter.setPen(Qt::white);
            if(m_count>9){
                painter.drawText(rect, Qt::AlignCenter, QString::fromUtf8("9+"));
            }else{
                painter.drawText(rect, Qt::AlignCenter, QString::fromUtf8("%1").arg(m_count));
            }
        }
    }

private:
    int m_count;
};



class wBadgePrivate{
public:
    QWidget* central;
    CircleText* text;
    int margin;
};

wBadge::wBadge(QWidget* parent):QFrame(parent) {
    d = new wBadgePrivate;
    d->central = nullptr;
    d->text = new CircleText(this);
    d->text->raise();
    //d->text->setCount(99);
    d->margin = 0;
}

void wBadge::setCentralWidget(QWidget* widget){
    d->central = widget;
    widget->setParent(this);
    widget->lower();
}

void wBadge::setCount(int count){
    d->text->setCount(count);
}

int wBadge::count(){
    return d->text->count();
}

void wBadge::clear(){
    this->setCount(0);
}

QWidget* wBadge::centralWidget(){
    return d->central;
}


QSize wBadge::sizeHint() const{
    return d->central?d->central->sizeHint():QSize{30,30};
}

void wBadge::resizeEvent(QResizeEvent* e){
    QFrame::resizeEvent(e);
    auto size = e->size();
    if(d->central){
        d->central->setGeometry({0 + d->margin,0 + d->margin,size.width() - d->margin * 2,size.height() - d->margin * 2});
    }
    if(d->text){
        d->text->setGeometry({0,0,size.width(),size.height()});
    }
}

void wBadge::mousePressEvent(QMouseEvent *e){
    QFrame::mousePressEvent(e);
    emit clicked();
}
