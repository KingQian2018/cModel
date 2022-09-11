#ifndef QCM_NODELINE_H
#define QCM_NODELINE_H

#include "qcm_node.h"
#include "qcm_line.h"

class QCM_NodeLine
{
public:
    QCM_NodeLine(QGraphicsScene *scene);
    ~QCM_NodeLine();

    void addNode(QCM_Node *node);
    void removeNode(QCM_Node *const node);
    QList<QCM_Node *> nodes() const { return m_nodes; }

private:
    QList<QCM_Node *> m_nodes;
    QGraphicsScene *m_scene;
};

#endif
