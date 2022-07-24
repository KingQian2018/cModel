#include "qcm_window.h"

QCM_Window::QCM_Window(QWidget *parent) : QWidget(parent)
{
    QCM_Scene *_scene = new QCM_Scene();
    _scene->setParperSize(QCM::A4Paper);
    m_view = new QCM_View(_scene);
    m_poslabel = new QLabel(m_view);
    m_poslabel->setGeometry(0, 0, 100, 20);

    m_vboxlayout = new QVBoxLayout(this);
    m_vboxlayout->addWidget(m_view);

    m_select = new QComboBox(m_view);
    QStringList selectStr;
    selectStr << "A2"
              << "A3"
              << "A4";
    m_select->addItems(selectStr);
    m_select->setGeometry(100, 0, 50, 20);
    m_select->setCurrentIndex(2);
    connect(m_select, SIGNAL(currentIndexChanged(int)), this, SLOT(paperSizeChanged(int)));

    {
        QToolButton *toolBtn = new QToolButton(m_view);
        toolBtn->setText("PID");
        toolBtn->setGeometry(160, 0, 60, 20);
        m_btns.append(toolBtn);
        connect(toolBtn, SIGNAL(clicked()), this, SLOT(btnClicked()));

        m_nodeLine = new QCM_NodeLine();
        m_view->scene()->addItem(m_nodeLine->nodeOut());
    }

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

void QCM_Window::btnClicked()
{
    m_view->scene()->addItem(new QCM_PID(1));
}
