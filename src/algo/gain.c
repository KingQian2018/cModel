#include "gain.h"

#if CM_GAIN

#if CM_LOG_GAIN
#define LOG_TAG "GAIN"
#endif
#include "cm_log.h"

const static char *name = "GAIN";

typedef struct _GainPar_s
{
    a_value gain;
} GainPar_s;

static uint32_t _run(CModel cm, uint32_t dt)
{
    if (cm == NULL)
    {
        LOG_E("%s run model is null.", name);
        return CMODEL_STATUS_CM_NULL;
    }

    GainPar_s *par = (GainPar_s *)cm->par;

    a_value ain1 = IO_GetAValue(cm->io, IOPIN_1, IOTYP_AI);
    a_value aout1 = ain1 * par->gain;
    IO_SetAOValue(cm->io, IOPIN_1, aout1);

    return CMODEL_STATUS_OK;
}

uint32_t gain_create(CModel *cm, uint32_t id, uint32_t dt)
{
    uint8_t num[4] = {1, 0, 1, 0};
    cm_create(cm, name, id, dt, num);
    if (cm[0] == NULL)
    {
        LOG_E("%s %d Create Error.", name, id);
        return CMODEL_STATUS_CM_CREATE;
    }
    cm[0]->type = CMODEL_GAIN;
    cm[0]->par = (GainPar_s *)calloc(1, sizeof(GainPar_s));
    if (cm[0]->par == NULL)
    {
        LOG_E("%s %d Create Par Error.", name, id);
        return CMODEL_STATUS_CM_CREATEPAR;
    }

    GainPar_s *par = (GainPar_s *)cm[0]->par;

    par->gain = 1.0f;
    cm[0]->deleateByCM = cm_commonDeleatePar;
    cm[0]->run = _run;
    return CMODEL_STATUS_OK;
}

uint32_t gain_setPar(CModel cm, a_value gain)
{
    IS_VALID_TYPE(cm, CMODEL_GAIN);
    GainPar_s *par = (GainPar_s *)cm->par;
    par->gain = gain;
    return CMODEL_STATUS_OK;
}

#endif
