#ifndef CPWEBP_GLOBAL_H
#define CPWEBP_GLOBAL_H



#include <QtCore/qglobal.h>

#if CP_WEBP_LIBRARY==1
#  define CP_WEBP_EXPORT Q_DECL_EXPORT
#else
#  define CP_WEBP_EXPORT
#endif

#endif // CPWEBP_GLOBAL_H
