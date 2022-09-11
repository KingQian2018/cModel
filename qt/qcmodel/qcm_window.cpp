#include "qcm_window.h"
#include "dialog/qcm_dialog.h"

QCM_Scene *g_scene;

QCM_Window::QCM_Window(QWidget *parent) : QMainWindow(parent)
{
    m_scene = new QCM_Scene();
    m_scene->setParperSize(QCM::A4Paper);
    g_scene = m_scene;
    m_view = new QCM_View(m_scene);
    m_view->setDragMode(QGraphicsView::RubberBandDrag);
    m_view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    m_view->setContextMenuPolicy(Qt::ActionsContextMenu);
    m_poslabel = new QLabel(m_view);
    m_poslabel->setGeometry(0, 0, 100, 20);

    setCentralWidget(m_view);
    {
        auto selectGrid = new QComboBox(m_view);
        selectGrid->addItems(QStringList({"10", "20", "30", "40", "50", "60"}));
        selectGrid->setGeometry(160, 0, 50, 20);
        selectGrid->setCurrentIndex(2);
        connect(selectGrid, SIGNAL(currentIndexChanged(int)), this, SLOT(gridChanged(int)));
    }

    connect(m_view, SIGNAL(posChanged(QPointF)), this, SLOT(viewMouseMoved(QPointF)));
    createAction();
    createToolBar();
    createMenu();
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
    m_scene->setParperSize(QCM::DefaultPapers[index][0]);
}

void QCM_Window::gridChanged(int index)
{
    QCM::SetGrid((index + 1) * 10);
}

#include "lua.hpp"

static int addPID(lua_State *L)
{
    long long a, b;
    a = lua_tointeger(L, 1); //获取函数第一个参数
    b = lua_tointeger(L, 2); //获取函数第二个参数
    g_scene->addItem(new QCM_PID(1));
    return 1;
}

void registerLua(lua_State *L)
{
    lua_register(L, "addPID", addPID);
}

void QCM_Window::createAction()
{
    addLineAction = new QAction(tr("Add Line"));
    addLineAction->setStatusTip(tr("Add Line"));
    connect(addLineAction, SIGNAL(triggered()), this, SLOT(addLine()));

    deleteLinesAction = new QAction(tr("Delete Lines"));
    deleteLinesAction->setStatusTip(tr("Delete Lines"));
    connect(deleteLinesAction, SIGNAL(triggered()), m_scene, SLOT(deleteLines()));

    escAction = new QAction(tr("Esc"));
    escAction->setShortcut(tr("ESC"));
    connect(escAction, SIGNAL(triggered()), this, SLOT(esc()));

    exitAction = new QAction(tr("E&xit"));
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    perferenceseAction = new QAction(tr("&Preferences..."));
    perferenceseAction->setStatusTip(tr("Preferences Setting"));
    connect(perferenceseAction, SIGNAL(triggered()), this, SLOT(perferencese()));
}

void QCM_Window::createMenu()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    editMenu = menuBar()->addMenu(tr("&Edit"));
    toolMenu = menuBar()->addMenu(tr("&Tool"));
    optionsMenu = menuBar()->addMenu(tr("&Options"));
    helpMenu = menuBar()->addMenu(tr("&Help"));

    fileMenu->addAction(exitAction);

    editMenu->addAction(addLineAction);
    editMenu->addAction(deleteLinesAction);
    editMenu->addAction(escAction);

    optionsMenu->addAction(perferenceseAction);
}

void QCM_Window::createToolBar()
{
    fileToolBar = addToolBar(tr("&File"));
    editToolBar = addToolBar(tr("&Edit"));
    toolToolBar = addToolBar(tr("&Tool"));
    optionsToolBar = addToolBar(tr("&Options"));
    helpToolBar = addToolBar(tr("&Help"));

    fileToolBar->addAction(exitAction);

    editToolBar->addAction(addLineAction);
    editToolBar->addAction(deleteLinesAction);
    editToolBar->addAction(escAction);

    optionsToolBar->addAction(perferenceseAction);
}

void QCM_Window::addLine()
{
    m_view->setLink(true);
}

void QCM_Window::perferencese()
{
    PerferencesDialog *dialog = new PerferencesDialog((int)m_scene->parperSize().type);
    if (dialog->exec() == QDialog::Accepted)
    {
        m_scene->setParperSize(QCM::DefaultPapers[dialog->parperSize()][0]);
    }
}

void QCM_Window::esc()
{
    m_view->setLink(false);
}
