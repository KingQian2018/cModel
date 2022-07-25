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

private:
    QRectF m_rect;

    void setRect(const QRectF &rect);
    void init();
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) { setCursor(Qt::DragMoveCursor); }
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) { setCursor(Qt::ArrowCursor); }
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // QCM_NODE_H
