
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
    while (tmp[0]->next != NULL)
    {
        if (tmp[0]->run != 0)
        {
            tmp[0]->run(tmp[0], dt);
        }
        tmp = &(tmp[0]->next);
    }
    return CMODEL_STATUS_OK;
}
