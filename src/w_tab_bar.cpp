#include "w_tab_bar.h"
#include <QBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QMenu>
#include <QPropertyAnimation>
#include <QPainter>
#include <QMouseEvent>
#include <QVector>
#include <QDebug>
#include <QTimer>

// 1.
struct TabItem {
    QString text;
    QIcon icon;
    bool isVisible; //
};

// 2.
class wTabBarItemWidget : public QWidget {
    Q_OBJECT
public:
    explicit wTabBarItemWidget(wTabBar::Position position, wTabBar* parent = nullptr)
        : QWidget(parent), m_position(position),m_parent(parent) {
        //setCursor(Qt::PointingHandCursor);
        if(m_position == wTabBar::North || m_position == wTabBar::South){
            m_layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
        } else {
            m_layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
        }
        this->setLayout(m_layout);
        m_icon = new QLabel(this);
        m_layout->addWidget(m_icon);
        m_text = new QLabel(this);
        m_layout->addWidget(m_text);
        m_icon->setVisible(false);
    }

    void setIcon(const QIcon& icon){
        if(icon.isNull()){
            m_icon->setVisible(false);
            m_icon->setPixmap(QPixmap());

        }else{
            m_icon->setVisible(true);
            m_icon->setPixmap(icon.pixmap(m_parent->iconSize()));
        }
    }

    void setText(const QString& text){ m_text->setText(text); }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) emit clicked();
        QWidget::mousePressEvent(event);
    }

signals:
    void clicked();

private:
    QBoxLayout* m_layout;
    wTabBar::Position m_position;
    QLabel* m_icon;
    QLabel* m_text;
    wTabBar* m_parent;
};

// 3.  Private
class wTabBarPrivate {
public:
    explicit wTabBarPrivate(wTabBar* q) : q_ptr(q) {}

    //
    QVector<TabItem> tabs;
    int currentIndex = -1;
    int maxVisibleTabs = 4; //
    int indicatorHeight = 4;
    QSize iconSize{24,24};

    // UI
    QBoxLayout* layout = nullptr;
    QWidget* indicator = nullptr; //
    QPropertyAnimation* animation = nullptr;
    QMenu* overflowMenu = nullptr;

    void rebuildLayout();
    void animateIndicatorTo(int index);
    void setIndicatorGeometryDirectly(int index);
    void updateIndicatorGeometry();
    void onTabItemClicked(int index);

private:
    wTabBar* q_ptr;
};

