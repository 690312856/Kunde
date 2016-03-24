#ifndef UI_GLOBAL_H
#define UI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UI_LIBRARY)
#  define SHARED_EXPORT Q_DECL_EXPORT
#else
#  define SHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QtCore>
typedef QMap<QString, QString> QStringMap;

#define str(a) QString::fromLocal8Bit(a)

#endif // UI_GLOBAL_H
