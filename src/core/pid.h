#ifndef __PID_H
#define __PID_H
#include "cm.h"

#ifdef __cplusplus
extern "C"
{
#endif

    uint32_t pid_create(CModel *cm, uint32_t id, uint32_t dt);
    uint32_t pid_setLink(CModel cm, a_value *pPV, a_value *pSP, a_value *pFF, a_value *pTR, d_value *pSTR);
    a_value PID_AOut(CModel cm);
    a_value *PID_AOutPoint(CModel cm);

#ifdef __cplusplus
}
#endif

#endif
