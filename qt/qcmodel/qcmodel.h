#ifndef QCM_MODEL_H
#define QCM_MODEL_H

#include <QGraphicsItem>

namespace QCM
{
    static const unsigned int io_len = 30, io_grap = 30;
    class QCModel;
    enum IO_TYPE
    {
        AI,
        AO,
        DI,
        DO
    };

}

class QCModel
{
    class QIO;

public:
    QCModel(QStringList AI, QStringList AO, QStringList DI, QStringList DO, QString name = "model", unsigned int width = 100, QGraphicsItem *parent = nullptr);
    ~QCModel();
    QGraphicsRectItem *body() const { return m_body; }

private:
    unsigned char m_numAI, m_numAO, m_numDI, m_numDO;
    unsigned int m_bodyWidth;
    QList<QIO *> m_ios;
    QGraphicsRectItem *m_body;
    QGraphicsTextItem *m_name;
};

#endif // QCM_MODEL_H
