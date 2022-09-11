#ifndef QCM_WINDOW_H
#define QCM_WINDOW_H

#include <QtWidgets>
#include <QLabel>
#include <QComboBox>

#include "qcm.h"
#include "qcmodel.h"
#include "qcm_view.h"
#include "qcm_scene.h"
#include "qcm_node.h"
#include "qcm_line.h"

#include "qcm_pid.h"

class QCM_Window : public QMainWindow
{
    Q_OBJECT
public:
    QCM_Window(QWidget *parent = nullptr);
    ~QCM_Window();

private:
    QCM_View *m_view;
    QCM_Scene *m_scene;
    QLabel *m_poslabel;
    QList<QAbstractButton *> m_btns;
    QVBoxLayout *m_vboxlayout;
    QCM_Node *m_node;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *toolMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *toolToolBar;
    QToolBar *optionsToolBar;
    QToolBar *helpToolBar;

    QAction *addLineAction;
    QAction *deleteLinesAction;
    QAction *escAction;
    QAction *exitAction;
    QAction *perferenceseAction;

    void createAction();
    void createMenu();
    void createToolBar();

private slots:
    void viewMouseMoved(QPointF pos);
    void paperSizeChanged(int index);
    void gridChanged(int index);

    void addLine();
    void esc();
    void perferencese();
};

#endif // QCM_WINDOW_H
