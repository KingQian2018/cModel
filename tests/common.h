#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include "cm_log.h"
#include "cm_parse.h"
#include "io_port.h"

#include "pid.h"
#include "const.h"
#include "translate.h"
#include "limit.h"

#include "Unity/src/unity_internals.h"
#include "Unity/examples/unity_config.h"
#include "unity/src/unity.h"

#define _CREATE_OK(condition) (condition) != NULL
#define _CONDITION_OK(condition) (condition) == CMODEL_STATUS_OK

#define CMODEL_TEST_ASSERT_CREATE_OK(condition, cmodel) TEST_ASSERT_MESSAGE(_CREATE_OK(condition), "Failed to create " #cmodel)
#define CMODEL_TEST_ASSERT_CREATE_FAIL(condition, cmodel) TEST_ASSERT_MESSAGE(!_CREATE_OK(condition), "Succeed to create " #cmodel)
#define CMODEL_TEST_ASSERT_SETLINK_OK(condition, cmodel) TEST_ASSERT_MESSAGE(_CONDITION_OK(condition), "Failed to set-link " #cmodel)
#define CMODEL_TEST_ASSERT_SETLINK_FAIL(condition, cmodel) TEST_ASSERT_MESSAGE(!_CONDITION_OK(condition), "Failed to set-link " #cmodel)

void cm_elog_init()
{
    elog_init();
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_set_text_color_enabled(true);
    elog_start();
}

#endif
