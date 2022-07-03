/**
 * @file parse_json.h
 * @author KisWang (KingQian2018)
 * @brief
 * @version 0.1
 * @date 2022-07-03
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __CM_PARSE_H
#define __CM_PARSE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "cmodel.h"
#include "pid.h"

    CMODEL_STATUS_e parse_file(const char* cm_file);

#ifdef __cplusplus
}
#endif

#endif