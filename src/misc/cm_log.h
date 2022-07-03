#ifndef __CM_LOG_H
#define __CM_LOG_H

#include "cmodel_conf.h"

#if CM_USE_LOG
#include "elog.h"

#define LOG_A log_a
#define LOG_E log_e
#define LOG_W log_w
#define LOG_I log_i
#define LOG_D log_d
#define LOG_V log_v
#else
#define LOG_A (void)
#define LOG_E (void)
#define LOG_W (void)
#define LOG_I (void)
#define LOG_D (void)
#define LOG_V (void)
#endif

#endif
