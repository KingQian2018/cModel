#include "translate.h"
#include <memory.h>
#include "cm_log.h"

#if CM_TRANSLATE

#if CM_LOG_TRANSLATE
#define LOG_TAG "TRANSLATE"
#endif

const static char *name = "TRANSLATE";

/// 模块状态结构体
typedef struct
{
	a_value *pMidStat;
	float a;
	float b;
} TranslateSta_s;

typedef struct _TranslatePar_s
{
	float K, T;
	unsigned short n, Tao;
	TranslateSta_s sta;
} TranslatePar_s;

static uint32_t _del(CModel cm)
{
	IS_VALID_TYPE(cm, CMODEL_TRANSLATE);
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

	TranslatePar_s *par = (TranslatePar_s *)cm->par;
	unsigned short mySta = par->n + (par->Tao) / dt;

	for (unsigned short i = mySta - 1; i >= par->n; i--)
	{
		par->sta.pMidStat[i] = par->sta.pMidStat[i - 1];
	}

	uint32_t mySta1 = par->n;
	if (par->n > 1)
	{

		for (unsigned short i = par->n; i > 0; i--)
		{
			par->sta.pMidStat[i] = par->sta.a * par->sta.pMidStat[i] + par->sta.b * par->sta.pMidStat[i - 1];
		}
	}
	par->sta.pMidStat[0] = par->sta.a * par->sta.pMidStat[0] + par->sta.b * par->K * IO_GetAValue(cm->io, IOPIN_1, IOTYP_AI);

	IO_SetAOValue(cm->io, IOPIN_1, par->sta.pMidStat[mySta - 1]);

	return CMODEL_STATUS_OK;
}

#include <math.h>
uint32_t translate_create(CModel *cm, uint32_t id, uint32_t dt)
{
	uint8_t num[4] = {1, 0, 1, 0};
	cm_create(cm, name, id, dt, num);
	if (cm[0] == NULL)
	{
		LOG_E("%s %d Create Error.", name, id);
		return CMODEL_STATUS_CM_CREATE;
	}
	cm[0]->type = CMODEL_TRANSLATE;
	cm[0]->par = (TranslatePar_s *)calloc(1, sizeof(TranslatePar_s));
	if (cm[0]->par == NULL)
	{
		LOG_E("%s %d Create Par Error.", name, id);
		return CMODEL_STATUS_CM_CREATEPAR;
	}

	TranslatePar_s *par = (TranslatePar_s *)cm[0]->par;
	par->K = 1;
	par->Tao = 0;
	par->T = 1;
	par->n = 1;
	par->sta.pMidStat = (a_value *)malloc(1 * sizeof(a_value));
	if (par->sta.pMidStat == NULL)
	{
		LOG_E("pMidStat malloc error.");
		return CMODEL_STATUS_CM_CREATEPAR;
	}
	else
	{
		memset(par->sta.pMidStat, 0, 1 * sizeof(a_value));
	}
	par->sta.a = expf(-((float)cm[0]->dt / 1000.0f) / (float)par->T);
	par->sta.b = 1 - par->sta.a;

	cm[0]->deleateByCM = _del;
	cm[0]->run = _run;
	return CMODEL_STATUS_OK;
}

/**
 * @param cm		模型
 * @param K			增益
 * @param T			时间常数。单位 S
 * @param n			阶次
 * @param Tao 		纯延时。单位 S
 * @return 
 * 		CMODEL_STATUS_OK 成功
 * 		CMODEL_STATUS_CM_CREATEPAR 中间状态参数创建失败 
 */
uint32_t translate_setPar(CModel cm, float K, uint32_t T, short n, float Tao)
{
	IS_VALID_TYPE(cm, CMODEL_TRANSLATE);
	TranslatePar_s *par = (TranslatePar_s *)cm->par;
	par->K = K;
	par->T = T;
	par->n = n;
	par->Tao = Tao * 1000;
	if (par->sta.pMidStat != NULL)
	{
		free(par->sta.pMidStat);
		par->sta.pMidStat = NULL;
	}
	uint32_t temp = (cm->dt != 0) ? par->n + (par->Tao) / cm->dt : 0;
	if (temp != 0)
	{
		par->sta.pMidStat = (a_value *)malloc(temp * sizeof(a_value));
		if (par->sta.pMidStat == NULL)
		{
			LOG_E("pMidStat malloc error.");
			return CMODEL_STATUS_CM_CREATEPAR;
		}
		else
		{
			memset(par->sta.pMidStat, 0, temp * sizeof(a_value));
		}
	}
	par->sta.a = expf(-((float)cm->dt / 1000.0f) / (float)par->T);
	par->sta.b = 1 - par->sta.a;
	return CMODEL_STATUS_OK;
}

#endif
