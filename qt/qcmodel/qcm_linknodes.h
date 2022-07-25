#ifndef QCM_LINKNODES_H
#define QCM_LINKNODES_H

#include "qcm.h"
#include "qcm_nodeline.h"

namespace QCM
{
    class QCM_LinkNodes;
}

class QCM_LinkNodes
{
public:
    QCM_LinkNodes(QGraphicsItem *parent = nullptr);
    QCM_LinkNodes(QCM_Node *node, QGraphicsItem *parent = nullptr);
    ~QCM_LinkNodes() {}

    void addNode(QCM_Node *node);
    void addNode(qreal x, qreal y);

private:
    void init();

    QList<QCM_NodeLine *> m_nodeLines;
    QGraphicsItem* m_parent;
};

#endif // QCM_LINKNODES_H
