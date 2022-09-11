#ifndef QCM_SCENE_H
#define QCM_SCENE_H

#include "qcm.h"
#include "qcmodel.h"
#include "qcm_line.h"
#include "qcm_nodeLine.h"

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

    QCM_PreLine *preLine() const { return m_preLine; }
    void linkClicked();
    void linkMove(bool, QPointF);

private:
    QCM::PaperSize_s m_parperSize = QCM::A4Paper;
    QMenu m_menu;
    QCM_PreLine *m_preLine;
    QList<QCM_NodeLine *> m_lines;
    int m_linkCnt = 0;

    void drawBackground(QPainter *painter, const QRectF &rect);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

public slots:
    void deleteLines();

private slots:
    void copyModelEvent();
    void deleteModelEvent();
};

#endif // QCM_SCENE_H
