#ifndef QCM_WINDOW_H
#define QCM_WINDOW_H

#include <QtWidgets>
#include <QLabel>
#include <QComboBox>

#include "qcm.h"
#include "qcmodel.h"
#include "qcm_view.h"
#include "qcm_scene.h"
#include "qcm_nodeline.h"
#include "qcm_node.h"
#include "qcm_linknodes.h"

#include "qcm_pid.h"

class QCM_Window : public QWidget
{
    Q_OBJECT
public:
    QCM_Window(QWidget *parent = nullptr);
    ~QCM_Window();

private:
    QCM_View *m_view;
    QCM_Scene *m_scene;
    QLabel *m_poslabel;
    QComboBox *m_select;
    QList<QAbstractButton *> m_btns;
    QVBoxLayout *m_vboxlayout;
    QCM_NodeLine *m_nodeLine;
    QCM_Node *m_node;

    bool isSetLink;

private slots:
    void viewMouseMoved(QPointF pos);
    void paperSizeChanged(int index);
    void gridChanged(int index);
    void btnClicked();
};

#endif // QCM_WINDOW_H
