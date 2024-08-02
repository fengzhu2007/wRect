#ifndef WSPIN_H
#define WSPIN_H
#include "global.h"
#include <QResizeEvent>

class wSpinPrivate;
class WRECT_EXPORT wSpin
{
public:
    explicit wSpin(QWidget* parent);
    virtual ~wSpin();
    void showLoading();
    void hideLoading();

protected:
    void resize();
    virtual void resizeEvent(QResizeEvent *event)=0;

private:
    wSpinPrivate *d;
};

#endif // WSPIN_H
