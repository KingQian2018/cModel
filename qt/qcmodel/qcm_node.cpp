#include "qcm_node.h"
QCM_Node::QCM_Node(QGraphicsItem *parent) : QGraphicsItem(parent)
{
    init();
}

QCM_Node::~QCM_Node()
{
}

void QCM_Node::init()
{
    setRect(QRect(0, 0, 3, 3));
    setFlags(QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
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
    if (change == ItemPositionChange && scene())
    {
        auto _scene = (QCM_Scene *)scene();
        auto newPos = value.toPoint();
        int h = (newPos.x()) / _scene->grid();
        int l = (newPos.x()) % _scene->grid();
        int pos = (l < (_scene->grid() / 2) ? h : h + 1) * _scene->grid();
        newPos.setX(pos);

        h = (newPos.y()) / _scene->grid();
        l = (newPos.y()) % _scene->grid();
        pos = (l < (_scene->grid() / 2) ? h : h + 1) * _scene->grid();
        newPos.setY(pos);

        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}