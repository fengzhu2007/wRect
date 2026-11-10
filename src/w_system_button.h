#ifndef W_SYSTEM_BUTTON_H
#define W_SYSTEM_BUTTON_H
#include "global.h"
#include <QPushButton>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QEnterEvent>
#endif

class wSystemButtonPrivate;
class WRECT_EXPORT wSystemButton : public QPushButton
{
    Q_OBJECT
public:
    enum Mode{
        Close=0,
        Minimize,
        Maximize,
        Restore,
        Custome,

    };
    enum State{
        Normal=0,
        Hover,
        Active
    };

    explicit wSystemButton(QWidget* parent,Mode mode);
    void setMode(Mode mode);
    Mode mode();

    void setState(State state);
    State state();

    void updateIcon();

public slots:
    void onClicked();

protected:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    virtual void enterEvent(QEnterEvent *event) override;
#else
    virtual void enterEvent(QEvent *event) override;
#endif
    virtual void leaveEvent(QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    wSystemButtonPrivate* d;

};

#endif // W_SYSTEM_BUTTON_H
