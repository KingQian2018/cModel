#include "pid.h"

#if CM_PID
#if CM_LOG_PID
#define LOG_TAG "PID"
#endif
#include "cm_log.h"

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
    a_value Err[3]; // 偏差值。用于存放k, k-1, k-2时刻的历史偏差
    a_value LastI;  // 历史积分值
    a_value LastU;  // 历史输出值
    a_value LastKP; // 历史比例增益
} PIDSta_t;

/// 模块参数结构体
typedef struct
{
    a_value KP;     // 比例增益
    a_value KI;     // 积分增益
    a_value KD;     // 微分增益
    PIDAdj_t SP;    // 设定值
    PIDAdj_t PV;    // 过程量
    a_value HigOut; // 输出上限
    a_value LowOut; // 输出下限
    PIDSta_t sta;   // 运行状态
} PIDPar_t;

static CMODEL_STATUS_e _run(CModel cm, uint32_t dt)
{
    IS_VALID_TYPE(cm, CMODEL_PID);

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
    float Dt = dt / 1000.0f;

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
        Out.I = par->KI * par->sta.Err[0] * Dt + par->sta.LastI;
        Out.D = par->KD * (par->sta.Err[0] - par->sta.Err[1]) / Dt;

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
    return CMODEL_STATUS_OK;
}

CModel pid_create(uint32_t id, uint32_t dt)
{
    CModel cm = NULL;
    uint8_t num[4] = {4, 1, 1, 0};
    cm_create(&cm, name, id, dt, num);
    if (cm == NULL)
    {
        LOG_E("PID %d Create Error.", id);
        return cm;
    }
    cm->type = CMODEL_PID;
    cm->par = (PIDPar_t *)calloc(1, sizeof(PIDPar_t));
    if (cm->par == NULL)
    {
        LOG_E("PID %d Create Par Error.", id);
        cm_deleate(&cm);
        return cm;
    }

    PIDPar_t *par = (PIDPar_t *)cm->par;
    par->HigOut = 100;
    par->LowOut = -100;
    par->KP = par->KI = par->KD = 0;
    par->SP.Basis = par->PV.Basis = 0;
    par->SP.Gain = par->PV.Gain = 1;
    par->sta.Err[0] = par->sta.Err[1] = par->sta.Err[2] = 0;
    par->sta.LastI = par->sta.LastKP = par->sta.LastU = 0;

    cm->deleateByCM = cm_commonDeleatePar;
    cm->run = _run;
    return cm;
}

CMODEL_STATUS_e pid_setPID(CModel cm, float p, float i, float d)
{
    IS_VALID_TYPE(cm, CMODEL_PID);
    PIDPar_t *par = cm->par;
    par->KP = p;
    par->KI = i;
    par->KD = d;
    return CMODEL_STATUS_OK;
}

static CMODEL_STATUS_e _pid_setSPPV(CModel cm, unsigned char sp_pv, float basis, float gain)
{
    IS_VALID_TYPE(cm, CMODEL_PID);
    PIDPar_t *par = cm->par;
    PIDAdj_t *adj = (sp_pv == 1) ? &par->PV : &par->SP;
    adj->Basis = basis;
    adj->Gain = gain;
    return CMODEL_STATUS_OK;
}

CMODEL_STATUS_e pid_setSP(CModel cm, float basis, float gain) { return _pid_setSPPV(cm, 0, basis, gain); }

CMODEL_STATUS_e pid_setPV(CModel cm, float basis, float gain) { return _pid_setSPPV(cm, 1, basis, gain); }

CMODEL_STATUS_e pid_setLimit(CModel cm, float h, float l)
{
    IS_VALID_TYPE(cm, CMODEL_PID);
    PIDPar_t *par = cm->par;
    par->HigOut = h;
    par->LowOut = l;
    return CMODEL_STATUS_OK;
}

CMODEL_STATUS_e pid_setDt(CModel cm, uint32_t dt)
{
    IS_VALID_TYPE(cm, CMODEL_PID);
    cm->dt = dt;
    return CMODEL_STATUS_OK;
}

#endif
