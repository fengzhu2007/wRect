#ifndef W_WINDOW_RESIZER_H
#define W_WINDOW_RESIZER_H
#include <QFrame>

    class wWindowResizerPrivate;
    class wWindowResizer : public QFrame{
        Q_OBJECT
    public:
        enum Region{
            Left=0,
            Top,
            Right,
            Bottom,
            LeftTop,
            LeftBottom,
            RightTop,
            RightBottom
        };
        wWindowResizer(QWidget* parent,Region region);
        ~wWindowResizer();

        void setRegion(Region region);
        Region region();

    protected:
        virtual void mousePressEvent(QMouseEvent *e) override;
        virtual void mouseMoveEvent(QMouseEvent *e) override;
        virtual void mouseReleaseEvent(QMouseEvent *e) override;

    private:
        wWindowResizerPrivate* d;


    };

#endif // DOCKING_PANE_WINDOW_RESIZER_H
