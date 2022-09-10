#include "qcm_line.h"
#include "qcm_node.h"

QCM_Node::QCM_Node(QGraphicsItem *parent) : QGraphicsItem(parent)
{
    init();
}
QCM_Node::QCM_Node(qreal x, qreal y, QGraphicsItem *parent) : QCM_Node(parent)
{
    setPos(x, y);
}

QCM_Node::QCM_Node(QPointF pos, QGraphicsItem *parent)
    : QCM_Node(pos.x(), pos.y(), parent)
{
}

QCM_Node::~QCM_Node()
{
}

void QCM_Node::init()
{
    setCacheMode(DeviceCoordinateCache);
}

QRectF QCM_Node::boundingRect() const
{
    return shape().boundingRect();
}

void QCM_Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPen pen(Qt::red);
    if (!scene()->collidingItems(this).isEmpty())
    {
        foreach (auto _item, scene()->collidingItems(this))
        {
            if (_item->type() == QCM::NODE + UserType)
            {
                pen.setColor(Qt::blue);
            }
        }
    }
    pen.setWidth(4);
    painter->setPen(pen);
    painter->drawPoint(0, 0);
}

void QCM_Node::setRect(const QRectF &rect)
{
    if (m_rect == rect)
        return;

    prepareGeometryChange();
    m_rect = rect;
    update();
}

QVariant QCM_Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange)
    {
        return QCM::AlignToGrid(value.toPointF());
    }
    else if (change == ItemPositionHasChanged)
    {
        for (auto line : qAsConst(m_lines))
        {
            line->adjust();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void QCM_Node::addLine(QCM_Line *line)
{
    m_lines << line;
    line->adjust();
}

void QCM_Node::removeLine(QCM_Line *line)
{
    auto idx = m_lines.indexOf(line);
    if (idx != -1)
    {
        m_lines.removeAt(idx);
        line->adjust();
    }
}

QPainterPath QCM_Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-2, -2, 4, 4);
    return path;
}

void QCM_Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void QCM_Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
