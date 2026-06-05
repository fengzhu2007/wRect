#ifndef W_TAB_BAR_H
#define W_TAB_BAR_H
#include <QWidget>


class wTabBarPrivate;

class wTabBar : public QWidget
{
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

private:
    wTabBarPrivate* d;
};

#endif // W_TAB_BAR_H
