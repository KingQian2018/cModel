#ifndef QCM_WINDOW_H
#define QCM_WINDOW_H

#include <QtWidgets>
#include <QLabel>
#include <QComboBox>

#include "qcm.h"
#include "qcmodel.h"
#include "qcm_view.h"
#include "qcm_scene.h"

#include "qcm_pid.h"

class QCM_Window : public QWidget
{
    Q_OBJECT
public:
    QCM_Window(QWidget *parent = nullptr);
    ~QCM_Window();

private:
    QCM_View *m_view;
    QLabel *m_poslabel;
    QComboBox *m_select;
private slots:
    void viewMouseMoved(QPointF pos);
    void paperSizeChanged(int index);
};

#endif // QCM_WINDOW_H
