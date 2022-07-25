#ifndef QCM_IO_H
#define QCM_IO_H

#include "qcm.h"
#include "qcm_node.h"

class QCM_IO : public QGraphicsLineItem
{
public:
    QCM_IO(QCM::IO_TYPE type, QString name, uint idx, QGraphicsItem *parent = nullptr, uint width = 0);
    ~QCM_IO() {}

    uint idxWidth() const { return m_idxWidth; }
    uint idxHeight() const { return m_idxHeight; }
    uint nameWidth() const { return m_nameWidth; }
    uint nameHeight() const { return m_nameHeight; }

private:
    QGraphicsTextItem *m_idx, *m_name;
    uint m_idxWidth;
    uint m_idxHeight;
    uint m_nameWidth;
    uint m_nameHeight;
    QCM_Node *m_node;
};

#endif // QCM_IO_H