#ifndef __LIMIT_H
#define __LIMIT_H

#include "cm.h"

#if CM_LIMIT

#ifdef __cplusplus
extern "C"
{
#endif

    uint32_t limit_create(CModel *cm, uint32_t id, uint32_t dt);
    uint32_t limit_setPar(CModel cm, a_value h, a_value l, a_value s);

#ifdef __cplusplus
}
#endif

#endif

#endif
