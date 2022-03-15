#ifndef __CMODEL_CONF_H
#define __CMODEL_CONF_H

#define CM_USE_LOG 1

#if CM_USE_LOG
#define CM_LOG_IO 1
#define CM_LOG_CM 1
#define CM_LOG_PID 1
#else
#define CM_LOG_IO 0
#define CM_LOG_CM 0
#define CM_LOG_PID 0
#endif

#define CM_SPRINTF_USE_FLOAT 1

#endif
