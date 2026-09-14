#include "w_popup_panel.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QScreen>
#include <QGuiApplication>
#include <QFocusEvent>
#include <QDebug>

class wPopupPanelPrivate {
public:
    QWidget *contentWidget = nullptr;
    QVBoxLayout *layout = nullptr;
    bool autoClose = true;
    int borderRadius = 6;
    int contentPadding = 8;
    QColor bgColor = QColor(0, 0, 0, 160);
    QWidget *targetWidget = nullptr;
    wPopupPanel::Direction targetDirection = wPopupPanel::BottomCenter;
};

wPopupPanel::wPopupPanel(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    d = new wPopupPanelPrivate;

    d->layout = new QVBoxLayout(this);
    d->layout->setContentsMargins(d->contentPadding, d->contentPadding,
                                  d->contentPadding, d->contentPadding);
    d->layout->setSpacing(0);

    setMinimumSize(40, 30);
}

wPopupPanel::~wPopupPanel()
{
    delete d;
}

void wPopupPanel::setContentWidget(QWidget *widget)
{
    if (d->contentWidget) {
        d->layout->removeWidget(d->contentWidget);
        d->contentWidget->setParent(nullptr);
    }
    d->contentWidget = widget;
    if (widget) {
        d->layout->addWidget(widget);
    }
    autoSize();
}

QWidget *wPopupPanel::contentWidget() const
{
    return d->contentWidget;
}

void wPopupPanel::setAutoClose(bool enable)
{
    d->autoClose = enable;
}

bool wPopupPanel::autoClose() const
{
    return d->autoClose;
}

void wPopupPanel::setBorderRadius(int radius)
{
    d->borderRadius = radius;
    update();
}

int wPopupPanel::borderRadius() const
{
    return d->borderRadius;
}

void wPopupPanel::setContentPadding(int padding)
{
    d->contentPadding = padding;
    d->layout->setContentsMargins(padding, padding, padding, padding);
    autoSize();
}

int wPopupPanel::contentPadding() const
{
    return d->contentPadding;
}

void wPopupPanel::setBackgroundColor(const QColor &color)
{
    d->bgColor = color;
    update();
}

QColor wPopupPanel::backgroundColor() const
{
    return d->bgColor;
}

void wPopupPanel::showPopup(QWidget *target, Direction direction)
{
    if (!target) return;
    d->targetWidget = target;
    d->targetDirection = direction;

    // Initial size estimate for positioning
    autoSize();
    QPoint pos = calcPosition(target, direction);

    QScreen *screen = QGuiApplication::screenAt(target->mapToGlobal(target->rect().center()));
    if (screen) {
        QRect screenGeo = screen->availableGeometry();
        pos.setX(qBound(screenGeo.left(), pos.x(), screenGeo.right() - width()));
        pos.setY(qBound(screenGeo.top(), pos.y(), screenGeo.bottom() - height()));
    }

    move(pos);
    show();

    // After show(), the content layout is activated.
    // Re-calculate size and position to fix first-show size inaccuracy.
    autoSize();
    QPoint newPos = calcPosition(target, direction);
    if (screen) {
        QRect screenGeo = screen->availableGeometry();
        newPos.setX(qBound(screenGeo.left(), newPos.x(), screenGeo.right() - width()));
        newPos.setY(qBound(screenGeo.top(), newPos.y(), screenGeo.bottom() - height()));
    }
    if (newPos != pos)
        move(newPos);

    emit shown();
}

void wPopupPanel::showPopup(const QPoint &globalPos)
{
    autoSize();

    QPoint pos = globalPos;
    QScreen *screen = QGuiApplication::screenAt(globalPos);
    if (screen) {
        QRect screenGeo = screen->availableGeometry();
        pos.setX(qBound(screenGeo.left(), pos.x(), screenGeo.right() - width()));
        pos.setY(qBound(screenGeo.top(), pos.y(), screenGeo.bottom() - height()));
    }

    move(pos);
    show();
    emit shown();
}

void wPopupPanel::autoSize()
{
    if (!d->contentWidget) return;
    adjustSize();
}

QPoint wPopupPanel::calcPosition(QWidget *target, Direction direction)
{
    QPoint targetPos = target->mapToGlobal(QPoint(0, 0));
    QSize targetSize = target->size();
    QSize popupSize = size();
    int x = 0, y = 0;

    switch (direction) {
    case TopLeft:
        x = targetPos.x();
        y = targetPos.y() - popupSize.height();
        break;
    case TopCenter:
        x = targetPos.x() + (targetSize.width() - popupSize.width()) / 2;
        y = targetPos.y() - popupSize.height();
        break;
    case TopRight:
        x = targetPos.x() + targetSize.width() - popupSize.width();
        y = targetPos.y() - popupSize.height();
        break;
    case BottomLeft:
        x = targetPos.x();
        y = targetPos.y() + targetSize.height();
        break;
    case BottomCenter:
        x = targetPos.x() + (targetSize.width() - popupSize.width()) / 2;
        y = targetPos.y() + targetSize.height();
        break;
    case BottomRight:
        x = targetPos.x() + targetSize.width() - popupSize.width();
        y = targetPos.y() + targetSize.height();
        break;
    case Left:
        x = targetPos.x() - popupSize.width();
        y = targetPos.y() + (targetSize.height() - popupSize.height()) / 2;
        break;
    case Right:
        x = targetPos.x() + targetSize.width();
        y = targetPos.y() + (targetSize.height() - popupSize.height()) / 2;
        break;
    }
    //qDebug()<<"pos:"<<x<<y<<popupSize<<targetPos<<target->pos();
    return QPoint(x, y);
}

void wPopupPanel::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (d->targetWidget && isVisible()) {

        QPoint pos = calcPosition(d->targetWidget, d->targetDirection);
        QScreen *screen = QGuiApplication::screenAt(d->targetWidget->mapToGlobal(d->targetWidget->rect().center()));
        if (screen) {
            QRect screenGeo = screen->availableGeometry();
            pos.setX(qBound(screenGeo.left(), pos.x(), screenGeo.right() - width()));
            pos.setY(qBound(screenGeo.top(), pos.y(), screenGeo.bottom() - height()));
        }
        move(pos);
    }
}

void wPopupPanel::focusOutEvent(QFocusEvent *event)
{
    QWidget::focusOutEvent(event);
    if (d->autoClose) {
        hide();
        emit hidden();
    }
}

void wPopupPanel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.addRoundedRect(rect(), d->borderRadius, d->borderRadius);

    painter.setClipPath(path);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(event->rect(), d->bgColor);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setPen(QPen(QColor(255, 255, 255, 30), 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect().adjusted(0, 0, -1, -1), d->borderRadius, d->borderRadius);

    QWidget::paintEvent(event);
}
