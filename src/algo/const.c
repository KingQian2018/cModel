#include "const.h"

#if CM_CONST

#if CM_LOG_CONST
#define LOG_TAG "CONST"
#endif
#include "cm_log.h"

const static char *name = "CONST";

typedef struct _ConstPar_s
{
	uint32_t triggleT; // 跳变时间
	uint32_t lstT;	  // 上次时间
	a_value value;	  // 设定值
} ConstPar_s;

static CMODEL_STATUS_e _del(CModel cm)
{
	IS_VALID_TYPE(cm, CMODEL_CONST);
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

	ConstPar_s *par = (ConstPar_s *)cm->par;
	par->lstT += dt;
	if (par->lstT > par->triggleT)
	{
		IO_SetAOValue(cm->io, IOPIN_1, par->value);
	}
}

CModel const_create(uint32_t id, uint32_t dt)
{
	uint8_t num[4] = {0, 0, 1, 0};
	CModel cm = NULL;
	cm_create(&cm, name, id, dt, num);
	if (cm == NULL)
	{
		LOG_E("CONST %d Create Error.", id);
		return cm;
	}
	cm->type = CMODEL_CONST;
	cm->par = (ConstPar_s *)calloc(1, sizeof(ConstPar_s));
	if (cm->par == NULL)
	{
		LOG_E("CONST %d Create Par Error.", id);
		cm_deleate(&cm);
		return cm;
	}

	ConstPar_s *par = (ConstPar_s *)cm->par;
	par->lstT = par->triggleT = 0;
	par->value = 1;
	
	cm->deleateByCM = cm_commonDeleatePar;
	cm->run = _run;
	return cm;
}

CMODEL_STATUS_e const_setTargetT(CModel cm, uint32_t tT)
{
	IS_VALID_TYPE(cm, CMODEL_CONST);
	ConstPar_s *par = (ConstPar_s *)cm->par;
	par->triggleT = tT;
	return CMODEL_STATUS_OK;
}

CMODEL_STATUS_e const_setValue(CModel cm, a_value v)
{
	IS_VALID_TYPE(cm, CMODEL_CONST);
	ConstPar_s *par = (ConstPar_s *)cm->par;
	par->value = v;
	return CMODEL_STATUS_OK;
}

#endif
