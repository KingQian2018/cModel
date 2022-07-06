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

#include <glad/glad.h>
#include <GLFW/glfw3.h>
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
static int run_graphic()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw创建窗口
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        printf("创建窗口失败");
        //终止
        glfwTerminate();
        return -1;
    }
    //设置当前OpenGL上下文
    glfwMakeContextCurrent(window);

    //设置回调，当窗口大小调整后将调用该回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //设置回调，当发生按键操作时将调用该回调函数
    glfwSetKeyCallback(window, key_callback);

    // glad初始化
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("加载失败");
        return -1;
    }

    // 使用循环达到循环渲染效果
    while (!glfwWindowShouldClose(window))
    {
        //检查事件
        glfwPollEvents();

        //渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //交换缓冲
        glfwSwapBuffers(window);
    }

    //终止渲染 关闭并清理glfw本地资源
    glfwTerminate();
    return 0;
}

int main(void)
{
    cm_elog_init();
    // UNITY_BEGIN();
    // RUN_TEST(parse_test1);
    // RUN_TEST(run_test);
    // return UNITY_END();
    return run_graphic();
}
