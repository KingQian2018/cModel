#ifndef QCM_SCENE_H
#define QCM_SCENE_H

#include "qcm.h"
#include "qcmodel.h"

namespace QCM
{
    class QCM_Scene;
}

class QCM_Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    QCM_Scene(QObject *parent = nullptr);
    ~QCM_Scene() {}
    void setParperSize(QCM::PaperSize_s size);
    QCM::PaperSize_s parperSize(void) const { return m_parperSize; }
    void setGrid(uint grid)
    {
        m_grid = grid;
        update();
    }
    uint grid() const { return m_grid; }

private:
    QCM::PaperSize_s m_parperSize = QCM::A4Paper;
    unsigned int m_grid = 100;
    QMenu m_menu;

    void drawBackground(QPainter *painter, const QRectF &rect);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private slots:
    void copyModelEvent();
    void deleteModelEvent();
};

#endif // QCM_SCENE_H
