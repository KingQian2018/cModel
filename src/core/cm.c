/**
 * @file cm.c
 * @author KisWang (KingQian2018)
 * @brief 模块核心代码
 * @version 0.1
 * @date 2022-07-03
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "cm.h"

#if CM_LOG_CM
#define LOG_TAG "CModel"
#endif
#include "cm_log.h"

static CModel _register; // 根模块
static const char *_name = "CMODEL";

/**
 * @brief 动态创建模块
 *
 * @param cm 模块指针
 * @param name 模块名
 * @param id 模块ID
 * @param dt 模块运行时间间隔
 * @param num 模块引脚个数
 * @return CMODEL_STATUS_CM_NOTNULL cm模块非空
 * @return CMODEL_STATUS_CM_CREATE cm模块创建失败
 * @return CMODEL_STATUS_IO_ERR cm模块引脚创建失败
 * @return CMODEL_STATUS_OK cm模块创建成功
 */
CMODEL_STATUS_e cm_create(CModel *cm, const char *name, CMODEL_STATUS_e id, CMODEL_STATUS_e dt, uint8_t num[4])
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
    if (IO_Create(&(cm[0]->io), num) != CMODEL_STATUS_OK)
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

/**
 * @brief 删除指定模块
 *
 * @param cm 删除模块
 * @return CMODEL_STATUS_OK 删除成功
 */
CMODEL_STATUS_e cm_deleate(CModel *cm)
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

/**
 * @brief 删除模块的参数配置
 *
 * @param cm 当前模块的参数配置
 * @return CMODEL_STATUS_e
 */
CMODEL_STATUS_e cm_commonDeleatePar(CModel cm)
{
    if (cm->par != NULL)
    {
        free(cm->par);
        cm->par = NULL;
    }
    return CMODEL_STATUS_OK;
}

/**
 * @brief 遍历所有模块运行
 *
 * @param dt 运行时间间隔
 * @return CMODEL_STATUS_OK 运行成功
 */
CMODEL_STATUS_e cm_run(unsigned int dt)
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

/**
 * @brief 模块建立链接
 *
 * @param type 链接类型
 * @param cmSrc 源模块
 * @param pinSrc 源模块引脚
 * @param cmDst 目标模块
 * @param pinDst 目标模块引脚
 * @return CMODEL_STATUS_OK 建立成功
 * @return CMODEL_STATUS_IO_ERR 建立失败
 */
CMODEL_STATUS_e cm_setLink(IOTYP_e type, CModel cmSrc, IOPIN_e pinSrc, CModel cmDst, IOPIN_e pinDst)
{
    if (type == IOTYP_AI)
    {
        return IO_setLink(cmSrc->io, type, pinSrc, IO_GetAOPoint(cmDst->io, pinDst));
    }
    else
    {
        return IO_setLink(cmSrc->io, type, pinSrc, IO_GetDOPoint(cmDst->io, pinDst));
    }
}

#include <stdio.h>
/**
 * @brief 显示指定模块数据
 *
 * @param cm 模块
 * @return CMODEL_STATUS_CM_NULL 模块空
 * @return CMODEL_STATUS_OK 成功
 */
CMODEL_STATUS_e cm_showAll(CModel cm)
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

/**
 * @brief 显示指定模块引脚
 *
 * @param cm 模块
 * @param type 引脚类型
 * @param pin 引脚索引
 * @return CMODEL_STATUS_CM_NULL 模块空
 * @return CMODEL_STATUS_OK 成功
 */
CMODEL_STATUS_e cm_showPin(CModel cm, IOTYP_e type, IOPIN_e pin)
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

/**
 * @brief 获取指定模块引脚模拟量数据
 *
 * @param cm 模块
 * @param pin 引脚索引
 * @param type 输出/输入
 * @return a_value
 */
a_value cm_getAPin(CModel cm, IOPIN_e pin, IOTYP_e type)
{
    if (cm == NULL)
    {
        LOG_E("%s-%s: CM_NULL.", __FILE__, __LINE__);
        return 0;
    }
    return IO_GetAValue(cm->io, pin, type);
}

/**
 * @brief 获取指定模块引脚数字量数据
 *
 * @param cm 模块
 * @param pin 引脚索引
 * @param type 输出/输入
 * @return d_value
 */
d_value cm_getDPin(CModel cm, IOPIN_e pin, IOTYP_e type)
{
    if (cm == NULL)
    {
        LOG_E("CM_NULL.");
        return 0;
    }
    return IO_GetDValue(cm->io, pin, type);
}
