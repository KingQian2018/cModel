#ifndef __QCMODEL_H
#define __QCMODEL_H

#include <QObject>
#include <QThread>

extern void qcm_elog_init();
extern void qcm_init();


class qcmodel : public QThread
{
    Q_OBJECT
private:
public:
    qcmodel();
    ~qcmodel();

    void stop() { is_run = false; }
    bool isRun() { return is_run; }

protected:
    void run();
    bool is_run = true;
};

#endif
