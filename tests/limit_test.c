
#include <stdio.h>

#include "Unity/src/unity_internals.h"
#include "Unity/examples/unity_config.h"
#include "unity/src/unity.h"

#include "common.h"

CModel _m_const_2;
CModel _m_limit_1;
void limit_test(void)
{
    elog_init();
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_start();

    const_create(&_m_const_2, 6, 1);
    const_setTargetT(_m_const_2, 100);
    const_setValue(_m_const_2, 200);
    limit_create(&_m_limit_1, 7, 1);
    limit_setPar(_m_limit_1, 100, -100, 100);

    cm_setLink(IOTYP_AI, _m_limit_1, IOPIN_1, _m_const_2, IOPIN_1);
}

void test_limit_change(a_value v)
{
    const_setValue(_m_const_2, v);
}

void test_run()
{
    limit_test();
    uint32_t count = 0;
    while (count < 500)
    {
        cm_run(1);
        if (count == 500 / 2)
        {
            // const_setValue(m_const_1, -5.0f);
            test_limit_change(-200);
        }
        cm_getAPin(_m_const_2, IOPIN_1, IOTYP_AO);
        cm_getAPin(_m_limit_1, IOPIN_1, IOTYP_AO);
        count++;
    }
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_run);
    return UNITY_END();
}
