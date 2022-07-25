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
    if (!m_isEdited)
    {
        return;
    }
    node->setPos(QCM::AlignToGrid(node->pos()));

    m_preNode1 = new QCM_Node(node);
    m_preNode1->setPos(node->pos());
    m_preNode2 = new QCM_Node(node);
    m_preNode2->setPos(node->pos());
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
    auto lines = m_scene->items();
    foreach (auto l, lines)
    {
        if (l->data(QCM::ITEM_CLASS) == QCM::PRE_NODE_LINE)
        {
            m_scene->removeItem(l);
            break;
        }
    }
    m_scene->addItem(new QCM_PreNodeLine(m_preNode1, m_preNode2));
}

void QCM_LinkNodes::init()
{
}
