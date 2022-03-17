
#include <stdio.h>
#include "cm_log.h"
#include "io_port.h"

#include "const.h"
#include "limit.h"

CModel _m_const_2;
CModel _m_limit_1;
void test_limit(void)
{
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
