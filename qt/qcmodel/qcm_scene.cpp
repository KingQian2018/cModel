#include "qcm_scene.h"

#include <QtGui>

QCM_Scene::QCM_Scene(QObject *parent) : QGraphicsScene(parent)
{
    setSceneRect(-100, -100, m_parperSize.width + 100, m_parperSize.height + 100);
}

void QCM_Scene::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    QPen pen;

    //边界值调整
    qreal left = 0;
    qreal right = m_parperSize.width;
    qreal top = 0;
    qreal bottom = m_parperSize.height;

    pen.setColor(QColor(0, 0, 0));
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    //绘制坐标轴
    painter->drawLine(left, top, right, top);
    painter->drawLine(left, top, left, bottom);
    painter->drawLine(left, bottom, right, bottom);
    painter->drawLine(right, top, right, bottom);

    pen.setColor(QColor(60, 60, 60));
    pen.setWidth(0);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    //绘制横线
    for (int i = top; i <= bottom; i += m_grid)
    {
        painter->drawLine(left, i, right, i);
    }

    //绘制竖线
    for (int i = left; i <= right; i += m_grid)
    {
        painter->drawLine(i, top, i, bottom);
    }
}

void QCM_Scene::setParperSize(QCM::PaperSize_s size)
{
    m_parperSize = size;
    setSceneRect(-100, -100, m_parperSize.width + 100, m_parperSize.height + 100);
}
