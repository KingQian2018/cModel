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

void QCM_Scene::addLinkMove(bool v, QPointF pos)
{
    if (v)
    {
        if (!m_preLine->isVisible() && m_linkCnt != 0)
        {
            m_preLine->show();
            m_preLine->fromNode()->show();
            m_preLine->toNode()->show();
        }
        m_preLine->toNode()->setPos(pos);
    }
    else
    {
        if (m_preLine->isVisible())
        {
            m_preLine->hide();
            m_preLine->fromNode()->hide();
            m_preLine->toNode()->hide();
        }
        m_linkCnt = 0;
    }
}

void QCM_Scene::addLinkClicked()
{
    QCM_Node *_node = new QCM_Node(m_preLine->toNode()->pos());
    QCM_NodeLine *_nodeLine = NULL;
    foreach (auto nodeLine, m_nodeLines)
    {
        foreach (auto n, nodeLine->nodes())
        {
            if (n->collidesWithItem(_node))
            {
                delete _node;
                _node = n;
                _nodeLine = nodeLine;
                goto over;
            }
        }
        if (_nodeLine == NULL)
        {
            foreach (auto l, nodeLine->lines())
            {
                if (l->collidesWithItem(_node))
                {
                    nodeLine->addNodeOnLine(_node, l);
                    _nodeLine = nodeLine;
                }
            }
        }
    }
    if (m_linkCnt == 0 && _nodeLine == NULL)
    {
        _nodeLine = new QCM_NodeLine(this);
        m_nodeLines.append(_nodeLine);
    }
    if (_nodeLine == NULL)
    {
        _nodeLine = m_nodeLines.last();
    }
over:
    _nodeLine->addNode(_node);
    m_linkCnt++;
    m_preLine->fromNode()->setPos(m_preLine->toNode()->pos());
}

void QCM_Scene::deleteLines()
{
    qDebug() << QString("before move m_nodeLines: %1")
                    .arg(m_nodeLines.count());
    foreach (auto nodeLine, m_nodeLines)
    {
        foreach (auto line, nodeLine->lines())
        {
            if (line->isSelected())
            {
                nodeLine->removeLine(line);
            }
        }
        if (nodeLine->lines().count() == 0)
        {
            m_nodeLines.removeOne(nodeLine);
            delete nodeLine;
            nodeLine = NULL;
        }
    }
    qDebug() << QString("after move m_nodeLines: %1")
                    .arg(m_nodeLines.count());
}
