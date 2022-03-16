#include "translate.h"

#if CM_TRANSLATE

#if CM_LOG_TRANSLATE
#define LOG_TAG "TRANSLATE"
#define T(t) #t,
static const char *_loginfo[] = {CMODEL_ERROR};
#undef T
#include "cm_log.h"
#else
static const char *_loginfo[] = NULL;
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
	unsigned short mySta = par->n + par->Tao;

	for (unsigned short i = mySta - 1; i >= par->n; i--)
	{
		par->sta.pMidStat[i] = par->sta.pMidStat[i - 1];
	}

	for (unsigned short i = par->n; i > 0; i--)
	{
		par->sta.pMidStat[i] = par->sta.a * par->sta.pMidStat[i] + par->sta.b * par->sta.pMidStat[i - 1];
	}
	par->sta.pMidStat[0] = par->sta.a * par->sta.pMidStat[0] + par->sta.b * par->K * IO_GetAValue(cm->io, IOPIN_1, IOTYP_AI);

	IO_SetAOValue(cm->io, IOPIN_1, par->sta.pMidStat[mySta - 1]);

	return CMODEL_STATUS_OK;
}

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
	par->K = par->T = par->n = 1;
	par->Tao = 0;
	par->sta.pMidStat = (a_value *)calloc(1, sizeof(a_value));
	par->sta.a = expf(-((float)cm[0]->dt) / (float)par->T);
	par->sta.b = 1 - par->sta.a;

	cm[0]->deleateByCM = _del;
	cm[0]->run = _run;
	return CMODEL_STATUS_OK;
}

#include <math.h>
uint32_t translate_setPar(CModel cm, float K, float T, float n, float Tao)
{
	IS_VALID_TYPE(cm, CMODEL_TRANSLATE);
	TranslatePar_s *par = (TranslatePar_s *)cm->par;
	par->K = K;
	par->T = T;
	par->n = n;
	par->Tao = Tao;
	if (par->sta.pMidStat != NULL)
	{
		free(par->sta.pMidStat);
	}
	par->sta.pMidStat = (n + Tao != 0) ? (a_value *)calloc(n + Tao, sizeof(a_value)) : NULL;
	par->sta.a = expf(-((float)cm->dt) / (float)par->T);
	par->sta.b = 1 - par->sta.a;
	return CMODEL_STATUS_OK;
}

#endif
