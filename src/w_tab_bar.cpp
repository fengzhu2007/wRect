#include "w_tab_bar.h"
#include <QBoxLayout>
#include <QIcon>
#include <QLabel>


class wTabBarItemWidget : public QWidget{
public:
    explicit wTabBarItemWidget(wTabBar::Position position,QWidget* parent):QWidget(parent),
        m_position(position){
        if(m_position==wTabBar::North || m_position==wTabBar::South){
            m_layout = new QBoxLayout(QBoxLayout::LeftToRight,this);
        }else{
            m_layout = new QBoxLayout(QBoxLayout::TopToBottom,this);
        }
        this->setLayout(m_layout);
        m_icon = new QLabel(this);
        m_layout->addWidget(m_icon);
        m_text = new QLabel(this);
        m_layout->addWidget(m_text);
    }

    void setIcon(const QIcon& icon){
        if(icon.isNull()){
            //m_icon->setPixmap(icon.pixmap({24,24}));
            m_icon->setPixmap({});
        }else{
            m_icon->setPixmap(icon.pixmap({24,24}));
        }
    }

    void setText(const QString& text){
        m_text->setText(text);
    }
private:
    QBoxLayout* m_layout;
    wTabBar::Position m_position;
    QLabel* m_icon;
    QLabel* m_text;
};




class wTabBarPrivate{
public:
    wTabBar::Position position = wTabBar::North;

};


wTabBar::wTabBar(QWidget* parent):QWidget(parent) {
    d = new wTabBarPrivate;
    auto layout = new QBoxLayout(QHBoxLayout::LeftToRight,this);
    this->setLayout(layout);
}


wTabBar::~wTabBar(){
    delete d;
}


int	wTabBar::addTab(const QString &text){
    QIcon icon;
    return this->addTab(icon,text);
}

int	wTabBar::addTab(const QIcon &icon, const QString &text){
    auto widget = new wTabBarItemWidget(d->position,this);
    auto layout = this->layout();
    layout->addWidget(widget);
    widget->setIcon(icon);
    widget->setText(text);

}
