#include "qcm_linknodes.h"

QCM_LinkNodes::QCM_LinkNodes(QGraphicsScene *parent)
    : m_scene(parent)
{
    init();
}

QCM_LinkNodes::QCM_LinkNodes(QCM_Node *node, QGraphicsScene *parent)
    : QCM_LinkNodes(parent)
{
    addNode(node);
}

void QCM_LinkNodes::addNode(QCM_Node *node)
{
    foreach (auto _node, m_nodes)
    {
        if (_node->collidesWithItem(node))
        {
            return;
        }
    }
    if (m_nodes.count() != 0)
    {
        m_scene->addItem(new QCM_NodeLine(m_nodes.last(), node));
    }
    m_nodes.append(node);
}

void QCM_LinkNodes::init()
{
}
