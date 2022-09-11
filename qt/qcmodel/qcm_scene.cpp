#include "qcm_scene.h"

QCM_Scene::QCM_Scene(QObject *parent) : QGraphicsScene(parent)
{
    setSceneRect(-100, -100, m_parperSize.width + 100, m_parperSize.height + 100);
    QAction *copyAction = m_menu.addAction("复制");
    QAction *deleteAction = m_menu.addAction("删除");

    connect(copyAction, SIGNAL(triggered()), this, SLOT(copyModelEvent()));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteModelEvent()));

    QCM_Node *node1, *node2;
    node1 = new QCM_Node();
    node2 = new QCM_Node();
    m_preLine = new QCM_PreLine(node1, node2);
    addItem(m_preLine);
    addItem(node1);
    addItem(node2);
    m_preLine->hide();
    node1->hide();
    node2->hide();
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
    pen.setWidth(2);
    painter->setPen(pen);

    //绘制网格点
    for (int i = top + QCM::Grid; i <= bottom; i += QCM::Grid)
    {
        for (int j = left + QCM::Grid; j < right; j += QCM::Grid)
        {
            painter->drawPoint(j, i);
        }
    }
}

void QCM_Scene::setParperSize(QCM::PaperSize_s size)
{
    m_parperSize = size;
    setSceneRect(-100, -100, m_parperSize.width + 100, m_parperSize.height + 100);
}

#include <QDebug>
void QCM_Scene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    auto items = selectedItems();
    if (items.count() != 0 && items.at(0)->type() == QCM::MODEL)
    {
        m_menu.exec(event->screenPos());
    }
}

void QCM_Scene::copyModelEvent(void)
{
    auto items = selectedItems();
    if (items.count() != 0)
    {
        QCModel *model = (QCModel *)items.at(0);
        qDebug() << "Add" << QString::number(model->ID());
        addItem(model->copy());
    }
}

void QCM_Scene::deleteModelEvent(void)
{
    auto items = selectedItems();
    if (items.count() != 0)
    {
        QCModel *model = (QCModel *)items.at(0);
        qDebug() << "Del" << QString::number(model->ID());
        removeItem(model);
    }
}

void QCM_Scene::linkMove(bool v, QPointF pos)
{
    if (v)
    {
        if (!m_preLine->isVisible() && m_linkCnt != 0)
        {
            m_preLine->show();
            m_preLine->sourceNode()->show();
            m_preLine->destNode()->show();
        }
        m_preLine->destNode()->setPos(pos);
    }
    else
    {
        if (m_preLine->isVisible())
        {
            m_preLine->hide();
            m_preLine->sourceNode()->hide();
            m_preLine->destNode()->hide();
        }
        m_linkCnt = 0;
    }
}

void QCM_Scene::linkClicked()
{
    if (m_linkCnt == 0)
    {
        m_lines.append(new QCM_NodeLine(this));
    }
    QCM_Node *node = new QCM_Node(m_preLine->destNode()->pos());
    m_lines.last()->addNode(node);
    m_linkCnt++;
    m_preLine->sourceNode()->setPos(m_preLine->destNode()->pos());
}

void QCM_Scene::deleteLines()
{
    foreach (auto line, m_lines)
    {
        qDebug() << "remove line";
        foreach (auto node, line->nodes())
        {
            if (node->isSelected())
            {
                line->removeNode(node);
                qDebug() << "remove node";
            }
        }
    }
}
