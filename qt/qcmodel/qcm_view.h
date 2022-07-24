#ifndef QCM_VIEW_H
#define QCM_VIEW_H

#include <QGraphicsView>

namespace QCM
{
    class QCM_View;
}

class QCM_View : public QGraphicsView
{
    Q_OBJECT
public:
    QCM_View(QWidget *parent = nullptr) : QGraphicsView(parent) {}
    QCM_View(QGraphicsScene *scene, QWidget *parent = nullptr) : QGraphicsView(scene, parent) {}
    ~QCM_View() {}

private:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool isMousePressed = false;
    QPoint posAnchor;

signals:
    void posChanged(QPointF pos);
};

#endif // QCM_VIEW_H
