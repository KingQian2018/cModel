#include "switch.h"

#if CM_SWITCH

#if CM_LOG_SWITCH
#define LOG_TAG "SWITCH"
#endif
#include "cm_log.h"

const static char *name = "SWITCH";

typedef struct _SwitchPar_s
{
    uint32_t triggleT; // 跳变时间
    uint32_t lstT;     // 上次时间
    d_value lstU;      // 设定值
    SwitchType_e type; // 按键类型
    bool toggleFlg;    // 翻转标志
} SwitchPar_s;

static CMODEL_STATUS_e _del(CModel cm)
{
    IS_VALID_TYPE(cm, CMODEL_SWITCH);
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

    SwitchPar_s *par = (SwitchPar_s *)cm->par;

    par->lstT += dt;
    if (par->lstT >= par->triggleT)
    {
        if (par->toggleFlg == false && par->type == SWITCH_TYPE_TOOGLE)
        {
            par->lstU = !par->lstU;
            par->toggleFlg = true;
        }
        if (par->type == SWITCH_TYPE_L2H)
        {
            par->lstU = 1;
        }
        else if (par->type == SWITCH_TYPE_H2L)
        {
            par->lstU = 0;
        }
    }
    else
    {
        if (par->type == SWITCH_TYPE_L2H)
        {
            par->lstU = 0;
        }
        else if (par->type == SWITCH_TYPE_H2L)
        {
            par->lstU = 1;
        }
    }

    IO_SetDOValue(cm->io, IOPIN_1, par->lstU);
    return CMODEL_STATUS_OK;
}

CModel switch_create(uint32_t id, uint32_t dt)
{
    CModel cm = NULL;
    uint8_t num[4] = {0, 0, 0, 1};
    cm_create(&cm, name, id, dt, num);
    if (cm == NULL)
    {
        LOG_E("%s %d Create Error.", name, id);
        return cm;
    }
    cm->type = CMODEL_SWITCH;
    cm->par = (SwitchPar_s *)calloc(1, sizeof(SwitchPar_s));
    if (cm->par == NULL)
    {
        LOG_E("%s %d Create Par Error.", name, id);
        cm_deleate(&cm);
        return cm;
    }

    SwitchPar_s *par = (SwitchPar_s *)cm->par;

    par->lstT = par->triggleT = 0;
    par->type = SWITCH_TYPE_L2H;
    if (par->type == SWITCH_TYPE_H2L)
    {
        par->lstU = 1;
    }

    cm->deleateByCM = cm_commonDeleatePar;
    cm->run = _run;
    return cm;
}

CMODEL_STATUS_e switch_setPar(CModel cm, SwitchType_e type, uint32_t triggleT)
{
    IS_VALID_TYPE(cm, CMODEL_SWITCH);
    SwitchPar_s *par = (SwitchPar_s *)cm->par;
    par->type = type;
    par->lstT = 0;
    par->triggleT = triggleT;
    if (par->type == SWITCH_TYPE_L2H)
    {
        par->lstU = 0;
    }
    else if (par->type == SWITCH_TYPE_H2L)
    {
        par->lstU = 1;
    }

    return CMODEL_STATUS_OK;
}

CMODEL_STATUS_e switch_toggle(CModel cm, uint32_t triggleT)
{
    IS_VALID_TYPE(cm, CMODEL_SWITCH);
    SwitchPar_s *par = (SwitchPar_s *)cm->par;

    par->type = SWITCH_TYPE_TOOGLE;
    par->lstT = 0;
    par->toggleFlg = false;
    par->triggleT = triggleT;
    return CMODEL_STATUS_OK;
}

#endif
