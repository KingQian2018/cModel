#include "qcmodel.h"

#include <QtGui/qtextdocument.h>
#include <QtGui/qabstracttextdocumentlayout.h>

QIO::QIO(QCM::IO_TYPE type, QString name, unsigned char idx, QGraphicsItem *parent, unsigned int width) : QGraphicsLineItem(parent)
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
    unsigned char _x, _y;
    _x = 0;
    _y = idx * QCM::IOGrap;
    if (type == QCM::AI)
    {
        m_idx->setPos(_x + QCM::IOLen - m_idxWidth, _y - m_idxHeight);
        m_name->setPos(_x + QCM::IOLen, _y - m_nameHeight / 2);
    }
    else if (type == QCM::AO)
    {
        _x = width + QCM::IOLen;
        m_idx->setPos(_x, _y - m_idxHeight);
        m_name->setPos(_x - m_nameWidth, _y - m_nameHeight / 2);
    }
    else if (type == QCM::DI)
    {
        m_idx->setPos(_x + QCM::IOLen - m_idxWidth, _y - m_idxHeight);
        m_name->setPos(_x + QCM::IOLen, _y - m_nameHeight / 2);
        pen.setStyle(Qt::DashDotLine);
    }
    else if (type == QCM::DO)
    {
        _x = width + QCM::IOLen;
        m_idx->setPos(_x, _y - m_idxHeight);
        m_name->setPos(_x - m_nameWidth, _y - m_nameHeight / 2);
        pen.setStyle(Qt::DashDotLine);
    }
    setPen(pen);
    setLine(_x, _y, _x + QCM::IOLen, _y);
}

QCModel::~QCModel()
{
}

QCModel::QCModel(QStringList AI, QStringList AO, QStringList DI, QStringList DO, uint ID, QString name, unsigned int width, QGraphicsItem *parent)
    : m_AI(AI), m_AO(AO), m_DI(DI), m_DO(DO),
      m_ID(ID), m_name(name), m_bodyWidth(width), QGraphicsRectItem(parent)
{
    initModel();
}

QCModel::QCModel(const QCModel &cmodel)
    : m_AI(cmodel.m_AI), m_AO(cmodel.m_AO), m_DI(cmodel.m_DI), m_DO(cmodel.m_DO),
      m_ID(cmodel.m_ID + 1), m_name(cmodel.m_name), m_bodyWidth(cmodel.m_bodyWidth), QGraphicsRectItem(cmodel.parentItem())
{
    initModel();
    setPos(cmodel.pos().x() + 100, cmodel.pos().y() + 100);
}

void QCModel::initModel()
{
    unsigned int _i = m_AI.count() + m_DI.count();
    unsigned int _o = m_AO.count() + m_DO.count();

    setRect(QCM::IOLen, 0, m_bodyWidth, ((_i > _o ? _i : _o) + 1) * QCM::IOGrap);
    setFlags(QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemIsSelectable);

    m_nameText = new QGraphicsTextItem(this);
    m_nameText->setPlainText(m_name + QString::number(m_ID));
    m_nameText->setPos(QCM::IOLen, ((_i > _o ? _i : _o) + 1) * QCM::IOGrap);

    for (unsigned int i = 0; i < _i; i++)
    {
        if (i < m_AI.count())
        {
            m_ios.append(new QIO(QCM::AI, m_AI.at(i), i + 1, this, m_bodyWidth));
        }
        else
        {
            m_ios.append(new QIO(QCM::DI, m_DI.at(i - m_AI.count()), i + 1, this, m_bodyWidth));
        }
    }

    for (unsigned int i = 0; i < _o; i++)
    {
        if (i < m_AI.count())
        {
            m_ios.append(new QIO(QCM::AO, m_AO.at(i), i + 1, this, m_bodyWidth));
        }
        else
        {
            m_ios.append(new QIO(QCM::DO, m_DO.at(i - m_AO.count()), i + 1, this, m_bodyWidth));
        }
    }
}

QCModel *QCModel::copy()
{
    return (new QCModel(*this));
}
