#ifndef __CM_H
#define __CM_H

#include "cmodel.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include "io_port.h"
#include <stdint.h>
#include <stdlib.h>

#define CMODEL_ERROR \
    T(OK)            \
    T(CM_NULL)       \
    T(CM_NOTNULL)    \
    T(CM_CREATE)     \
    T(CM_FINDNONE)   \
    T(CM_TYPEERR)    \
    T(CM_CREATEPAR)  \
    T(CM_IOINVALID)  \
    T(IO_ERR)

#define T(typ) CMODEL_STATUS_##typ,
    typedef enum
    {
        CMODEL_ERROR
    } CMODEL_STATUS_e;
#undef T

#define CMODEL_TYPE \
    T(NONE)         \
    T(CONST)        \
    T(TRANSLATE)    \
    T(LIMIT)        \
    T(PID)
#define T(typ) CMODEL_##typ,
    typedef enum
    {
        CMODEL_TYPE
    } CMODEL_TYPE_e;
#undef T

#define IS_VALID_TYPE(cm, typ)           \
    if (cm == NULL || cm->type != typ)   \
    {                                    \
        LOG_E("TYPEERR.");               \
        return CMODEL_STATUS_CM_TYPEERR; \
    }

    typedef void *Par_t;
    typedef void *Fun_t;
    typedef void *UserData_t;
    typedef struct _CModel_s CModel_s, *CModel;
    typedef uint32_t (*Init_CB)(CModel cm);
    typedef uint32_t (*Run_CB)(CModel cm, uint32_t dt);
    typedef uint32_t (*DeleateByCM_CB)(CModel cm);
    typedef uint32_t (*User_CB)(void *);

    struct _CModel_s
    {
        CModel pre;
        CModel next;
        const char *name;           // 模块名称
        uint32_t id;                // 唯一标识符
        IO io;                      // io 引脚
        CMODEL_TYPE_e type;         // 模块类型
        uint32_t dt;                // 运行时间间隔
        Init_CB init;               // 初始化
        Run_CB run;                 // 运行
        DeleateByCM_CB deleateByCM; // 删除
        Par_t par;                  // 模块参数
        Fun_t fun;                  // 模块方法
        UserData_t user_data;       // 用户数据
        User_CB user_cb;            // 用户回调
    };

    uint32_t cm_create(CModel *cm, const char *name, uint32_t id, uint32_t dt, uint8_t num[4]);
    uint32_t cm_setLink(IOTYP_e type, CModel cmSrc, IOPIN_e pinSrc, CModel cmDst, IOPIN_e pinDst);
    uint32_t cm_deleate(CModel *cm);
    uint32_t cm_run(unsigned int dt);
    uint32_t cm_showAll(CModel cm);
    uint32_t cm_showPin(CModel cm, IOTYP_e type, IOPIN_e pin);
    a_value cm_getAPin(CModel cm, IOPIN_e pin, IOTYP_e type);
    d_value cm_getDPin(CModel cm, IOPIN_e pin, IOTYP_e type);

#ifdef __cplusplus
}
#endif

#endif
