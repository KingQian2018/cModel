#ifndef __COMMON_H
#define __COMMON_H

#include "cm_log.h"
#include "io_port.h"

#include "pid.h"
#include "const.h"
#include "translate.h"
#include "limit.h"

#define _CONDITION_OK(condition) (condition) == CMODEL_STATUS_OK

#define CMODEL_TEST_ASSERT_CREATE_OK(condition, cmodel) TEST_ASSERT_MESSAGE(_CONDITION_OK(condition), "Failed to create " #cmodel)
#define CMODEL_TEST_ASSERT_CREATE_FAIL(condition, cmodel) TEST_ASSERT_MESSAGE(!_CONDITION_OK(condition), "Succeed to create " #cmodel)
#define CMODEL_TEST_ASSERT_SETLINK_OK(condition, cmodel) TEST_ASSERT_MESSAGE(_CONDITION_OK(condition), "Failed to set-link " #cmodel)
#define CMODEL_TEST_ASSERT_SETLINK_FAIL(condition, cmodel) TEST_ASSERT_MESSAGE(!_CONDITION_OK(condition), "Failed to set-link " #cmodel)

#endif
