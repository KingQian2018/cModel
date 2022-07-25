#ifndef QCM_SCENE_H
#define QCM_SCENE_H

#include "qcm.h"
#include "qcmodel.h"
#include "qcm_linknodes.h"

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

    void addLinks(QCM_LinkNodes* node) { m_links.append(node); }
    QList<QCM_LinkNodes *> links() const { return m_links; }

private:
    QCM::PaperSize_s m_parperSize = QCM::A4Paper;
    QMenu m_menu;
    QList<QCM_LinkNodes *> m_links;

    void drawBackground(QPainter *painter, const QRectF &rect);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private slots:
    void copyModelEvent();
    void deleteModelEvent();
};

#endif // QCM_SCENE_H
