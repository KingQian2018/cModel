#ifndef QCM_SCENE_H
#define QCM_SCENE_H

#include <QGraphicsScene>
#include "qcmodel.h"

namespace QCM
{
    class QCM_Scene;
}

class QCM_Scene : public QGraphicsScene
{
public:
    QCM_Scene(QObject *parent = nullptr);
    ~QCM_Scene() {}
    void setParperSize(QCM::PaperSize_s size);
    QCM::PaperSize_s parperSize(void) const { return m_parperSize; }

private:
    void drawBackground(QPainter *painter, const QRectF &rect);
    QCM::PaperSize_s m_parperSize = QCM::A4Paper;
    unsigned int m_grid = 100;
};

#endif // QCM_SCENE_H
