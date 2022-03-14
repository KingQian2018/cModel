
#include <stdio.h>
#include "cm_log.h"
#include "io_port.h"

IO _io;
const char *io_test_name = "Test";

void _iotest(void)
{
    unsigned char num[4];
    num[IOTYP_AI] = 2;
    num[IOTYP_AO] = 1;
    num[IOTYP_DI] = 2;
    num[IOTYP_DO] = 2;
    IO_Create(&_io, io_test_name, num);
    IO_Create(&_io, io_test_name, num);
    IO_ShowALL(_io);
    IO_SetAOValue(_io, IOPIN_1, 1.0f);
    IO_ShowPin(_io, IOTYP_AO, IOPIN_1);
    IO_SetAOValue(_io, IOPIN_8, 1.0f);
    IO_ShowPin(_io, IOTYP_AO, IOPIN_8);
}

int main(void)
{
    elog_init();
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_start();

    _iotest();

    return 1;
}
