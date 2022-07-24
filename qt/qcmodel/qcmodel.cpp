#include "qcmodel.h"

QCModel::~QCModel()
{
}

QCModel::QCModel(QStringList AI, QStringList AO, QStringList DI, QStringList DO, uint ID, QString name, uint width, QGraphicsItem *parent)
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
    setPos(cmodel.pos().x() + m_bodyWidth, cmodel.pos().y() + m_bodyWidth);
}

void QCModel::initModel()
{
    setData(QCM::ITEM_CLASS, QCM::MODEL);
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(3);
    setPen(pen);

    uint _i = m_AI.count() + m_DI.count();
    uint _o = m_AO.count() + m_DO.count();

    setRect(QCM::IOLen, 0, m_bodyWidth, ((_i > _o ? _i : _o) + 1) * QCM::IOGrap);
    setFlags(QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemIsSelectable);

    m_nameText = new QGraphicsTextItem(this);
    m_nameText->setPlainText(m_name + QString::number(m_ID));
    m_nameText->setPos(QCM::IOLen, ((_i > _o ? _i : _o) + 1) * QCM::IOGrap);

    for (uint i = 0; i < _i; i++)
    {
        if (i < m_AI.count())
        {
            m_ios.append(new QCM_IO(QCM::AI, m_AI.at(i), i + 1, this, m_bodyWidth));
        }
        else
        {
            m_ios.append(new QCM_IO(QCM::DI, m_DI.at(i - m_AI.count()), i + 1, this, m_bodyWidth));
        }
    }

    for (uint i = 0; i < _o; i++)
    {
        if (i < m_AI.count())
        {
            m_ios.append(new QCM_IO(QCM::AO, m_AO.at(i), i + 1, this, m_bodyWidth));
        }
        else
        {
            m_ios.append(new QCM_IO(QCM::DO, m_DO.at(i - m_AO.count()), i + 1, this, m_bodyWidth));
        }
    }
}

QCModel *QCModel::copy()
{
    return (new QCModel(*this));
}
