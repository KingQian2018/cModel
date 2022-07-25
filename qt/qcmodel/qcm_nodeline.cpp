#include "qcm_nodeline.h"

QCM_NodeLine::QCM_NodeLine(QGraphicsItem *parent) : QGraphicsLineItem(parent)
{
    init();
}

QCM_NodeLine::QCM_NodeLine(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent)
    : QGraphicsLineItem(x1, y1, x2, y2, parent)
{
    init();
}

QCM_NodeLine::QCM_NodeLine(qreal x1, qreal y1, QGraphicsItem *parent)
    : QCM_NodeLine(x1, y1, x1, y1, parent)

{
}

QCM_NodeLine::QCM_NodeLine(QCM_Node *_node1, QCM_Node *_node2, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    setLine(_node1->pos().x(), _node1->pos().y(),
            _node2->pos().x(), _node2->pos().y());
    m_node1 = _node1;
    m_node1->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
    m_node1->setAcceptHoverEvents(true);
    m_node2 = _node2;
    m_node2->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
    m_node2->setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemSendsGeometryChanges);
    setPen(QPen(Qt::black, 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

void QCM_NodeLine::init()
{
    m_node1 = new QCM_Node(this);
    m_node1->setPos(line().p1().x(), line().p1().y());
    m_node1->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
    m_node1->setAcceptHoverEvents(true);
    if (!(line().p1().x() == line().p2().x() && line().p2().y() == line().p1().y()))
    {
        m_node2 = new QCM_Node(this);
        m_node2->setPos(line().p2().x(), line().p2().y());
        m_node2->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
        m_node2->setAcceptHoverEvents(true);
    }

    setFlags(QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemSendsGeometryChanges);
    setPen(QPen(Qt::black, 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

QRectF QCM_NodeLine::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2;
    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(), line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void QCM_NodeLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (m_node2 == nullptr || m_node1->collidesWithItem(m_node2)) //判断图形项是否存在相交
        return;
    QPen pen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);

    qreal arrowsize = 10;

    QPointF p1 = QPointF(m_node2->pos().x(), m_node2->pos().y()); // item的左上方点  + item在场景的位置点（偏移得到左上点在场景中的位置）
    QPointF p2 = QPointF(m_node1->pos().x(), m_node1->pos().y());

    setLine(QLineF(p1, p2));

    double angle = std::atan2(-line().dy(), line().dx()); //反正切 [-PI,PI]
    QPointF arrowP1 = line().p1() +
                      QPointF(sin(angle + M_PI / 3) * arrowsize,  //计算对边
                              cos(angle + M_PI / 3) * arrowsize); //计算临边
    QPointF arrowP2 = line().p1() +
                      QPointF(sin(angle + M_PI - M_PI / 3) * arrowsize,
                              cos(angle + M_PI - M_PI / 3) * arrowsize);

    painter->drawLine(line());
}

#include "qcm_scene.h"
QVariant QCM_NodeLine::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
        auto _scene = (QCM_Scene *)scene();
        auto newPos = value.toPointF();
        int h = qRound(newPos.x() / _scene->grid());
        int pos = h * _scene->grid();
        newPos.setX(pos);

        h = qRound(newPos.y() / _scene->grid());
        pos = h * _scene->grid();
        newPos.setY(pos);
        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}
