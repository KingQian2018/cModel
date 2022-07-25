#ifndef QCM_LINKNODES_H
#define QCM_LINKNODES_H

#include "qcm.h"
#include "qcm_nodeline.h"

namespace QCM
{
    class QCM_LinkNodes;
}

class QCM_LinkNodes : public QObject
{
    Q_OBJECT
public:
    QCM_LinkNodes(QGraphicsScene *parent);
    QCM_LinkNodes(QCM_Node *node, QGraphicsScene *parent);
    ~QCM_LinkNodes() {}

public slots:
    void addNode(QCM_Node *node);
    void addNode(qreal x, qreal y) { addNode(new QCM_Node(x, y)); }

private:
    void init();

    QList<QCM_Node *> m_nodes;
    QGraphicsScene *m_scene;
};

#endif // QCM_LINKNODES_H
