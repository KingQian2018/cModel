#ifndef QCM_NODE_H
#define QCM_NODE_H

#include "qcm.h"

namespace QCM
{
    class QCM_Node;
}

class QCM_Node : public QGraphicsItem
{
public:
    QCM_Node(QGraphicsItem *parent = nullptr);
    ~QCM_Node();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

    uint x() const { return m_x; }
    uint y() const { return m_y; }

    void setX(uint x) { m_x = x; }
    void setY(uint y) { m_y = y; }

private:
    QRectF m_rect;
    uint m_x, m_y;

    void setRect(const QRectF &rect);
    void init();
};

#endif // QCM_NODE_H
