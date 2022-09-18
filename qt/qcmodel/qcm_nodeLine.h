#ifndef QCM_NODELINE_H
#define QCM_NODELINE_H

#include "qcm_node.h"
#include "qcm_line.h"

class QCM_NodeLine
{
public:
    QCM_NodeLine(QGraphicsScene *scene);
    ~QCM_NodeLine();

    void addNode(QCM_Node *node);
    void addNodeOnLine(QCM_Node *node, QCM_Line *line);
    void removeLine(QCM_Line *line);
    QList<QCM_Node *> nodes() const { return m_nodes; }
    QList<QCM_Line *> lines() const { return m_lines; }

private:
    QList<QCM_Node *> m_nodes;
    QList<QCM_Line *> m_lines;
    QGraphicsScene *m_scene;

    void removeNode(QCM_Node *node);
    void removeNodeOnLine(QCM_Node *node);
};

#endif
