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
    QCM_NodeLine(qreal x1, qreal y1, QGraphicsItem *parent = nullptr);
    QCM_NodeLine(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = nullptr);
    ~QCM_NodeLine() {}

    QCM_Node *node1() const { return m_node1; }
    void setNode1(QCM_Node *node)
    {
        if (m_node1 != nullptr)
        {
            delete m_node1;
        }
        m_node1 = node;
    }

    QCM_Node *node2() const { return m_node2; }
    void setNode2(QCM_Node *node)
    {
        if (m_node2 != nullptr)
        {
            delete m_node2;
        }
        m_node2 = node;
    }

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QCM_Node *m_node1, *m_node2 = nullptr;

    void init();
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // QCM_NODELINE_H
