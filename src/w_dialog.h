#ifndef WDIALOG_H
#define WDIALOG_H
#include "global.h"
#include <QDialog>
class wDialogPrivate;
class WRECT_EXPORT wDialog : public QDialog
{
public:
    wDialog(QWidget* parent);
    ~wDialog();
    void resetupUi();
    void updateResizer();
    void updateNClient();
    void setWindowTitle(const QString& title);
    void setStyleSheet(const QString& qss);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;


private:
    void initResizer();
    void initNClient();
    void initBackground();
private:
    wDialogPrivate *d;
};

#endif // WDIALOG_H
