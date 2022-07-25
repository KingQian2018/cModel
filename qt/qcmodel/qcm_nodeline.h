#ifndef QCM_NODELINE_H
#define QCM_NODELINE_H

#include "qcm.h"
#include "qcm_node.h"

namespace QCM
{
    class QCM_NodeLine;
}

class QCM_NodeLine : public QGraphicsLineItem
{
public:
    QCM_NodeLine(QGraphicsItem *parent = nullptr);
    ~QCM_NodeLine() {}

    QCM_Node *nodeOut() const { return m_nodeOut; }
    void setNodeOut(QCM_Node *node) { m_nodeOut = node; }
    void addNodeIn(QCM_Node *node);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QCM_Node *m_nodeOut;
    QList<QCM_Node *> m_nodesIn;

    void init();
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // QCM_NODELINE_H
