#ifndef __CONST_H
#define __CONST_H

#include "cm.h"

#if CM_CONST

#ifdef __cplusplus
extern "C"
{
#endif

	CModel const_create(uint32_t id, uint32_t dt);
	CMODEL_STATUS_e const_setTargetT(CModel cm, uint32_t tT);
	CMODEL_STATUS_e const_setValue(CModel cm, a_value v);

#ifdef __cplusplus
}
#endif

#endif

#endif
