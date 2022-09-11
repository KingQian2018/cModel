#include "qcm_nodeLine.h"

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

void QCM_NodeLine::addNode(QCM_Node *node)
{
    m_scene->addItem(node);
    m_nodes.append(node);

    if (m_nodes.count() > 1)
    {
        m_scene->addItem(new QCM_Line(m_nodes.at(m_nodes.count() - 2), node));
    }
}

void QCM_NodeLine::removeNode(QCM_Node *const node)
{
    m_nodes.removeOne(node);
    delete node;
}
