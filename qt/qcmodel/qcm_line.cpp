#include "qcm_node.h"
#include "qcm_line.h"

QCM_Line::QCM_Line(QCM_Node *sourceNode, QCM_Node *destNode)
    : m_source(sourceNode), m_dest(destNode)
{
    m_source->setFlag(ItemIsMovable);
    m_source->setFlag(ItemSendsGeometryChanges);
    m_source->setAcceptHoverEvents(true);
    m_source->addLine(this);

    m_dest->setFlag(ItemIsMovable);
    m_dest->setFlag(ItemSendsGeometryChanges);
    m_dest->setAcceptHoverEvents(true);
    m_dest->addLine(this);

    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setZValue(-1);

    adjust();
}

void QCM_Line::adjust()
{
    if (!m_source || !m_dest)
        return;

    QLineF line(mapFromItem(m_source, 0, 0), mapFromItem(m_dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.))
    {
        QPointF edgeOffset((line.dx() * 2) / length, (line.dy() * 2) / length);
        m_sourcePoint = line.p1() + edgeOffset;
        m_destPoint = line.p2() - edgeOffset;
    }
    else
    {
        m_sourcePoint = m_destPoint = line.p1();
    }
}

QRectF QCM_Line::boundingRect() const
{
    if (!m_source || !m_dest)
        return QRectF();

    qreal penWidth = 3;
    qreal extra = (penWidth + 20) / 2.0;

    return QRectF(m_sourcePoint, QSizeF(m_destPoint.x() - m_sourcePoint.x(),
                                        m_destPoint.y() - m_sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void QCM_Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!m_source || !m_dest)
        return;

    QLineF line(m_sourcePoint, m_destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    painter->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);
}

QVariant QCM_Line::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange)
    {
        return QCM::AlignToGrid(value.toPointF());
    }
    else if (change == ItemPositionHasChanged)
    {
        m_dltPos = value.toPointF() - m_dltPos;
        m_source->setPos(m_source->pos() + m_dltPos);
        m_dest->setPos(m_dest->pos() + m_dltPos);
        m_dltPos = value.toPointF();
    }
    return QGraphicsItem::itemChange(change, value);
}