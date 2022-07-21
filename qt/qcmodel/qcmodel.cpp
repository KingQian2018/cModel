#include "qcmodel.h"

#include <QtGui/qpen.h>
#include <QtGui/qtextdocument.h>
#include <QtGui/qabstracttextdocumentlayout.h>

class QCModel::QIO : public QGraphicsLineItem
{
public:
    QIO(QCM::IO_TYPE type, QString name, unsigned char idx, QGraphicsItem *parent = nullptr, unsigned int width = 0) : QGraphicsLineItem(parent)
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
        _y = idx * QCM::io_grap;
        if (type == QCM::AI)
        {
            m_idx->setPos(_x + QCM::io_len - m_idxWidth, _y - m_idxHeight);
            m_name->setPos(_x + QCM::io_len, _y - m_nameHeight / 2);
        }
        else if (type == QCM::AO)
        {
            _x = width + QCM::io_len;
            m_idx->setPos(_x, _y - m_idxHeight);
            m_name->setPos(_x - m_nameWidth, _y - m_nameHeight / 2);
        }
        else if (type == QCM::DI)
        {
            m_idx->setPos(_x + QCM::io_len - m_idxWidth, _y - m_idxHeight);
            m_name->setPos(_x + QCM::io_len, _y - m_nameHeight / 2);
            pen.setStyle(Qt::DashDotLine);
        }
        else if (type == QCM::DO)
        {
            _x = width + QCM::io_len;
            m_idx->setPos(_x, _y - m_idxHeight);
            m_name->setPos(_x - m_nameWidth, _y - m_nameHeight / 2);
            pen.setStyle(Qt::DashDotLine);
        }
        setPen(pen);
        setLine(_x, _y, _x + QCM::io_len, _y);
    }
    ~QIO() {}

    unsigned int idxWidth() const { return m_idxWidth; }
    unsigned int idxHeight() const { return m_idxHeight; }
    unsigned int nameWidth() const { return m_nameWidth; }
    unsigned int nameHeight() const { return m_nameHeight; }

private:
    QGraphicsTextItem *m_idx, *m_name;
    unsigned int m_idxWidth;
    unsigned int m_idxHeight;
    unsigned int m_nameWidth;
    unsigned int m_nameHeight;
};

QCModel::~QCModel()
{
}

QCModel::QCModel(QStringList AI, QStringList AO, QStringList DI, QStringList DO, QString name, unsigned int width, QGraphicsItem *parent)
    : m_numAI(AI.count()), m_numAO(AO.count()), m_numDI(DI.count()), m_numDO(DO.count()), m_bodyWidth(width)
{
    unsigned int _i = m_numAI + m_numDI;
    unsigned int _o = m_numAO + m_numDO;

    m_body = new QGraphicsRectItem(parent);
    m_body->setRect(QCM::io_len, 0, m_bodyWidth, ((_i > _o ? _i : _o) + 1) * QCM::io_grap);
    m_body->setFlag(QGraphicsItem::ItemIsMovable);

    m_name = new QGraphicsTextItem(m_body);
    m_name->setPlainText(name);
    m_name->setPos(QCM::io_len, ((_i > _o ? _i : _o) + 1) * QCM::io_grap);

    for (unsigned int i = 0; i < _i; i++)
    {
        if (i < m_numAI)
        {
            m_ios.append(new QIO(QCM::AI, AI.at(i), i + 1, m_body, m_bodyWidth));
        }
        else
        {
            m_ios.append(new QIO(QCM::DI, DI.at(i - m_numAI), i + 1, m_body, m_bodyWidth));
        }
    }

    for (unsigned int i = 0; i < _o; i++)
    {
        if (i < m_numAI)
        {
            m_ios.append(new QIO(QCM::AO, AO.at(i), i + 1, m_body, m_bodyWidth));
        }
        else
        {
            m_ios.append(new QIO(QCM::DO, DO.at(i - m_numAO), i + 1, m_body, m_bodyWidth));
        }
    }
}
