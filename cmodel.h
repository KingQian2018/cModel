#ifndef __CMODEL_H
#define __CMODEL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "cmodel_conf.h"

    typedef float a_value;
    typedef unsigned char d_value;

#define CMODEL_ERROR  \
    T(OK)             \
    T(CM_NULL)        \
    T(CM_NOTNULL)     \
    T(CM_CREATE)      \
    T(CM_FINDNONE)    \
    T(CM_TYPEERR)     \
    T(CM_CREATEPAR)   \
    T(CM_IOINVALID)   \
    T(IO_NONE)        \
    T(IO_DONE)        \
    T(IO_ERR)         \
    T(IO_FILE)        \
    T(IO_RESE)        \
    T(PS_FILEINVALID) \
    T(PS_FAILFILE)    \
    T(PS_FAILPARSE)

#define T(typ) CMODEL_STATUS_##typ,
    typedef enum
    {
        CMODEL_ERROR
    } CMODEL_STATUS_e;
#undef T

    extern const char *_loginfo[];
    
#ifdef __cplusplus
}
#endif

#endif
