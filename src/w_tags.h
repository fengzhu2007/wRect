#ifndef WTAGS_H
#define WTAGS_H
#include "global.h"
#include <QLabel>

class wTagsPrivate;
class WRECT_EXPORT wTags : public QWidget
{
    Q_OBJECT
public:
    explicit wTags(QWidget *parent = nullptr);
    ~wTags();
    void setTags(QStringList list);
    void addTags(QStringList list);
    void addTag(const QString& text);
protected:
    virtual void paintEvent(QPaintEvent *e) override;
private:

    QLabel* newTag(int i);
private:
    wTagsPrivate *d;


signals:

};

#endif // WTAGS_H
