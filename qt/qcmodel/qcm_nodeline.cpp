#include "qcm_nodeline.h"

QCM_NodeLine::QCM_NodeLine(QGraphicsItem *parent) : QGraphicsLineItem(parent)
{
    init();
}

void QCM_NodeLine::init()
{
    m_nodeOut = new QCM_Node(this);
    m_nodeOut->setPos(0, 0);
    m_nodeOut->setFlags(QGraphicsItem::ItemIsMovable |
                        QGraphicsItem::ItemSendsGeometryChanges);
    m_nodeOut->setAcceptHoverEvents(true);
    QCM_Node *nodeIn = new QCM_Node(this);
    nodeIn->setPos(100, 0);
    nodeIn->setFlags(QGraphicsItem::ItemIsMovable |
                     QGraphicsItem::ItemSendsGeometryChanges);
    nodeIn->setAcceptHoverEvents(true);
    addNodeIn(nodeIn);
    this->setZValue(m_nodeOut->zValue() - 1); //目的：让箭头后置
    setFlags(QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemSendsGeometryChanges);
    setPen(QPen(Qt::black, 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

void QCM_NodeLine::addNodeIn(QCM_Node *node)
{
    m_nodesIn.append(node);
}

QRectF QCM_NodeLine::boundingRect() const
{
    qreal extra = (this->pen().width() + 20) / 2;
    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(), line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void QCM_NodeLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    auto m_pendItem = (QCM_Node *)m_nodesIn.at(0);
    if (m_nodeOut->collidesWithItem(m_pendItem)) //判断图形项是否存在相交
        return;
    QPen pen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);

    qreal arrowsize = 10;

    QPointF p1 = QPointF(m_pendItem->pos().x(), m_pendItem->pos().y()); // item的左上方点  + item在场景的位置点（偏移得到左上点在场景中的位置）
    QPointF p2 = QPointF(m_nodeOut->pos().x(), m_nodeOut->pos().y());

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
