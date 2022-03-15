#ifndef __PID_H
#define __PID_H
#include "cm.h"

#ifdef __cplusplus
extern "C"
{
#endif

    uint32_t pid_create(CModel *cm, uint32_t id, uint32_t dt);

#ifdef __cplusplus
}
#endif

#endif
