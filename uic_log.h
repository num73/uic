#ifndef _UIC_LOG_H
#define _UIC_LOG_H

#include <stdio.h>

#define UIC_DEBUG_OPEN 1


#define UIC_DEBUG "[uic debug]"
#define UIC_INFO "[uic info]"
#define UIC_WARN "[uic warn]"
#define UIC_ERROR "[uic error]"


#if UIC_DEBUG_OPEN
#define uic_dbg(fmt, ...) printf(UIC_DEBUG " " fmt, ##__VA_ARGS__)
#else
#define uic_dbg(fmt, ...)
#endif


#define uic_info(fmt, ...) printf(UIC_INFO " " fmt, ##__VA_ARGS__)
#define uic_warn(fmt, ...) printf(UIC_WARN " " fmt, ##__VA_ARGS__)
#define uic_error(fmt, ...) printf(UIC_ERROR " " fmt, ##__VA_ARGS__)

#endif //_UIC_LOG_H
