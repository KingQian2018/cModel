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
    setFlags(QGraphicsItem::ItemIsMovable);
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
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawEllipse(0, 0, 3, 3);
}

void QCM_Node::setRect(const QRectF &rect)
{
    if (m_rect == rect)
        return;

    prepareGeometryChange();
    m_rect = rect;
    update();
}
