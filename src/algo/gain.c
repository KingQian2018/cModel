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

static CMODEL_STATUS_e _run(CModel cm, uint32_t dt)
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

CModel gain_create(uint32_t id, uint32_t dt)
{
    uint8_t num[4] = {1, 0, 1, 0};
    CModel cm = NULL;
    cm_create(&cm, name, id, dt, num);
    if (cm == NULL)
    {
        LOG_E("%s %d Create Error.", name, id);
        return cm;
    }
    cm->type = CMODEL_GAIN;
    cm->par = (GainPar_s *)calloc(1, sizeof(GainPar_s));
    if (cm->par == NULL)
    {
        LOG_E("%s %d Create Par Error.", name, id);
        cm_deleate(&cm);
        return cm;
    }

    GainPar_s *par = (GainPar_s *)cm->par;

    par->gain = 1.0f;
    cm->deleateByCM = cm_commonDeleatePar;
    cm->run = _run;
    return cm;
}

CMODEL_STATUS_e gain_setPar(CModel cm, a_value gain)
{
    IS_VALID_TYPE(cm, CMODEL_GAIN);
    GainPar_s *par = (GainPar_s *)cm->par;
    par->gain = gain;
    return CMODEL_STATUS_OK;
}

#endif
