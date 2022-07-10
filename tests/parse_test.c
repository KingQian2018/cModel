#define LOG_TAG "PARSE_TEST"
#include <pthread.h>
#include "common.h"
#if WIN32
#include <windows.h>
#elif linux
#include <unistd.h>
#endif

static void parse_test1()
{
#define PARSE_PATH "parse_test/cmodel.cm"
    TEST_ASSERT_MESSAGE(parse_file(PARSE_PATH) == CMODEL_STATUS_OK, "parse failed.");
}

static void *_run(void *arg)
{
    int *v = (int *)arg;
    while (v[0]--)
    {
        cm_run(1);
        if (v[0] % 100 == 0)
        {
            LOG_I("run %d", v[0]);
        }
        Sleep(1);
    }
    return 0;
}

static void *_timer(void *arg)
{
    char tmp[256];
    FILE *v = (FILE *)arg;
    unsigned char idx = 1;
    CModel cm = cm_getModelByID(idx);
    while (cm != NULL)
    {
        cm_showAll(cm);
        idx++;
        cm = cm_getModelByID(idx);
    }

    while (true)
    {
        Sleep(10);
        fwrite(tmp, sprintf(tmp, "$"), 1, v);
        for (unsigned char i = 0; i < idx; i++)
        {
            CModel cmShow = cm_getModelByID(i);
            if (cmShow != NULL)
            {
                for (unsigned char pinIdx = 0; pinIdx < cmShow->io->O.ANum; pinIdx++)
                {
                    a_value av = IO_GetAValue(cmShow->io, pinIdx, IOTYP_AO);
                    fwrite(tmp, sprintf(tmp, "%.3f ", av), 1, v);
                }
            }
        }
        fwrite(tmp, sprintf(tmp, "0;"), 1, v);
    }
    return 0;
}

static void run_test()
{
    pthread_t th, timer;
    FILE *fSerial;
    TEST_ASSERT_MESSAGE((fSerial = fopen("COM2", "wb+")) != NULL, "Filed to open COM2");
    int arg = 2000;
    int ret = pthread_create(&th, NULL, _run, &arg);
    int *thread_ret = NULL;
    TEST_ASSERT_MESSAGE(ret == 0, "Create thread error!");
    ret = pthread_create(&timer, NULL, _timer, fSerial);
    TEST_ASSERT_MESSAGE(ret == 0, "Create timer error!");
    pthread_join(th, (void **)&thread_ret);
    fclose(fSerial);
}

int main(void)
{
    cm_elog_init();
    UNITY_BEGIN();
    RUN_TEST(parse_test1);
    RUN_TEST(run_test);
    return UNITY_END();
}
