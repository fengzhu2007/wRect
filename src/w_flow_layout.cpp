#include "w_flow_layout.h"
#include <QWidget>
#include <QDebug>
class wFlowLayoutPrivate{
public:
    QList<QLayoutItem* >items;

};


wFlowLayout::wFlowLayout(QWidget* parent, int margin, int spacing)
    :QLayout(parent){
    setMargin(margin);
    setSpacing(spacing);
    d = new wFlowLayoutPrivate;
}

void wFlowLayout::addItem(QLayoutItem *item)
{
    d->items.push_back(item);
}



Qt::Orientations wFlowLayout::expandingDirections() const
{
    return Qt::Horizontal | Qt::Vertical;
}

bool wFlowLayout::hasHeightForWidth() const
{
    return false;
}

int wFlowLayout::count() const
{
    //return QLayout::count();
    return d->items.count();
}

QLayoutItem *wFlowLayout::itemAt(int index) const
{
    if(index>=0 && index<d->items.length()){
        return d->items.at(index);
    }else{
        return nullptr;
    }

}

QSize wFlowLayout::minimumSize() const
{
    /*int n = this->count();
    int h = 0;
    for(int i=0;i<n;i++){
        QLayoutItem* item = itemAt(i);
        QSize size = item->minimumSize();
        h += size.height();
    }
    return QSize(100,h);*/
    return this->sizeHint();
}

void wFlowLayout::setGeometry(const QRect &rect)
{
    int sp = this->spacing();
    int n = this->count();
    //int h = 0;
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    QRect rc = parentWidget()->geometry();
    int width = rc.width();
    int line_width = width;
    int lines_height = 0;
    int line_num = 0;
    //qDebug()<<"tag n"<<n<<rc;
    for(int i=0;i<n;i++){
        QLayoutItem* item = itemAt(i);
        //QSize size = item->widget()->size();
        QSize size = item->sizeHint();

        //qDebug()<<"tag size:"<<size<<";widget geo:"<<item->widget()->geometry()<<"item geo:"<<item->geometry();
        //qDebug()<<"size:"<<item->sizeHint()<<";widget size:"<<item->widget()->geometry()<<";widget min size:"<<item->widget()->minimumSize()<<";widget min size hint"<<item->widget()->minimumSizeHint()
                 //<<";widget max size:"<<item->widget()->maximumSize();

        w = size.width();
        h = size.height();
        if(w>line_width && line_num>0){
            //next line
            x = sp;
            y = sp + lines_height;
            item->setGeometry(QRect(x,y,w,h));
            //qDebug()<<"item set geo1:"<<QRect(x,y,w,h);
            line_num = 1;
        }else{
            item->setGeometry(QRect(x,y,w,h));
             //qDebug()<<"item set geo2:"<<QRect(x,y,w,h);
            x+=w;
            x+=sp;
            line_width -= (w+sp);
            if(line_num==0){
                lines_height += h;
            }
            line_num+=1;
        }
    }
}

QSize wFlowLayout::sizeHint() const
{
    int sp = this->spacing();
    int n = this->count();
    int h = sp;
    int w = 0;
    QRect rc = parentWidget()->geometry();
    int width = rc.width();
    int line_height_total = 0;
    int line_height = 0;
    int line_num = 0;
    for(int i=0;i<n;i++){
        QLayoutItem* item = itemAt(i);
        QSize size = item->sizeHint();

        if(size.width()>width && line_num>0){
            //next line
            line_num = 0;
            width = rc.width();
            line_height_total += line_height;
            line_height = 0;
        }
        width -= size.width();
        if(line_height<size.height()){
            line_height = size.height();
        }
    }
    line_height_total += line_height;
    return QSize(rc.width(),line_height_total);
}

QLayoutItem *wFlowLayout::takeAt(int index){
    return nullptr;
}
