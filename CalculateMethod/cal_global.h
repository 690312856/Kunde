#ifndef CAL_GLOBAL_H
#define CAL_GLOBAL_H

#if defined(CAL_LIBRARY)
#  define CAL_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define CAL_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CAL_GLOBAL_H

#include <QObject>
#define str(a) QString::fromLocal8Bit(a)
