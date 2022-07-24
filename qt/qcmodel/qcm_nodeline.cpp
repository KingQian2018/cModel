#include "qcm_nodeline.h"

QCM_NodeLine::QCM_NodeLine(const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    init();
    setRect(rect);
}

QCM_NodeLine::QCM_NodeLine(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    init();
    setRect(x, y, w, h);
}

QCM_NodeLine::QCM_NodeLine(QGraphicsItem *parent) : QGraphicsItem(parent)
{
    init();
    setRect(QRect(m_x1, m_y1, m_x2, m_y2));
}

QRectF QCM_NodeLine::boundingRect() const
{
    if (m_boundingRect.isNull())
    {
        m_boundingRect = m_rect;
    }
    m_boundingRect.adjust(-10, -10, 10, 10);
    return m_boundingRect;
}

void QCM_NodeLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(m_pen);
    painter->drawEllipse(m_x1, m_y1, 3, 3);
    painter->drawEllipse(m_x2, m_y2, 3, 3);
    painter->setPen(QPen(Qt::black));
    painter->drawLine(m_x1, m_y1, m_x2, m_y2);
}

void QCM_NodeLine::setPen(const QPen &pen)
{
    m_pen = pen;
}

QRectF QCM_NodeLine::rect() const
{
    return m_rect;
}

void QCM_NodeLine::setRect(const QRectF &rect)
{
    if (m_rect == rect)
        return;

    prepareGeometryChange();
    m_rect = rect;
    m_boundingRect = QRectF();
    update();
}

inline void QCM_NodeLine::setRect(qreal ax, qreal ay, qreal w, qreal h)
{
    setRect(QRectF(ax, ay, w, h));
}

void QCM_NodeLine::init()
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    m_pen.setColor(Qt::red);
    m_pen.setWidth(3);
    setData(QCM::ITEM_CLASS, QCM::NODE_LINE);
}
