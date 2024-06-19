#include "w_tags.h"
#include "w_flow_layout.h"
#include <QList>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>

class wTagsPrivate{
public:
    QStringList list;
    QList<QLabel*> tags;

};


wTags::wTags(QWidget *parent)
    : QWidget{parent}
{
    this->setStyleSheet(".QLabel{background:#F0F0F0;border-radius:4px}");
    d = new wTagsPrivate;
}

wTags::~wTags(){
    delete d;
}

void wTags::setTags(QStringList list){
    d->list.clear();
    this->addTags(list);
}

void wTags::addTags(QStringList list){
    wFlowLayout* layout = (wFlowLayout*)this->layout();
    if(layout==nullptr){
        layout = new wFlowLayout(this);
        this->setLayout(layout);
    }
    d->list+=list;
    for(int i=0;i<d->list.length();i++){
        QLabel* label = this->newTag(i);
        if(label!=nullptr){
            layout->addWidget(label);
        }
    }
}


void wTags::addTag(const QString& text){
    this->addTags(QStringList{text});
}

void wTags::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

QLabel* wTags::newTag(int i){
    QLabel* label = nullptr;
    if(i>=0 && i<d->tags.size()){
        label = d->tags.at(i);
        label->show();
        label->setText(d->list.at(i));
        return nullptr;
    }else{
        label = new QLabel(this);
        label->setContentsMargins(4,4,4,4);
        d->tags.push_back(label);
        label->show();
        label->setText(d->list.at(i));
        return label;
    }
}
