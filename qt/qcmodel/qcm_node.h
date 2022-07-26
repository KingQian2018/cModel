#ifndef QCM_NODE_H
#define QCM_NODE_H

#include "qcm.h"

class QCM_Line;

class QCM_Node : public QGraphicsItem
{
public:
    QCM_Node(QGraphicsItem *parent = nullptr);
    QCM_Node(qreal x, qreal y, QGraphicsItem *parent = nullptr);
    QCM_Node(QPointF pos, QGraphicsItem *parent = nullptr);

    ~QCM_Node();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
    QPainterPath shape() const;
    enum
    {
        Type = UserType + QCM::NODE
    };
    int type() const override { return Type; }

    void addLine(QCM_Line *line);
    QVector<QCM_Line *> lines() const { return m_lines; }
    void adjust();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) { setCursor(Qt::DragMoveCursor); }
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) { setCursor(Qt::ArrowCursor); }
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QRectF m_rect;
    QVector<QCM_Line *> m_lines;

    void setRect(const QRectF &rect);
    void init();
};

#endif // QCM_NODE_H
