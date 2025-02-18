#ifndef W_BADGE_H
#define W_BADGE_H
#include "global.h"
#include <QFrame>
class wBadgePrivate;
class WRECT_EXPORT wBadge : public QFrame
{
    Q_OBJECT
public:
    explicit wBadge(QWidget* parent);
    void setCentralWidget(QWidget* widget);
    void setCount(int count);
    int count();
    void clear();
    QWidget* centralWidget();
    virtual QSize sizeHint() const override;

signals:
    void clicked();

protected:
    void resizeEvent(QResizeEvent* e) override;
    virtual void mousePressEvent(QMouseEvent *e) override;

private:
    wBadgePrivate* d;
};

#endif // W_BADGE_H