// 4. Private
void wTabBarPrivate::rebuildLayout() {

    QLayoutItem* child;
    while ((child = layout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    for (int i = 0; i < tabs.size(); ++i) {
        if (tabs[i].isVisible) {
            auto widget = new wTabBarItemWidget(wTabBar::Position::North, q_ptr);
            widget->setIcon(tabs[i].icon);
            widget->setText(tabs[i].text);
            QObject::connect(widget, &wTabBarItemWidget::clicked, q_ptr, [this, i]() { onTabItemClicked(i); });
            layout->addWidget(widget);
        }
    }
    layout->addStretch(); //
    updateIndicatorGeometry();
}

void wTabBarPrivate::animateIndicatorTo(int index) {
    QWidget* targetWidget = nullptr;
    int visibleIdx = 0;
    for (int i = 0; i < tabs.size(); ++i) {
        if (tabs[i].isVisible) {
            if (i == index) {
                targetWidget = layout->itemAt(visibleIdx)->widget();
                break;
            }
            visibleIdx++;
        }
    }

    if (targetWidget) {
        QRect targetRect = targetWidget->geometry();
        QRect endRect(targetRect.x(), q_ptr->height() - 4, targetRect.width(), 4);

        animation->stop();
        animation->setStartValue(indicator->geometry());
        animation->setEndValue(endRect);
        animation->start();
    }
}

void wTabBarPrivate::setIndicatorGeometryDirectly(int index) {
    QWidget* targetWidget = nullptr;
    int visibleIdx = 0;
    for (int i = 0; i < tabs.size(); ++i) {
        if (tabs[i].isVisible) {
            if (i == index) {
                targetWidget = layout->itemAt(visibleIdx)->widget();
                break;
            }
            visibleIdx++;
        }
    }

    if (targetWidget) {
        QRect targetRect = targetWidget->geometry();
        QRect endRect(targetRect.x(), q_ptr->height() - 4, targetRect.width(), 4);
        indicator->setGeometry(endRect); //
    }
}

void wTabBarPrivate::updateIndicatorGeometry() {
    if (currentIndex >= 0) {
        animateIndicatorTo(currentIndex);
    }
}

void wTabBarPrivate::onTabItemClicked(int index) {
    q_ptr->setCurrentIndex(index);
}

// 5. wTabBar
wTabBar::wTabBar(QWidget* parent) : QWidget(parent), d(new wTabBarPrivate(this)) {
    d->layout = new QBoxLayout(QHBoxLayout::LeftToRight, this);
    d->layout->setContentsMargins(0, 0, 0, 0);
    d->layout->setSpacing(0);
    this->setLayout(d->layout);

    d->indicator = new QWidget(this);
    d->indicator->setStyleSheet("background-color: #007acc;");
    d->indicator->setFixedHeight(d->indicatorHeight);
    d->indicator->raise();

    d->animation = new QPropertyAnimation(d->indicator, "geometry", this);
    d->animation->setDuration(250);
    d->animation->setEasingCurve(QEasingCurve::InOutQuad);

    d->overflowMenu = new QMenu(this);
}

wTabBar::~wTabBar() {
    delete d;
}

int wTabBar::addTab(const QString &text) {
    return addTab(QIcon(), text);
}

int wTabBar::addTab(const QIcon &icon, const QString &text) {
    TabItem item;
    item.text = text;
    item.icon = icon;
    item.isVisible = (d->tabs.size() < d->maxVisibleTabs);
    d->tabs.append(item);

    d->rebuildLayout();
    if (d->currentIndex == -1) setCurrentIndex(0);
    return d->tabs.size() - 1;
}

void wTabBar::setCurrentIndex(int index) {
    if (index < 0 || index >= d->tabs.size() || index == d->currentIndex) return;

    if (!d->tabs[index].isVisible) {
        d->tabs[index].isVisible = true;

        for (int i = d->tabs.size() - 1; i >= 0; --i) {
            if (i != index && d->tabs[i].isVisible) {
                d->tabs[i].isVisible = false;
                break;
            }
        }
        d->rebuildLayout();
    }

    d->currentIndex = index;
    d->animateIndicatorTo(index);
    emit currentChanged(index);
}

QSize wTabBar::iconSize(){
    return d->iconSize;
}

void wTabBar::setIconSize(const QSize& size){
    if(d->iconSize!=size){
        d->iconSize = size;
        //update tab icon
        auto layout = this->layout();
        for(int i=0;i<d->tabs.size();i++){
            auto icon = d->tabs.at(i).icon;
            if(icon.isNull()==false){
                auto widget = static_cast<wTabBarItemWidget*>(layout->itemAt(i)->widget());
                widget->setIcon(icon);
            }
        }
    }
}
void wTabBar::setMaxVisibleTabs(int count){
    d->maxVisibleTabs = count;
}

QIcon wTabBar::tabIcon(int index){
    auto tab = d->tabs.at(index);
    return tab.icon;
}

QString wTabBar::tabText(int index){
    auto tab = d->tabs.at(index);
    return tab.text;
}

void wTabBar::setIndicatorHeight(int height){
    if(height!=d->indicatorHeight){
        d->indicatorHeight = height;
        d->indicator->setFixedHeight(d->indicatorHeight);
        d->updateIndicatorGeometry();
    }

}

void wTabBar::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
}

void wTabBar::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    d->updateIndicatorGeometry();
}

void wTabBar::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    //d->indicator->setVisible(true);
    if (d->currentIndex >= 0) {
        d->setIndicatorGeometryDirectly(d->currentIndex);
    }
    /*QTimer::singleShot(100,[this]{
        d->indicator->setVisible(true);
    });*/
}

#include "w_tab_bar.moc"
