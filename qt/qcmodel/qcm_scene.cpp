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

    qreal left = rect.left();
    qreal right = rect.right();
    qreal top = rect.top();
    qreal bottom = rect.bottom();

    //边界值调整
    left = (left / m_grid) * m_grid;
    right = (right / m_grid) * m_grid;
    top = (top / m_grid) * m_grid;
    bottom = (bottom / m_grid) * m_grid;

    left = left < 0 ? 0 : left;
    right = right > m_parperSize.width ? m_parperSize.width : right;
    top = top < 0 ? 0 : top;
    bottom = bottom > m_parperSize.height ? m_parperSize.height : bottom;

    pen.setColor(QColor(0, 0, 0));
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    //绘制坐标轴
    painter->drawLine(left, 0, right, 0);
    painter->drawLine(0, top, 0, bottom);
    painter->drawLine(left, m_parperSize.height, right, m_parperSize.height);
    painter->drawLine(m_parperSize.width, top, m_parperSize.width, bottom);

    pen.setColor(QColor(60, 60, 60));
    pen.setWidth(0);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    //绘制横线
    //绘制Y轴正半轴
    for (int i = 0; i >= top; i -= m_grid)
    {
        painter->drawLine(left, i, right, i);
    }
    //绘制Y轴负半轴
    for (int i = 0; i <= bottom; i += m_grid)
    {
        painter->drawLine(left, i, right, i);
    }

    //绘制竖线
    //绘制X轴正半轴
    for (int i = 0; i <= right; i += m_grid)
    {
        painter->drawLine(i, top, i, bottom);
    }
    //绘制X轴负半轴
    for (int i = 0; i >= left; i -= m_grid)
    {
        painter->drawLine(i, top, i, bottom);
    }
}

void QCM_Scene::setParperSize(QCM::PaperSize_s size)
{
    m_parperSize = size;
    setSceneRect(-100, -100, m_parperSize.width + 100, m_parperSize.height + 100);
}
