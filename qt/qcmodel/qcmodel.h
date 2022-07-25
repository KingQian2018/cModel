#ifndef QCM_MODEL_H
#define QCM_MODEL_H

#include "qcm.h"
#include "qcm_io.h"

class QCModel : public QGraphicsRectItem
{

public:
    QCModel(const QCModel &);
    QCModel(QStringList AI, QStringList AO, QStringList DI, QStringList DO, uint ID,
            QString name = "model", uint width = 100, QGraphicsItem *parent = nullptr);
    ~QCModel();

    uint ID() const { return m_ID; }
    virtual QCModel *copy();

protected:
    uint m_ID;

private:
    QStringList m_AI, m_AO, m_DI, m_DO;
    uint m_bodyWidth;
    QList<QCM_IO *> m_ios;
    QString m_name;
    QGraphicsTextItem *m_nameText;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void initModel();
};

#endif // QCM_MODEL_H
