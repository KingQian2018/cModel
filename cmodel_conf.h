#ifndef __CMODEL_CONF_H
#define __CMODEL_CONF_H

#define CM_USE_LOG 1

#if CM_USE_LOG
#define CM_LOG_IO 1
#define CM_LOG_CM 1
#define CM_LOG_PID 1
#define CM_LOG_CONST 1
#define CM_LOG_TRANSLATE 1
#define CM_LOG_PARSE 1
#else
#define CM_LOG_IO 0
#define CM_LOG_CM 0
#define CM_LOG_PID 0
#define CM_LOG_CONST 0
#define CM_LOG_TRANSLATE 0
#endif

#define CM_PID 1
#define CM_CONST 1
#define CM_TRANSLATE 1
#define CM_LIMIT 1

#define CM_SPRINTF_USE_FLOAT 1

#endif
