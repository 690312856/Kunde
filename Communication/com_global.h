#ifndef COMMUNICATION_GLOBAL_H
#define COMMUNICATION_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMMUNICATION_LIBRARY)
#  define COM_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define COM_SHARED_EXPORT Q_DECL_IMPORT
#endif

#define str(a) QString::fromLocal8Bit(a)

#endif // COMMUNICATION_GLOBAL_H
