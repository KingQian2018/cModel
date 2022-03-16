
#include "cm.h"

#if CM_LOG_CM
#define T(t) #t,
#define LOG_TAG "CModel"
static const char *_loginfo[] = {CMODEL_ERROR};
#undef T
#include "cm_log.h"
#else
static const char *_loginfo[] = NULL;
#endif

static CModel _register;
static const char *_name = "CMODEL";
uint32_t cm_create(CModel *cm, const char *name, uint32_t id, uint32_t dt, uint8_t num[4])
{
    if (cm[0] != NULL)
    {
        LOG_E("%s was created.", cm[0]->name);
        return CMODEL_STATUS_CM_NOTNULL;
    }
    cm[0] = (CModel)calloc(1, sizeof(CModel_s));
    if (cm[0] == NULL)
    {
        LOG_E("%s created error.", name);
        return CMODEL_STATUS_CM_CREATE;
    }
    cm[0]->name = name == NULL ? _name : name;
    cm[0]->id = id;
    cm[0]->dt = dt;
    if (IO_Create(&(cm[0]->io), num) != IOSTUS_OK)
    {
        LOG_E("%s create IO error.", name);
        return CMODEL_STATUS_IO_ERR;
    }

    CModel *tmp = &_register;
    if (tmp[0] == NULL)
    {
        tmp[0] = cm[0];
        return CMODEL_STATUS_OK;
    }
    while (tmp[0]->next != NULL)
    {
        tmp = &(tmp[0]->next);
    }
    tmp[0]->next = cm[0];
    cm[0]->pre = tmp[0];
    return CMODEL_STATUS_OK;
}

uint32_t cm_deleate(CModel *cm)
{
    CModel pre = cm[0]->pre;
    CModel next = cm[0]->next;

    if (pre != NULL && next != NULL)
    {
        pre->next = next;
        next->pre = pre;
    }
    else if (pre == NULL && next != NULL)
    {
        _register = next;
        next->pre = NULL;
    }
    else if (pre != NULL && next == NULL)
    {
        pre->next = NULL;
    }

    IO_Deleate(cm[0]->io);
    if (cm[0]->deleateByCM != NULL)
    {
        cm[0]->deleateByCM(cm[0]);
    }
    free(cm[0]);
    cm[0] = NULL;
    return CMODEL_STATUS_OK;
}

uint32_t cm_run(unsigned int dt)
{
    CModel *tmp = &_register;
    if (tmp[0] == NULL)
    {
        return CMODEL_STATUS_OK;
    }
    while (tmp[0] != NULL)
    {
        if (tmp[0]->run != 0)
        {
            tmp[0]->run(tmp[0], dt);
        }
        tmp = &(tmp[0]->next);
    }
    return CMODEL_STATUS_OK;
}

uint32_t cm_setLink(IOTYP_e type, CModel cmSrc, IOPIN_e pinSrc, CModel cmDst, IOPIN_e pinDst)
{
    IO_setLink(cmSrc->io, type, pinSrc, IO_GetAOPoint(cmDst->io, pinDst));
    return CMODEL_STATUS_OK;
}

#include <stdio.h>
uint32_t cm_showAll(CModel cm)
{
    if (cm == NULL)
    {
        LOG_E("CM_NULL.");
        return CMODEL_STATUS_CM_NULL;
    }
    printf("\n%d\t%s", cm->id, cm->name);
    IO_ShowALL(cm->io);
    return CMODEL_STATUS_OK;
}

uint32_t cm_showPin(CModel cm, IOTYP_e type, IOPIN_e pin)
{
    if (cm == NULL)
    {
        LOG_E("CM_NULL.");
        return CMODEL_STATUS_CM_NULL;
    }
    printf("\n%d\t%s", cm->id, cm->name);
    IO_ShowPin(cm->io, type, pin);
    return CMODEL_STATUS_OK;
}

a_value cm_getAPin(CModel cm, IOPIN_e pin, IOTYP_e type)
{
    if (cm == NULL)
    {
        LOG_E("CM_NULL.");
        return 0;
    }
    return IO_GetAValue(cm->io, pin, type);
}

d_value cm_getDPin(CModel cm, IOPIN_e pin, IOTYP_e type)
{
    if (cm == NULL)
    {
        LOG_E("CM_NULL.");
        return 0;
    }
    return IO_GetDValue(cm->io, pin, type);
}
