#include "qcm_nodeLine.h"
#include <QDebug>

QCM_NodeLine::QCM_NodeLine(QGraphicsScene *scene) : m_scene(scene)
{
}

QCM_NodeLine::~QCM_NodeLine()
{
    foreach (auto item, m_nodes)
    {
        delete item;
    }
}

inline void QCM_NodeLine::addNode(QCM_Node *node) { addNode(node, true); }

void QCM_NodeLine::addNode(QCM_Node *node, bool isAddNode)
{
    if (isAddNode)
    {
        m_scene->addItem(node);
        m_nodes.append(node);
    }

    if (m_lines.count() != 0)
    {
        QCM_Line *nl = new QCM_Line(m_lines.last()->toNode(), node);
        m_lines.append(nl);
        m_scene->addItem(nl);
    }
    else if (m_nodes.count() == 2)
    {
        QCM_Line *nl = new QCM_Line(m_nodes[0], m_nodes[1]);
        m_lines.append(nl);
        m_scene->addItem(nl);
    }
}

void QCM_NodeLine::addNodeOnLine(QCM_Node *node, QCM_Line *l)
{
    QCM_Node *_tonode = l->toNode();
    _tonode->removeLine(l);
    l->setToNode(node);
    node->addLine(l);
    QCM_Line *line1 = new QCM_Line(node, _tonode);
    m_lines.insert(0, line1);
    m_scene->addItem(line1);
}

void QCM_NodeLine::removeNode(QCM_Node *const node)
{
    qDebug() << QString("before move m_lines: %1, m_nodes: %2, m_scene.item: %3")
                    .arg(m_lines.count())
                    .arg(m_nodes.count())
                    .arg(m_scene->items().count());
    m_nodes.removeOne(node);
    // m_scene->removeItem(node);
    QSet<QCM_Line *> remLines = node->lines();
    foreach (auto l, remLines)
    {
        if (m_lines.contains(l))
        {
            m_lines.removeOne(l);
        }
    }
    delete node;
    qDebug() << QString("after move m_lines: %1, m_nodes: %2, m_scene.item: %3")
                    .arg(m_lines.count())
                    .arg(m_nodes.count())
                    .arg(m_scene->items().count());
}

void QCM_NodeLine::removeLine(QCM_Line *const line)
{
    m_lines.removeOne(line);
    delete line;
    QCM_Node *s = line->fromNode();
    QCM_Node *d = line->toNode();
    if (s->lines().count() == 0)
    {
        removeNode(s);
    }
    if (d->lines().count() == 0)
    {
        removeNode(d);
    }
}
