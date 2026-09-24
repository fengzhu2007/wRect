#ifndef W_POPUP_PANEL_H
#define W_POPUP_PANEL_H

#include "global.h"
#include <QWidget>
#include <QResizeEvent>

class wPopupPanelPrivate;

/**
 * wPopupPanel - 通用弹窗面板
 *
 * 支持8个方向相对于目标控件定位，自动根据内容调整大小。
 * 可设置圆角、内边距、背景色等样式属性。
 */
class WRECT_EXPORT wPopupPanel : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 弹窗相对于目标控件的弹出方向
     */
    enum Direction {
        TopLeft,        ///< 目标左上方
        TopCenter,      ///< 目标正上方
        TopRight,       ///< 目标右上方
        BottomLeft,     ///< 目标左下方
        BottomCenter,   ///< 目标正下方
        BottomRight,    ///< 目标右下方
        Left,           ///< 目标左侧
        Right           ///< 目标右侧
    };
    Q_ENUM(Direction)

    explicit wPopupPanel(QWidget *parent = nullptr);
    ~wPopupPanel();

    /// 设置内容控件（成为弹窗的唯一子控件）
    void setContentWidget(QWidget *widget);
    QWidget *contentWidget() const;

    /// 是否在点击外部区域时自动关闭，默认 true
    void setAutoClose(bool enable);
    bool autoClose() const;

    /// 弹窗圆角半径，默认 6
    void setBorderRadius(int radius);
    int borderRadius() const;

    /// 内容区域内边距，默认 8
    void setContentPadding(int padding);
    int contentPadding() const;

    /// 背景色，默认半透明黑色
    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;

    /// 背景透明度 (0-255)，修改当前背景色的 alpha 通道
    void setBackgroundOpacity(int opacity);

    /// 在目标控件的指定方向弹出显示
    void showPopup(QWidget *target, Direction direction = BottomCenter);

    /// 在全局坐标位置弹出显示
    void showPopup(const QPoint &globalPos);

signals:
    void shown();
    void hidden();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void autoSize();
    QPoint calcPosition(QWidget *target, Direction direction);

    wPopupPanelPrivate *d;
};

#endif // W_POPUP_PANEL_H
