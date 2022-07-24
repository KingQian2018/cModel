#include "qcm_nodeline.h"

QCM_NodeLine::QCM_NodeLine(QGraphicsItem *parent) : m_parent(parent)
{
    init();
}

void QCM_NodeLine::init()
{
    m_nodeOut = new QCM_Node(m_parent);
    m_nodeOut->setPos(0, 0);
    QCM_Node *nodeIn = new QCM_Node(m_nodeOut);
    nodeIn->setPos(100, 100);
    addNodeIn(nodeIn);
}

void QCM_NodeLine::addNodeIn(QCM_Node *node)
{
    QGraphicsLineItem *line = new QGraphicsLineItem(m_nodeOut);
    line->setLine(m_nodeOut->x(), m_nodeOut->y(),
                  node->x(), node->y());
    line->setData(QCM::ITEM_CLASS, QCM::NODE_LINE);
    qDebug() << "m_nodeOut: x" << m_nodeOut->x() << " y:" << m_nodeOut->y();
    qDebug() << "node: x" << node->x() << " y:" << node->y();
    m_nodesIn.append(node);
}
