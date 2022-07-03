#include "common.h"

static void parse_test1()
{
#define PARSE_PATH "parse_test/cmodel.cm"
    TEST_ASSERT_MESSAGE(parse_file(PARSE_PATH) == CMODEL_STATUS_OK, "parse failed.");
}

int main(void)
{
    cm_elog_init();
    UNITY_BEGIN();
    RUN_TEST(parse_test1);
    return UNITY_END();
}
