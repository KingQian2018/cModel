#ifndef __GAIN_H
#define __GAIN_H

#include "cm.h"

#if CM_GAIN

#ifdef __cplusplus
extern "C"
{
#endif

    uint32_t gain_create(CModel *cm, uint32_t id, uint32_t dt);
    uint32_t gain_setPar(CModel cm, a_value gain);

#ifdef __cplusplus
}
#endif

#endif

#endif
