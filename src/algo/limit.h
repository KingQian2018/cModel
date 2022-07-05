#ifndef __LIMIT_H
#define __LIMIT_H

#include "cm.h"

#if CM_LIMIT

#ifdef __cplusplus
extern "C"
{
#endif

    CModel limit_create(uint32_t id, uint32_t dt);
    CMODEL_STATUS_e limit_setPar(CModel cm, a_value h, a_value l, a_value s);

#ifdef __cplusplus
}
#endif

#endif

#endif
