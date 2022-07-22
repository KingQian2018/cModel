#include "qcm_view.h"
#include "qcm_scene.h"

#include <QMouseEvent>
#include <QDebug>

void QCM_View::wheelEvent(QWheelEvent *event)
{
    int wheelValue = event->angleDelta().y();
    double _ratio = (double)wheelValue / (double)1200 + 1;
    scale(_ratio, _ratio);
    ratio += _ratio - 1;
    qDebug() << QString("ratio: %1").arg(ratio);
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
        // 记录鼠标按下时的中心点坐标
        // centerAnchor = mapToScene(event->pos()) - event->pos() + QPointF(width() / 2, height() / 2);
        centerAnchor = event->pos();
        qDebug() << QString("centerAnchor %1 %2").arg(centerAnchor.x()).arg(centerAnchor.y());
        // 记录当前鼠标在view中的位置，用来在mouseMove事件中计算偏移
        // 此处不将view坐标转换成scene坐标的原因是优化性能，在move的过程中会产生抖动
        posAnchor = event->pos();
        isMousePressed = true;
        setCursor(Qt::ClosedHandCursor);
    }
}

void QCM_View::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    QPointF offsetPos = event->pos() - posAnchor;
    if (isMousePressed)
    {
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        centerOn(centerAnchor - offsetPos);
    }
    emit posChanged(mapToScene(event->pos()));
}

void QCM_View::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    if (event->button() == Qt::RightButton)
    {
        isMousePressed = false;
        setCursor(Qt::ArrowCursor);
    }
}