#ifndef QCM_NODELINE_H
#define QCM_NODELINE_H

#include "qcm.h"
#include "qcm_node.h"

namespace QCM
{
    class QCM_NodeLine;
}

class QCM_NodeLine : public QGraphicsItem
{
public:
    QCM_NodeLine(const QRectF &rect, QGraphicsItem *parent = nullptr);
    QCM_NodeLine(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr);
    QCM_NodeLine(QGraphicsItem *parent = nullptr);
    ~QCM_NodeLine() {}
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

    void setPen(const QPen &pen);
    QPen pen() const { return m_pen; }

    QRectF rect() const;
    void setRect(const QRectF &rect);
    inline void setRect(qreal x, qreal y, qreal w, qreal h);

    QCM_Node *nodeOut() const { return m_nodeOut; }
    void setNodeOut(QCM_Node *node) { m_nodeOut = node; }

private:
    uint m_x1 = 0, m_y1 = 0;
    uint m_x2 = 100, m_y2 = 100;
    QPen m_pen;
    mutable QRectF m_boundingRect;
    QRectF m_rect;
    QCM_Node *m_nodeOut;
    QList<QCM_Node *> m_nodesIn;

    void init();
};

#endif // QCM_NODELINE_H
