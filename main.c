
#include <stdio.h>
#include "cm_log.h"
#include "io_port.h"

#include "pid.h"
#include "const.h"
#include "translate.h"

IO _io;

void _iotest(void)
{
    unsigned char num[4];
    num[IOTYP_AI] = 2;
    num[IOTYP_AO] = 1;
    num[IOTYP_DI] = 2;
    num[IOTYP_DO] = 2;
    IO_Create(&_io, num);
    IO_Create(&_io, num);
    IO_ShowALL(_io);
    IO_SetAOValue(_io, IOPIN_1, 1.0f);
    IO_ShowPin(_io, IOTYP_AO, IOPIN_1);
    IO_SetAOValue(_io, IOPIN_8, 1.0f);
    IO_ShowPin(_io, IOTYP_AO, IOPIN_8);
}

CModel m_pid_1 = NULL;
CModel m_pid_2 = NULL;
CModel m_pid_3 = NULL;
void _pid_test(void)
{
    pid_create(&m_pid_1, 1, 1);
    pid_create(&m_pid_2, 2, 1);
    pid_create(&m_pid_3, 3, 1);
    cm_setLink(IOTYP_AI, m_pid_1, IOPIN_1, m_pid_2, IOPIN_1);
    cm_deleate(&m_pid_3);
    IO_ShowALL(m_pid_1->io);
}

CModel m_const_1 = NULL;
void _const_test(void)
{
    const_create(&m_const_1, 4, 1);
    cm_setLink(IOTYP_AI, m_pid_1, IOPIN_1, m_const_1, IOPIN_1);
    // const_setTargetT(m_const_1, 5);
    // const_setValue(m_const_1, 5.0f);
}

CModel m_translate_1 = NULL;
CModel m_translate_2 = NULL;
void _translate_test(void)
{
    translate_create(&m_translate_1, 5, 1);
    cm_setLink(IOTYP_AI, m_translate_1, IOPIN_1, m_pid_1, IOPIN_1);
    cm_setLink(IOTYP_AI, m_pid_1, IOPIN_2, m_translate_1, IOPIN_1);
    translate_setPar(m_translate_1, 1, 1, 1, 0);
}

void _model_create(void)
{
    pid_create(&m_pid_1, 1, 1);
    pid_create(&m_pid_2, 2, 1);
    pid_create(&m_pid_3, 3, 1);
    const_create(&m_const_1, 4, 1);
    translate_create(&m_translate_1, 5, 1);
    translate_create(&m_translate_2, 5, 1);
}

void _model_set(void)
{
    pid_setPID(m_pid_1, 1, 0.1, 0);
    pid_setPID(m_pid_2, 0.5, 0.05, 0);
    translate_setPar(m_translate_1, 1, 100, 1, 0);
    translate_setPar(m_translate_2, 1, 500, 1, 0);
    const_setTargetT(m_const_1, 100);
    const_setValue(m_const_1, 5.0f);
}

void _model_setLink(void)
{
    cm_setLink(IOTYP_AI, m_pid_1, IOPIN_2, m_const_1, IOPIN_1);     // sp1
    cm_setLink(IOTYP_AI, m_pid_1, IOPIN_1, m_translate_2, IOPIN_1); // pv1

    cm_setLink(IOTYP_AI, m_pid_2, IOPIN_2, m_pid_1, IOPIN_1);       // sp1
    cm_setLink(IOTYP_AI, m_translate_1, IOPIN_1, m_pid_2, IOPIN_1); // pv2

    cm_setLink(IOTYP_AI, m_translate_2, IOPIN_1, m_translate_1, IOPIN_1); // obj
}

#define SHOW_ITEMS (5)
#define SHOW_NUMBER (1000)
a_value temp[SHOW_ITEMS][SHOW_NUMBER];
#include <stdio.h>
#include <string.h>
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

    // _iotest();
    _model_create();
    _model_set();
    _model_setLink();

    FILE *f;
    f = fopen("D:/Users/KisWang/Documents/kw_cModel/testPY/out.txt", "w");

    uint32_t count = 0;
    while (count < SHOW_NUMBER)
    {
        cm_run(1);
        if (count == 500)
        {
            const_setValue(m_const_1, -5.0f);
        }
        temp[0][count] = cm_getAPin(m_const_1, IOPIN_1, IOTYP_AO);
        temp[1][count] = cm_getAPin(m_pid_1, IOPIN_1, IOTYP_AO);
        temp[2][count] = cm_getAPin(m_translate_1, IOPIN_1, IOTYP_AO);
        temp[3][count] = cm_getAPin(m_pid_2, IOPIN_1, IOTYP_AO);
        temp[4][count] = cm_getAPin(m_translate_2, IOPIN_1, IOTYP_AO);
        count++;
    }
    for (uint32_t m = 0; m < SHOW_ITEMS; m++)
    {
        fprintf(f, "\n");
        for (uint32_t n = 0; n < SHOW_NUMBER; n++)
        {
            fprintf(f, "%.3f ", temp[m][n]);
        }
    }
    fclose(f);
    cm_showAll(m_pid_1);

    return 1;
}
