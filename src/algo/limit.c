#include "limit.h"

#if CM_LIMIT

#if CM_LOG_LIMIT
#define LOG_TAG "LIMIT"
#define T(t) #t,
static const char *_loginfo[] = {CMODEL_ERROR};
#undef T
#else
static const char *_loginfo[] = {NULL};
#endif
#include "cm_log.h"

const static char *name = "LIMIT";

typedef struct _LimitPar_s
{
    a_value highOut, lowOut;
    a_value speed;
    a_value lstU;
} LimitPar_s;

static uint32_t _del(CModel cm)
{
    IS_VALID_TYPE(cm, CMODEL_LIMIT);
    free(cm->par);
    return CMODEL_STATUS_OK;
}

static uint32_t _run(CModel cm, uint32_t dt)
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
    if (din_1)
    {
        par->lstU = par->lstU > par->highOut  ? par->highOut
                    : par->lstU < par->lowOut ? par->lowOut
                                              : par->lstU;
    }
    if (din_2)
    {
        a_value dlt = (ain_1 - par->lstU);
        a_value speed = par->speed / 1000.0f * dt;
        par->lstU = speed < dlt    ? par->lstU + speed
                    : -speed > dlt ? par->lstU - speed
                                   : par->lstU + dlt;
    }
    if (!din_2 && !din_1)
    {
        par->lstU = ain_1;
    }
    IO_SetAOValue(cm->io, IOPIN_1, par->lstU);
    return CMODEL_STATUS_OK;
}

uint32_t limit_create(CModel *cm, uint32_t id, uint32_t dt)
{
    uint8_t num[4] = {1, 2, 1, 0};
    cm_create(cm, name, id, dt, num);
    if (cm[0] == NULL)
    {
        LOG_E("%s %d Create Error.", name, id);
        return CMODEL_STATUS_CM_CREATE;
    }
    cm[0]->type = CMODEL_LIMIT;
    cm[0]->par = (LimitPar_s *)calloc(1, sizeof(LimitPar_s));
    if (cm[0]->par == NULL)
    {
        LOG_E("%s %d Create Par Error.", name, id);
        return CMODEL_STATUS_CM_CREATEPAR;
    }

    LimitPar_s *par = (LimitPar_s *)cm[0]->par;

    par->highOut = 0;
    par->lowOut = 0;
    par->speed = 0;
    par->lstU = 0;

    cm[0]->deleateByCM = _del;
    cm[0]->run = _run;
    return CMODEL_STATUS_OK;
}

uint32_t limit_setPar(CModel cm, a_value h, a_value l, a_value s)
{
    IS_VALID_TYPE(cm, CMODEL_LIMIT);
    LimitPar_s *par = (LimitPar_s *)cm->par;
    par->highOut = h;
    par->lowOut = l;
    par->speed = s;
    return CMODEL_STATUS_OK;
}

#endif
