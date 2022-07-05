#include "limit.h"

#if CM_LIMIT

#if CM_LOG_LIMIT
#define LOG_TAG "LIMIT"
#endif
#include "cm_log.h"

const static char *name = "LIMIT";

typedef struct _LimitPar_s
{
    a_value highOut, lowOut;
    a_value speed;
    a_value lstU;
} LimitPar_s;

static CMODEL_STATUS_e _del(CModel cm)
{
    IS_VALID_TYPE(cm, CMODEL_LIMIT);
    free(cm->par);
    return CMODEL_STATUS_OK;
}

static CMODEL_STATUS_e _run(CModel cm, uint32_t dt)
{
    if (cm == NULL)
    {
        LOG_E("%s run model is null.", name);
        return CMODEL_STATUS_CM_NULL;
    }

    LimitPar_s *par = (LimitPar_s *)cm->par;

    a_value ain_1 = IO_GetAValue(cm->io, IOPIN_1, IOTYP_AI);
    d_value din_1 = IO_GetDValue(cm->io, IOPIN_1, IOTYP_DI);
    d_value din_2 = IO_GetDValue(cm->io, IOPIN_2, IOTYP_DI);
    a_value target = (!din_1)               ? ain_1
                     : ain_1 > par->highOut ? par->highOut
                     : ain_1 < par->lowOut  ? par->lowOut
                                            : ain_1;
    if (din_2)
    {
        a_value dlt = (target - par->lstU);
        a_value speed = par->speed / 1000.0f * dt;
        par->lstU = speed < dlt    ? par->lstU + speed
                    : -speed > dlt ? par->lstU - speed
                                   : par->lstU + dlt;
    }
    else
    {
        par->lstU = target;
    }
    IO_SetAOValue(cm->io, IOPIN_1, par->lstU);
    return CMODEL_STATUS_OK;
}

CModel limit_create(uint32_t id, uint32_t dt)
{
    uint8_t num[4] = {1, 2, 1, 0};
    CModel cm = NULL;
    cm_create(&cm, name, id, dt, num);
    if (cm == NULL)
    {
        LOG_E("%s %d Create Error.", name, id);
        return cm;
    }
    cm->type = CMODEL_LIMIT;
    cm->par = (LimitPar_s *)calloc(1, sizeof(LimitPar_s));
    if (cm->par == NULL)
    {
        LOG_E("%s %d Create Par Error.", name, id);
        cm_deleate(&cm);
        return cm;
    }

    LimitPar_s *par = (LimitPar_s *)cm->par;

    par->highOut = 0;
    par->lowOut = 0;
    par->speed = 0;
    par->lstU = 0;

    cm->deleateByCM = cm_commonDeleatePar;
    cm->run = _run;
    return cm;
}

CMODEL_STATUS_e limit_setPar(CModel cm, a_value h, a_value l, a_value s)
{
    IS_VALID_TYPE(cm, CMODEL_LIMIT);
    LimitPar_s *par = (LimitPar_s *)cm->par;
    par->highOut = h;
    par->lowOut = l;
    par->speed = s;
    return CMODEL_STATUS_OK;
}

#endif
