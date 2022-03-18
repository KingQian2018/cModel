
#include <stdio.h>
#include "cm_log.h"
#include "io_port.h"

#include "const.h"
#include "limit.h"
#include "switch.h"

CModel _m_const_2;
CModel _m_limit_1;
CModel _m_switch_1;
CModel _m_switch_2;
void test_limit(void)
{
    const_create(&_m_const_2, 6, 1);
    const_setTargetT(_m_const_2, 100);
    const_setValue(_m_const_2, 200);

    limit_create(&_m_limit_1, 7, 1);
    limit_setPar(_m_limit_1, 100, -100, 100);

    switch_create(&_m_switch_1, 8, 1, SWITCH_TYPE_L2H);
    switch_setPar(_m_switch_1, SWITCH_TYPE_L2H, 400);
    switch_create(&_m_switch_2, 9, 1, SWITCH_TYPE_H2L);
    switch_setPar(_m_switch_2, SWITCH_TYPE_H2L, 200);

    cm_setLink(IOTYP_AI, _m_limit_1, IOPIN_1, _m_const_2, IOPIN_1);
    cm_setLink(IOTYP_DI, _m_limit_1, IOPIN_1, _m_switch_1, IOPIN_1);
    cm_setLink(IOTYP_DI, _m_limit_1, IOPIN_2, _m_switch_2, IOPIN_1);
}

void test_limit_change(a_value v)
{
    const_setValue(_m_const_2, v);
}

void test_switch1_toggle(uint32_t triT)
{
    switch_toggle(_m_switch_1, 0);
}

void test_switch2_toggle(uint32_t triT)
{
    switch_toggle(_m_switch_2, 0);
}
