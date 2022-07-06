#include <pthread.h>
#include "common.h"
#if WIN32
#include <windows.h>
#elif linux
#include <unistd.h>
#endif 

#define LOG_TAG "PARSE_TEST"

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
    int *v = (int *)arg;
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
        Sleep(v[0]);
        for (unsigned char i = 1; i < idx; i++)
        {
            cm_showAll(cm_getModelByID(i));
        }
    }
    return 0;
}

static void run_test()
{
    pthread_t th, timer;
    int arg = 2000;
    int ret = pthread_create(&th, NULL, _run, &arg);
    int *thread_ret = NULL;
    TEST_ASSERT_MESSAGE(ret == 0, "Create thread error!");
    int timer_arg = 1000;
    ret = pthread_create(&timer, NULL, _timer, &timer_arg);
    TEST_ASSERT_MESSAGE(ret == 0, "Create timer error!");
    pthread_join(th, (void **)&thread_ret);
}

int main(void)
{
    cm_elog_init();
    UNITY_BEGIN();
    RUN_TEST(parse_test1);
    RUN_TEST(run_test);
    return UNITY_END();
}
