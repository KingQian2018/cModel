#ifndef QCM_MODEL_H
#define QCM_MODEL_H

#include "qcm.h"

class QIO : public QGraphicsLineItem
{
public:
    QIO(QCM::IO_TYPE type, QString name, unsigned char idx, QGraphicsItem *parent = nullptr, unsigned int width = 0);
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

class QCModel : public QGraphicsRectItem
{

public:
    QCModel(const QCModel &);
    QCModel(QStringList AI, QStringList AO, QStringList DI, QStringList DO, uint ID,
            QString name = "model", unsigned int width = 100, QGraphicsItem *parent = nullptr);
    ~QCModel();

    uint ID() const { return m_ID; }
    virtual QCModel *copy();

protected:
    uint m_ID;

private:
    QStringList m_AI, m_AO, m_DI, m_DO;
    unsigned int m_bodyWidth;
    QList<QIO *> m_ios;
    QString m_name;
    QGraphicsTextItem *m_nameText;

    void initModel();
};

#endif // QCM_MODEL_H
