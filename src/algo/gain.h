#ifndef __GAIN_H
#define __GAIN_H

#include "cm.h"

#if CM_GAIN

#ifdef __cplusplus
extern "C"
{
#endif

    CModel gain_create(uint32_t id, uint32_t dt);
    CMODEL_STATUS_e gain_setPar(CModel cm, a_value gain);

#ifdef __cplusplus
}
#endif

#endif

#endif
