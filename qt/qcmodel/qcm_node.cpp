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
    setRect(QRect(0, 0, 3, 3));
    setData(QCM::ItemKey_TYPE::ITEM_CLASS, QCM::NODE);
}

QRectF QCM_Node::boundingRect() const
{
    QRectF _rect = m_rect;
    _rect.adjust(-3, -3, 3, 3);
    return _rect;
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
            if (_item->data(QCM::ItemKey_TYPE::ITEM_CLASS) == QCM::NODE)
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

#include "qcm_scene.h"
QVariant QCM_Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange)
    {
        return QCM::AlignToGrid(value.toPointF());
    }
    return QGraphicsItem::itemChange(change, value);
}