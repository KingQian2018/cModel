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
        item = nullptr;
    }
}

void QCM_NodeLine::addNode(QCM_Node *node)
{

    if (m_lines.count() != 0)
    {
        QCM_Line *lstLine = m_lines.last();
        QCM_Node *fNode = lstLine->fromNode();
        QCM_Node *tNode = lstLine->toNode();
        if ((tNode->lines().count() == 1) &&
            ((fNode->pos().x() == node->pos().x()) ||
             (fNode->pos().y() == node->pos().y())))
        {
            tNode->setPos(node->pos());
            tNode->update();
            delete node;
            node = nullptr;
        }
        else
        {
            if (!m_nodes.contains(node))
            {
                m_scene->addItem(node);
                m_nodes.append(node);
            }
            QCM_Line *nl = new QCM_Line(tNode, node);
            m_lines.append(nl);
            m_scene->addItem(nl);
        }
    }
    else
    {
        m_scene->addItem(node);
        m_nodes.append(node);
        if (m_nodes.count() == 2)
        {
            QCM_Line *nl = new QCM_Line(m_nodes[0], m_nodes[1]);
            m_lines.append(nl);
            m_scene->addItem(nl);
        }
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

void QCM_NodeLine::removeNodeOnLine(QCM_Node *node)
{
    QCM_Line *l1 = node->lines()[0];
    QCM_Line *l2 = node->lines()[1];
    QCM_Node *n1 = l1->fromNode();
    QCM_Node *n2 = l1->toNode();
    QCM_Node *n3 = l2->fromNode();
    QCM_Node *n4 = l2->toNode();
    if (n1 == n3)
    {
        if ((n2->pos().x() == n4->pos().x()) || (n2->pos().y() == n4->pos().y()))
        {
            n1->removeLine(l1);
            n1->removeLine(l2);
            removeLine(l2);
            l1->setFromNode(n4);
            n4->addLine(l1);
        }
    }
    else if (n1 == n4)
    {
        if ((n2->pos().x() == n3->pos().x()) || (n2->pos().y() == n3->pos().y()))
        {
            n1->removeLine(l1);
            n1->removeLine(l2);
            removeLine(l2);
            l1->setFromNode(n3);
            n3->addLine(l1);
        }
    }
    else if (n2 == n3)
    {
        if ((n1->pos().x() == n4->pos().x()) || (n1->pos().y() == n4->pos().y()))
        {
            n2->removeLine(l1);
            n2->removeLine(l2);
            removeLine(l2);
            l1->setToNode(n4);
            n4->addLine(l1);
        }
    }
    else if (n2 == n4)
    {
        if ((n1->pos().x() == n3->pos().x()) || (n1->pos().y() == n3->pos().y()))
        {
            n2->removeLine(l1);
            n2->removeLine(l2);
            removeLine(l2);
            l1->setToNode(n3);
            n3->addLine(l1);
        }
    }
}

void QCM_NodeLine::removeNode(QCM_Node *node)
{
    qDebug() << QString("before move m_lines: %1, m_nodes: %2, m_scene.item: %3")
                    .arg(m_lines.count())
                    .arg(m_nodes.count())
                    .arg(m_scene->items().count());
    m_nodes.removeOne(node);
    QList<QCM_Line *> remLines = node->lines();
    foreach (auto l, remLines)
    {
        if (m_lines.contains(l))
        {
            m_lines.removeOne(l);
        }
    }
    delete node;
    node = nullptr;
    qDebug() << QString("after move m_lines: %1, m_nodes: %2, m_scene.item: %3")
                    .arg(m_lines.count())
                    .arg(m_nodes.count())
                    .arg(m_scene->items().count());
}

void QCM_NodeLine::removeLine(QCM_Line *line)
{
    QCM_Node *s = line->fromNode();
    QCM_Node *d = line->toNode();
    m_lines.removeOne(line);
    delete line;
    line = nullptr;
    if (s != nullptr && s->lines().count() == 0)
    {
        qDebug() << "Remove sNode";
        removeNode(s);
    }
    else if (s->lines().count() == 2)
    {
        removeNodeOnLine(s);
    }

    if (d != nullptr && d->lines().count() == 0)
    {
        qDebug() << "Remove dNode";
        removeNode(d);
    }
    else if (d->lines().count() == 2)
    {
        removeNodeOnLine(d);
    }
}