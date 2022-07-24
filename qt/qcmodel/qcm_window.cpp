#include "qcm_window.h"

QCM_Window::QCM_Window(QWidget *parent) : QWidget(parent)
{
    QCM_Scene *_scene = new QCM_Scene();
    QCM_PID *_pid = new QCM_PID(1);

    _scene->addItem(_pid);
    
    _scene->setParperSize(QCM::A3Paper);
    m_view = new QCM_View(_scene, this);
    m_poslabel = new QLabel(m_view);
    m_poslabel->setGeometry(0, 0, 100, 20);

    m_select = new QComboBox(m_view);
    QStringList selectStr;
    selectStr << "A2"
              << "A3"
              << "A4";
    m_select->addItems(selectStr);
    m_select->setGeometry(120, 0, 50, 20);
    connect(m_select, SIGNAL(currentIndexChanged(int)), this, SLOT(paperSizeChanged(int)));

    connect(m_view, SIGNAL(posChanged(QPointF)), this, SLOT(viewMouseMoved(QPointF)));
}

QCM_Window::~QCM_Window()
{
}

void QCM_Window::viewMouseMoved(QPointF pos)
{
    m_poslabel->setText(QString("x:%1 y:%2").arg((int)pos.x()).arg((int)pos.y()));
}

void QCM_Window::paperSizeChanged(int index)
{
    QCM_Scene *scene = (QCM_Scene *)(m_view->scene());
    scene->setParperSize(QCM::DefaultPapers[index][0]);
}
