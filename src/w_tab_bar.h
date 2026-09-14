#ifndef W_TAB_BAR_H
#define W_TAB_BAR_H
#include <QWidget>
#include "global.h"

class wTabBarPrivate;

class WRECT_EXPORT wTabBar : public QWidget
{
    Q_OBJECT
public:
    enum Position{
        North=0,
        South,
        West,
        East
    };
    explicit wTabBar(QWidget* parent=nullptr);
    ~wTabBar();
    int	addTab(const QString &text);
    int	addTab(const QIcon &icon, const QString &text);
    void setCurrentIndex(int index);
    QSize iconSize();
    void setIconSize(const QSize& size);
    void setMaxVisibleTabs(int count);
    QIcon tabIcon(int index);
    QString tabText(int index);
    void setIndicatorHeight(int height);


signals:
    void currentChanged(int index);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
     void showEvent(QShowEvent *event) override;

private:
    wTabBarPrivate* d;
};

#endif // W_TAB_BAR_H
