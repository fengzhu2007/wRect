#ifndef WFLOWLAYOUT_H
#define WFLOWLAYOUT_H
#include "global.h"
#include <QLayout>


class wFlowLayoutPrivate;
class WRECT_EXPORT wFlowLayout : public QLayout
{
public:
    explicit wFlowLayout(QWidget* parent, int margin = 0, int spacing = -1);
    virtual void addItem(QLayoutItem *item) override;
    virtual Qt::Orientations expandingDirections() const override;
    virtual bool hasHeightForWidth() const override;
    virtual int count() const override;
    virtual QLayoutItem *itemAt(int index) const override;
    virtual QSize minimumSize() const override;
    virtual void setGeometry(const QRect &rect) override;
    virtual QSize sizeHint() const override;
    virtual QLayoutItem *takeAt(int index) override;

private:
    wFlowLayoutPrivate* d;

};

#endif // WFLOWLAYOUT_H
