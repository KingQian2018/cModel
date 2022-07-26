#include "qcm_view.h"
#include "qcm_scene.h"

#include <QMouseEvent>
#include <QDebug>

void QCM_View::wheelEvent(QWheelEvent *event)
{
    int wheelValue = event->angleDelta().y();
    double _ratio = (double)wheelValue / (double)1200 + 1;
    scale(_ratio, _ratio);
}

void QCM_View::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    if (scene() == nullptr)
    {
        qDebug() << "The scene is null";
        return;
    }
    if (event->button() == Qt::RightButton)
    {
        posAnchor = event->pos();
        isMousePressed = true;
    }
    else if (event->button() == Qt::LeftButton)
    {
        if (m_isSetLink)
        {
            auto _point = mapToScene(event->pos());
            emit setLinked(_point);
        }
    }
}

void QCM_View::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    if (isMousePressed)
    {
        //获取每次鼠标在场景坐标系下的平移量
        QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(posAnchor);
        //调用平移方法
        //如果是在缩放之后，调用的平移方法，那么平移量先要乘上缩放比，transform是view的变换矩阵，m11可以用为缩放比
        mouseDelta *= this->transform().m11();

        //修改锚点，调用缩放方法
        this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        this->centerOn(this->mapToScene(QPoint(this->viewport()->rect().width() / 2 - mouseDelta.x(),
                                               this->viewport()->rect().height() / 2 - mouseDelta.y())));
        this->setTransformationAnchor(QGraphicsView::AnchorViewCenter);

        // posAnchor是MyGraphicsView的私有成员变量，用以记录每次的事件结束时候的鼠标位置
        posAnchor = event->pos();
        if (cursor() != Qt::ClosedHandCursor)
        {
            setCursor(Qt::ClosedHandCursor);
        }
    }
    else if (m_isSetLink)
    {
        if (cursor() != Qt::CrossCursor)
        {
            setCursor(Qt::CrossCursor);
        }
        emit movePreLinked(mapToScene(event->pos()));
    }
    else
    {
        if (cursor() != Qt::ArrowCursor)
        {
            setCursor(Qt::ArrowCursor);
        }
    }
    emit posChanged(mapToScene(event->pos()));
}

void QCM_View::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    if (event->button() == Qt::RightButton)
    {
        if (m_isSetLink)
        {
            m_isSetLink = false;
        }
        isMousePressed = false;
    }
}