#ifndef __SWITCH_H
#define __SWITCH_H

#include "cm.h"

#if CM_SWITCH

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum _SwitchType_e
    {
        SWITCH_TYPE_L2H,
        SWITCH_TYPE_H2L,
        SWITCH_TYPE_TOOGLE,
    } SwitchType_e;

    CModel switch_create(uint32_t id, uint32_t dt);
    CMODEL_STATUS_e switch_setPar(CModel cm, SwitchType_e type, uint32_t triggleT);
    CMODEL_STATUS_e switch_toggle(CModel cm, uint32_t triggleT);

#ifdef __cplusplus
}
#endif

#endif

#endif
