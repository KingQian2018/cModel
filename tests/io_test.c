#include "common.h"

IO _io;
#define SHOW_NUMBER (50000)
#include <string.h>

static void _iotest(void)
{
    unsigned char num[4];
    num[IOTYP_AI] = 2;
    num[IOTYP_AO] = 1;
    num[IOTYP_DI] = 2;
    num[IOTYP_DO] = 2;
    TEST_ASSERT(IO_Create(&_io, num) == CMODEL_STATUS_OK);
    TEST_ASSERT(IO_Create(&_io, num) != CMODEL_STATUS_OK);
    IO_ShowALL(_io);
    TEST_ASSERT(IO_SetAOValue(_io, IOPIN_1, 1.0f) == CMODEL_STATUS_OK);
    IO_ShowPin(_io, IOTYP_AO, IOPIN_1);
    TEST_ASSERT(IO_SetAOValue(_io, IOPIN_8, 1.0f) != CMODEL_STATUS_OK);
    IO_ShowPin(_io, IOTYP_AO, IOPIN_8);
}

CModel m_pid_1 = NULL;
CModel m_pid_2 = NULL;
CModel m_pid_3 = NULL;
static void _pid_test(void)
{
    CMODEL_TEST_ASSERT_CREATE_OK(m_pid_1 = pid_create(1, 1), m_pid_1);
    CMODEL_TEST_ASSERT_CREATE_OK(m_pid_2 = pid_create(1, 1), m_pid_2);
    CMODEL_TEST_ASSERT_CREATE_OK(m_pid_3 = pid_create(1, 1), m_pid_3);
    CMODEL_TEST_ASSERT_SETLINK_OK(cm_setLink(IOTYP_AI, m_pid_1, IOPIN_1, m_pid_2, IOPIN_1), m_pid_2);
    cm_deleate(&m_pid_3);
    IO_ShowALL(m_pid_1->io);
}

CModel m_const_1 = NULL;
static void _const_test(void)
{
    CMODEL_TEST_ASSERT_CREATE_OK(m_const_1 = const_create(4, 1), m_const_1);
    CMODEL_TEST_ASSERT_SETLINK_OK(cm_setLink(IOTYP_AI, m_pid_1, IOPIN_1, m_const_1, IOPIN_1), m_const_1);
}

CModel m_translate_1 = NULL;
CModel m_translate_2 = NULL;
static void _translate_test(void)
{
    CMODEL_TEST_ASSERT_CREATE_OK(m_translate_1 = translate_create(5, 1), m_translate_1);
    CMODEL_TEST_ASSERT_SETLINK_OK(cm_setLink(IOTYP_AI, m_translate_1, IOPIN_1, m_pid_1, IOPIN_1), m_pid_1);
    CMODEL_TEST_ASSERT_SETLINK_OK(cm_setLink(IOTYP_AI, m_pid_1, IOPIN_2, m_translate_1, IOPIN_1), m_translate_1);
    translate_setPar(m_translate_1, 1, 1, 1, 0);
}

static void _model_create(void)
{
    m_pid_1 = pid_create(1, 1);
    m_pid_2 = pid_create(2, 1);
    m_pid_3 = pid_create(3, 1);
    m_const_1 = const_create(4, 1);
    m_translate_1 = translate_create(5, 1);
    m_translate_2 = translate_create(6, 1);
}

static void _model_set(void)
{
    pid_setPID(m_pid_1, 1.75283057974542, 0.208227670266639, 0);
    pid_setPID(m_pid_2, 1.304228, 0.4293199, 0);
    translate_setPar(m_translate_1, 1, 5, 1, 0);
    translate_setPar(m_translate_2, 1, 10, 1, 0);
    const_setValue(m_const_1, 1.0f);
}

static void _model_setLink(void)
{
    cm_setLink(IOTYP_AI, m_pid_1, IOPIN_2, m_const_1, IOPIN_1);     // sp1
    cm_setLink(IOTYP_AI, m_pid_1, IOPIN_1, m_translate_2, IOPIN_1); // pv1

    cm_setLink(IOTYP_AI, m_pid_2, IOPIN_2, m_pid_1, IOPIN_1);       // sp1
    cm_setLink(IOTYP_AI, m_pid_2, IOPIN_1, m_translate_1, IOPIN_1); // pv2

    cm_setLink(IOTYP_AI, m_translate_1, IOPIN_1, m_pid_2, IOPIN_1);       // obj
    cm_setLink(IOTYP_AI, m_translate_2, IOPIN_1, m_translate_1, IOPIN_1); // obj
}

#include <windows.h>
static void test()
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
    _model_create();
    _model_set();
    _model_setLink();
    FILE *file = NULL;
    file = fopen("COM2", "wb+");
    char tmp[256];
    TEST_ASSERT_NOT_NULL_MESSAGE(file, "failed to open COM2");
    uint32_t count = 0;
    if (file != NULL)
    {

        while (count < SHOW_NUMBER)
        {
            cm_run(1);
            if (count == 5000)
            {
                const_setValue(m_const_1, -5.0f);
            }
            fwrite(tmp, sprintf(tmp, "$"), 1, file);
            fwrite(tmp, sprintf(tmp, "%.3f ", cm_getAPin(m_const_1, IOPIN_1, IOTYP_AO)), 1, file);
            fwrite(tmp, sprintf(tmp, "%.3f ", cm_getAPin(m_pid_1, IOPIN_1, IOTYP_AO)), 1, file);
            fwrite(tmp, sprintf(tmp, "%.3f ", cm_getAPin(m_translate_1, IOPIN_1, IOTYP_AO)), 1, file);
            fwrite(tmp, sprintf(tmp, "%.3f ", cm_getAPin(m_pid_2, IOPIN_1, IOTYP_AO)), 1, file);
            fwrite(tmp, sprintf(tmp, "%.3f", cm_getAPin(m_translate_2, IOPIN_1, IOTYP_AO)), 1, file);
            fwrite(tmp, sprintf(tmp, ";"), 1, file);
            count++;
            Sleep(1);
        }
        fclose(file);
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test);
    return UNITY_END();
}
