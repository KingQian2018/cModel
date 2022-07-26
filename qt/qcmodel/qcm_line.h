
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
    QCM_Node *sourceNode() const { return m_source; }
    QCM_Node *destNode() const { return m_dest; }

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    QPointF sourcePoint() const { return m_sourcePoint; }
    QPointF destPoint() const { return m_destPoint; }

private:
    QCM_Node *m_source, *m_dest;
    QPointF m_dltPos;
    QPointF m_sourcePoint;
    QPointF m_destPoint;
};

class QCM_PreLine : public QCM_Line
{
public:
    QCM_PreLine(QCM_Node *sourceNode, QCM_Node *destNode) : QCM_Line(sourceNode, destNode) {}
    ~QCM_PreLine() {}
    enum
    {
        Type = UserType + QCM::PRE_LINE
    };
    int type() const override { return Type; }
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};
#endif // QCM_LINE_H
