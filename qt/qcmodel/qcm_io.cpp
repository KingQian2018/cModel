#include "qcm_io.h"

#include <QtGui/qtextdocument.h>
#include <QtGui/qabstracttextdocumentlayout.h>

QCM_IO::QCM_IO(QCM::IO_TYPE type, QString name, uint idx, QGraphicsItem *parent, uint width) : QGraphicsLineItem(parent)
{
    m_idx = new QGraphicsTextItem(this);
    m_idx->setPlainText(QString::number(idx));

    m_idxWidth = m_idx->document()->documentLayout()->documentSize().width();
    m_idxHeight = m_idx->document()->documentLayout()->documentSize().height();

    m_name = new QGraphicsTextItem(this);
    m_name->setPlainText(name);
    m_nameWidth = m_name->document()->documentLayout()->documentSize().width();
    m_nameHeight = m_name->document()->documentLayout()->documentSize().height();

    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(2);
    uint _x, _y;
    _x = 0;
    _y = idx * QCM::IOGrap;

    m_node = new QCM_Node(this);
    if (type == QCM::AI)
    {
        m_idx->setPos(_x + QCM::IOLen - m_idxWidth, _y - m_idxHeight);
        m_name->setPos(_x + QCM::IOLen, _y - m_nameHeight / 2);
        m_node->setPos(_x, _y);
    }
    else if (type == QCM::AO)
    {
        _x = width + QCM::IOLen;
        m_idx->setPos(_x, _y - m_idxHeight);
        m_name->setPos(_x - m_nameWidth, _y - m_nameHeight / 2);
        m_node->setPos(_x + QCM::IOLen, _y);
    }
    else if (type == QCM::DI)
    {
        m_idx->setPos(_x + QCM::IOLen - m_idxWidth, _y - m_idxHeight);
        m_name->setPos(_x + QCM::IOLen, _y - m_nameHeight / 2);
        pen.setStyle(Qt::DashDotLine);
        m_node->setPos(_x, _y);
    }
    else if (type == QCM::DO)
    {
        _x = width + QCM::IOLen;
        m_idx->setPos(_x, _y - m_idxHeight);
        m_name->setPos(_x - m_nameWidth, _y - m_nameHeight / 2);
        pen.setStyle(Qt::DashDotLine);
        m_node->setPos(_x + QCM::IOLen, _y);
    }
    setPen(pen);
    setLine(_x, _y, _x + QCM::IOLen, _y);
}
