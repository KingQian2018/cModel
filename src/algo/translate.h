#ifndef __TRANSLATE_H
#define __TRANSLATE_H

#include "cm.h"

#if CM_TRANSLATE

#ifdef __cplusplus
extern "C"
{
#endif

	CModel translate_create(uint32_t id, uint32_t dt);
	CMODEL_STATUS_e translate_setPar(CModel cm, float K, uint32_t T, short n, float Tao);

#ifdef __cplusplus
}
#endif

#endif

#endif
