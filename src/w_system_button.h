#ifndef W_SYSTEM_BUTTON_H
#define W_SYSTEM_BUTTON_H

#include <QPushButton>

class wSystemButtonPrivate;
class wSystemButton : public QPushButton
{
    Q_OBJECT
public:
    enum Mode{
        Close=0,
        Minimize,
        Maximize,
        Restore

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
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    wSystemButtonPrivate* d;

};

#endif // W_SYSTEM_BUTTON_H
