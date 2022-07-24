#ifndef QCM_PID_H
#define QCM_PID_H

#include "qcmodel.h"

class QCM_PID : public QCModel
{
public:
    QCM_PID(uint id, QGraphicsItem *parent = nullptr);
    ~QCM_PID();
    QCM_PID* copy();
};

#endif // QCM_PID_H
