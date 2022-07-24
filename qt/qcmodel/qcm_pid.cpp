#include "qcm_pid.h"

QCM_PID::QCM_PID(uint ID, QGraphicsItem *parent) : QCModel(QStringList({"SP", "PV", "FF"}),
                                                           QStringList({"AO"}),
                                                           QStringList({"TR"}),
                                                           QStringList(),
                                                           ID, "PID", 100, parent)
{
}

QCM_PID::~QCM_PID()
{
}

QCM_PID *QCM_PID::copy()
{
    return new QCM_PID(*this);
}