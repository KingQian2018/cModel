#include "pid.h"

#if CM_LOG_PID
#define LOG_TAG "PID"
#define T(t) #t,
static const char *_loginfo[] = {CMODEL_ERROR};
#undef T
#include "cm_log.h"
#else
static const char *_loginfo[] = NULL;
#endif

const static char *name = "PID";

/// 宏定义，PID模块引脚类型
#define PID_AI_PV (IOPIN_1)
#define PID_AI_SP (IOPIN_2)
#define PID_AI_FF (IOPIN_3)
#define PID_AI_TR (IOPIN_4)

#define PID_DI_STR (IOPIN_1)

#define PID_AO_OUT (IOPIN_1)

/// 参数调整结构体
typedef struct
{
    float Basis; // 基准
    float Gain;  // 增益
} PIDAdj_t;

/// 模块状态结构体
typedef struct
{
    float Err[3]; // 偏差值。用于存放k, k-1, k-2时刻的历史偏差
    float Dt;     // 模块运行的基准时间间隔
    float LastI;  // 历史积分值
    float LastU;  // 历史输出值
    float LastKP; // 历史比例增益
} PIDSta_t;

/// 模块参数结构体
typedef struct
{
    float KP;     // 比例增益
    float KI;     // 积分增益
    float KD;     // 微分增益
    PIDAdj_t SP;  // 设定值
    PIDAdj_t PV;  // 过程量
    float HigOut; // 输出上限
    float LowOut; // 输出下限
    PIDSta_t sta; // 运行状态
} PIDPar_t;

static uint32_t pid_del(CModel cm)
{
    if (cm->type != CMODEL_PID)
    {
        LOG_E("Delete PID Model Error.");
        return CMODEL_STATUS_CM_TYPEERR;
    }
    free(cm->par);
    return CMODEL_STATUS_OK;
}

static uint32_t pid_run(CModel cm, uint32_t dt)
{
    if (cm == NULL)
    {
        LOG_E("pid run model is null.");
        return CMODEL_STATUS_CM_NULL;
    }

    PIDPar_t *par = (PIDPar_t *)cm->par;
    struct
    {
        float P;
        float I;
        float D;
        float U;
    } Out = {0.0f};
    a_value mySP = IO_GetAValue(cm->io, PID_AI_SP, IOTYP_AI);
    a_value myPV = IO_GetAValue(cm->io, PID_AI_PV, IOTYP_AI);
    a_value myFF = IO_GetAValue(cm->io, PID_AI_FF, IOTYP_AI);
    a_value myTR = IO_GetAValue(cm->io, PID_AI_TR, IOTYP_AI);
    d_value myStr = IO_GetDValue(cm->io, PID_DI_STR, IOTYP_DI);

    mySP = (mySP - par->SP.Basis) * par->SP.Gain;
    myPV = (myPV - par->PV.Basis) * par->PV.Gain;

    par->sta.Err[0] = mySP - myPV;
    if (myStr)
    {
        Out.U = myTR;
    }
    else
    {
        Out.P = par->KP * par->sta.Err[0];
        Out.I = par->KI * par->sta.Err[0] * par->sta.Dt + par->sta.LastI;
        Out.D = par->KD * (par->sta.Err[0] - par->sta.Err[1]) / par->sta.Dt;

        /// 积分输出
        Out.I = (Out.I > par->HigOut) ? par->HigOut : (Out.I < par->LowOut) ? par->LowOut
                                                                            : Out.I;
        Out.U = Out.P + Out.I + Out.D + myFF;
    }

    /// 限幅输出
    if (Out.U > par->HigOut)
    {
        IO_SetAOValue(cm->io, PID_AO_OUT, par->HigOut);
    }
    else if (Out.U < par->LowOut)
    {
        IO_SetAOValue(cm->io, PID_AO_OUT, par->LowOut);
    }
    else
    {
        IO_SetAOValue(cm->io, PID_AO_OUT, Out.U);
    }

    /// 更新历史状态
    par->sta.LastKP = par->KP;
    par->sta.Err[2] = par->sta.Err[1];
    par->sta.Err[1] = par->sta.Err[0];
    par->sta.LastI = Out.I;
    par->sta.LastU = IO_GetAValue(cm->io, PID_AO_OUT, IOTYP_AO);
}

uint32_t pid_setLink(CModel cm, a_value *pPV, a_value *pSP, a_value *pFF, a_value *pTR, d_value *pSTR)
{
    if (!IS_VALID_IO(cm->io))
    {
        LOG_E("%s.", _loginfo[CMODEL_STATUS_CM_IOINVALID]);
        return CMODEL_STATUS_CM_IOINVALID;
    }
    IO_setLink(cm->io, IOTYP_AI, IOPIN_1, pPV);
    IO_setLink(cm->io, IOTYP_AI, IOPIN_2, pSP);
    IO_setLink(cm->io, IOTYP_AI, IOPIN_3, pFF);
    IO_setLink(cm->io, IOTYP_AI, IOPIN_4, pTR);
    IO_setLink(cm->io, IOTYP_DI, IOPIN_1, pSTR);
    return CMODEL_STATUS_OK;
}

a_value PID_AOut(CModel cm)
{
    if (!IS_VALID_IO(cm->io))
    {
        return 0.0f;
    }
    return IO_GetAValue(cm->io, PID_AO_OUT, IOTYP_AO);
}

a_value *PID_AOutPoint(CModel cm)
{
    if (!IS_VALID_IO(cm->io))
    {
        return 0;
    }
    return IO_GetAOPoint(cm->io, PID_AO_OUT);
}

uint32_t pid_create(CModel *cm, uint32_t id, uint32_t dt)
{
    uint8_t num[4] = {4, 1, 1, 0};
    cm_create(cm, name, id, dt, num);
    if (cm[0] == NULL)
    {
        LOG_E("PID %d Create Error.", id);
        return CMODEL_STATUS_CM_CREATE;
    }
    cm[0]->type = CMODEL_PID;
    cm[0]->par = (PIDPar_t *)calloc(1, sizeof(PIDPar_t));
    if (cm[0]->par == NULL)
    {
        LOG_E("PID %d Create Par Error.", id);
        return CMODEL_STATUS_CM_CREATEPAR;
    }
    cm[0]->deleateByCM = pid_del;
    cm[0]->run = pid_run;
    return CMODEL_STATUS_OK;
}
