
#ifndef QCM_LINE_H
#define QCM_LINE_H

#include "qcm.h"

class QCM_Node;

class QCM_Line : public QGraphicsItem
{
public:
    QCM_Line(QCM_Node *sourceNode, QCM_Node *destNode);
    ~QCM_Line() {}

    enum
    {
        Type = UserType + QCM::LINE
    };
    int type() const override { return Type; }
    void adjust();

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    QCM_Node *m_source, *m_dest;
    QPointF m_dltPos;
    QPointF m_sourcePoint;
    QPointF m_destPoint;
};

#endif // QCM_LINE_H
