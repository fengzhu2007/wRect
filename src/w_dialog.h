#ifndef WDIALOG_H
#define WDIALOG_H
#include "global.h"
#include <QDialog>


#ifdef Q_OS_MAC

class WRECT_EXPORT wDialog : public QDialog
{
public:
    explicit wDialog(QWidget* parent);
    ~wDialog();
    void resetupUi();
    void hideNClient();
    void showNClient();

};



#else
class wWindowNClient;
class wDialogPrivate;
class WRECT_EXPORT wDialog : public QDialog
{
public:
    explicit wDialog(QWidget* parent);
    ~wDialog();
    void resetupUi();
    void updateResizer();
    void updateNClient();
    void setWindowTitle(const QString& title);
    void setStyleSheet(const QString& qss);
    void setWindowIcon(const QString& pathname);
    void hideNClient();
    void showNClient();
    wWindowNClient* nClient();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void showEvent(QShowEvent* event) override;

private:
    void initResizer();
    void initNClient();
    void initBackground();
private:
    wDialogPrivate *d;
};

#endif

#endif // WDIALOG_H
