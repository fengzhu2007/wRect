#ifndef WRECT_GLOBAL_H
#define WRECT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(WRECT_LIBRARY)
#  define WRECT_EXPORT Q_DECL_EXPORT
#else
#  define WRECT_EXPORT Q_DECL_IMPORT
#endif

#endif // DOCKINGPANE_GLOBAL_H
