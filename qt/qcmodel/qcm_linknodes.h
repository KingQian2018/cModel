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
    
    bool isEdited() const { return m_isEdited; }
    void setIsEdited(bool v) { m_isEdited = v; }

public slots:
    void addNode(QCM_Node *node);
    void addNode(qreal x, qreal y) { addNode(new QCM_Node(x, y)); }
    void addNode(QPointF point) { addNode(point.x(), point.y()); }
    void movePreNode(QPointF pos)
    {
        if (m_preNode2 != 0)
        {
            m_preNode2->setPos(pos);
        }
    }

private:
    void init();

    QList<QCM_Node *> m_nodes;
    QCM_Node *m_preNode1 = nullptr;
    QCM_Node *m_preNode2 = nullptr;
    QGraphicsScene *m_scene;
    bool m_isEdited = true;
};

#endif // QCM_LINKNODES_H
