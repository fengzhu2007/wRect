#ifndef W_QSS_H
#define W_QSS_H
#include <QString>
#include "global.h"



class WRECT_EXPORT wQSS {
public:
    enum Theme{
        Light,
        Dark
    };
    static QString  global();
    static void init(Theme theme);
    static void destory();
    static int theme();

private:
    wQSS(Theme theme);

private:
    const Theme m_theme;
    static wQSS* instance;

};


#endif // W_QSS_H
