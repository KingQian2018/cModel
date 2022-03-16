#ifndef __PID_H
#define __PID_H

#include "cm.h"

#if CM_PID

#ifdef __cplusplus
extern "C"
{
#endif

    uint32_t pid_create(CModel *cm, uint32_t id, uint32_t dt);
    uint32_t pid_setLink(CModel cm, a_value *pPV, a_value *pSP, a_value *pFF, a_value *pTR, d_value *pSTR);
    uint32_t pid_setPID(CModel cm, float p, float i, float d);
    uint32_t pid_setSP(CModel cm, float basis, float gain);
    uint32_t pid_setPV(CModel cm, float basis, float gain);
    uint32_t pid_setLimit(CModel cm, float h, float l);
    uint32_t pid_setDt(CModel cm, uint32_t dt);

#ifdef __cplusplus
}
#endif

#endif
#endif
