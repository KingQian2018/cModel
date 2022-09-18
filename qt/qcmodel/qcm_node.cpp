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
    foreach (QCM_Line *line, m_lines)
    {
        delete line;
        line = NULL;
    }
}

void QCM_Node::init()
{
    setCacheMode(DeviceCoordinateCache);
}

QRectF QCM_Node::boundingRect() const
{
    const int Margin = 1;
    return outlinkRect().adjusted(-Margin, -Margin, Margin, Margin);
}

void QCM_Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (m_lines.count() >= 3)
    {
        QPen pen(Qt::red);
        pen.setWidth(4);
        painter->setPen(pen);
        painter->drawPoint(0, 0);
    }
    else
    {
        QPen pen(Qt::blue);
        pen.setWidth(4);
        painter->setPen(pen);
        painter->drawPoint(0, 0);
    }
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
        foreach (auto line, m_lines)
        {
            line->trackNodes();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void QCM_Node::addLine(QCM_Line *line) { m_lines << line; }

void QCM_Node::removeLine(QCM_Line *line) { m_lines.removeOne(line); }

QPainterPath QCM_Node::shape() const
{
    QRectF rect = outlinkRect();
    QPainterPath path;
    path.addEllipse(rect);
    return path;
}

QRectF QCM_Node::outlinkRect() const
{
    QRectF rect(-2, -2, 4, 4);
    const int Padding = 8;
    rect.adjust(-Padding, -Padding, Padding, Padding);
    rect.translate(-rect.center());
    return rect;
}
