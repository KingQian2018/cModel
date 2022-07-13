#include <qcmodel.h>

#include "cm_log.h"
#include "cm_parse.h"
#include "io_port.h"

void qcm_elog_init()
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

void qcm_init()
{
    if (parse_file("../tests/parse_test/cmodel.json") != CMODEL_STATUS_OK)
    {
        LOG_E("prase failed.");
    }
}

qcmodel::qcmodel()
{
}

qcmodel::~qcmodel()
{
    if (is_run)
    {
        is_run = false;
    }
}

void qcmodel::run()
{
    while (is_run)
    {
        msleep(1);
        cm_run(1);
    }
}
