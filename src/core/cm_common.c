#include "cmodel.h"


#if CM_USE_LOG
#define T(t) #t,
const char *_loginfo[] = {CMODEL_ERROR};
#undef T
#else
const char *_loginfo[] = {};
#endif
