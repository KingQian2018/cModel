#include "qcm_node.h"
#include "qcm_line.h"

QCM_Line::QCM_Line(QCM_Node *sourceNode, QCM_Node *destNode)
    : m_sourceNode(sourceNode), m_destNode(destNode)
{
    m_sourceNode->setFlag(ItemIsMovable);
    m_sourceNode->setFlag(ItemSendsGeometryChanges);
    m_sourceNode->setAcceptHoverEvents(true);
    m_sourceNode->addLine(this);

    m_destNode->setFlag(ItemIsMovable);
    m_destNode->setFlag(ItemSendsGeometryChanges);
    m_destNode->setAcceptHoverEvents(true);
    m_destNode->addLine(this);

    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setFlag(ItemIsSelectable);
    setZValue(-1);

    adjust();
}

void QCM_Line::adjust()
{
    if (!m_sourceNode || !m_destNode)
        return;

    QLineF line(mapFromItem(m_sourceNode, 0, 0), mapFromItem(m_destNode, 0, 0));
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
    if (!m_sourceNode || !m_destNode)
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
    if (!m_sourceNode || !m_destNode)
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
        m_sourceNode->setPos(m_sourceNode->pos() + m_dltPos);
        m_destNode->setPos(m_destNode->pos() + m_dltPos);
        m_dltPos = value.toPointF();
    }
    return QGraphicsItem::itemChange(change, value);
}

void QCM_Line::addNode(QCM_Node *node)
{
    scene()->addItem(node);
    m_sourceNode->removeLine(this);
    scene()->addItem(new QCM_Line(m_sourceNode, node));
    m_sourceNode = node;
    node->addLine(this);
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
