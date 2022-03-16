#ifndef __CONST_H
#define __CONST_H

#include "cm.h"

#if CM_CONST

#ifdef __cplusplus
extern "C"
{
#endif

	uint32_t const_create(CModel *cm, uint32_t id, uint32_t dt);
	uint32_t const_setTargetT(CModel cm, uint32_t tT);
	uint32_t const_setValue(CModel cm, a_value v);

#ifdef __cplusplus
}
#endif

#endif

#endif
