#ifndef __PID_H
#define __PID_H

#include "cm.h"

#if CM_PID

#ifdef __cplusplus
extern "C"
{
#endif

    CModel pid_create(uint32_t id, uint32_t dt);
    CMODEL_STATUS_e pid_setLink(CModel cm, a_value *pPV, a_value *pSP, a_value *pFF, a_value *pTR, d_value *pSTR);
    CMODEL_STATUS_e pid_setPID(CModel cm, float p, float i, float d);
    CMODEL_STATUS_e pid_setSP(CModel cm, float basis, float gain);
    CMODEL_STATUS_e pid_setPV(CModel cm, float basis, float gain);
    CMODEL_STATUS_e pid_setLimit(CModel cm, float h, float l);
    CMODEL_STATUS_e pid_setDt(CModel cm, uint32_t dt);

#ifdef __cplusplus
}
#endif

#endif
#endif
