#include "qcm_node.h"
#include "qcm_line.h"

QCM_Line::QCM_Line(QCM_Node *sourceNode, QCM_Node *destNode)
    : m_fromNode(sourceNode), m_toNode(destNode)
{
    m_fromNode->setFlag(ItemIsMovable);
    m_fromNode->setFlag(ItemIsSelectable);
    m_fromNode->setFlag(ItemSendsGeometryChanges);
    m_fromNode->setAcceptHoverEvents(true);
    m_fromNode->addLine(this);

    m_toNode->setFlag(ItemIsMovable);
    m_toNode->setFlag(ItemIsSelectable);
    m_toNode->setFlag(ItemSendsGeometryChanges);
    m_toNode->setAcceptHoverEvents(true);
    m_toNode->addLine(this);

    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setFlag(ItemIsSelectable);
    setZValue(-1);

    trackNodes();
}

QCM_Line::~QCM_Line()
{
    m_fromNode->removeLine(this);
    m_toNode->removeLine(this);
}

void QCM_Line::trackNodes()
{
    if (!m_fromNode || !m_toNode)
        return;

    QLineF line(mapFromItem(m_fromNode, 0, 0), mapFromItem(m_toNode, 0, 0));
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
    if (!m_fromNode || !m_toNode)
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
    if (!m_fromNode || !m_toNode)
        return;

    QLineF line(m_sourcePoint, m_destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    if (option->state & QStyle::State_Selected)
    {
        painter->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    else
    {
        painter->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    painter->drawLine(line);
}

QPainterPath QCM_Line::shape() const
{
    QLineF line(m_sourcePoint, m_destPoint);
    QPainterPath path;
    path.moveTo(m_sourcePoint);
    path.lineTo(m_destPoint);
    QPainterPathStroker stroker;
    stroker.setWidth(10);
    stroker.setJoinStyle(Qt::MiterJoin);
    stroker.setCapStyle(Qt::RoundCap);
    stroker.setDashPattern(Qt::DashLine);
    return stroker.createStroke(path);
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
        m_fromNode->setPos(m_fromNode->pos() + m_dltPos);
        m_toNode->setPos(m_toNode->pos() + m_dltPos);
        m_dltPos = value.toPointF();
    }
    return QGraphicsItem::itemChange(change, value);
}

void QCM_Line::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void QCM_Line::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void QCM_PreLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!sourceNode() || !destNode())
        return;

    QLineF line(sourcePoint(), destPoint());
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    painter->setPen(QPen(Qt::black, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);
}
