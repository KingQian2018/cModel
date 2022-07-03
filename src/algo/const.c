#include "const.h"

#if CM_CONST

#if CM_LOG_CONST
#define LOG_TAG "CONST"
#endif
#include "cm_log.h"

const static char *name = "CONST";

typedef struct _ConstPar_s
{
	uint32_t targetT; // 跳变时间
	uint32_t lstT;	  // 上次时间
	a_value value;	  // 设定值
} ConstPar_s;

static uint32_t _del(CModel cm)
{
	IS_VALID_TYPE(cm, CMODEL_CONST);
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

	ConstPar_s *par = (ConstPar_s *)cm->par;
	par->lstT += dt;
	if (par->lstT > par->targetT)
	{
		IO_SetAOValue(cm->io, IOPIN_1, par->value);
	}
}

uint32_t const_create(CModel *cm, uint32_t id, uint32_t dt)
{
	uint8_t num[4] = {0, 0, 1, 0};
	cm_create(cm, name, id, dt, num);
	if (cm[0] == NULL)
	{
		LOG_E("CONST %d Create Error.", id);
		return CMODEL_STATUS_CM_CREATE;
	}
	cm[0]->type = CMODEL_CONST;
	cm[0]->par = (ConstPar_s *)calloc(1, sizeof(ConstPar_s));
	if (cm[0]->par == NULL)
	{
		LOG_E("CONST %d Create Par Error.", id);
		return CMODEL_STATUS_CM_CREATEPAR;
	}

	ConstPar_s *par = (ConstPar_s *)cm[0]->par;
	par->lstT = par->targetT = 0;
	par->value = 1;
	
	cm[0]->deleateByCM = _del;
	cm[0]->run = _run;
	return CMODEL_STATUS_OK;
}

uint32_t const_setTargetT(CModel cm, uint32_t tT)
{
	IS_VALID_TYPE(cm, CMODEL_CONST);
	ConstPar_s *par = (ConstPar_s *)cm->par;
	par->targetT = tT;
	return CMODEL_STATUS_OK;
}

uint32_t const_setValue(CModel cm, a_value v)
{
	IS_VALID_TYPE(cm, CMODEL_CONST);
	ConstPar_s *par = (ConstPar_s *)cm->par;
	par->value = v;
	return CMODEL_STATUS_OK;
}

#endif
