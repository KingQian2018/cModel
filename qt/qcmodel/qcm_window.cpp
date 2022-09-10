#include "qcm_window.h"

QCM_Scene *g_scene;

QCM_Window::QCM_Window(QWidget *parent) : QWidget(parent)
{
    m_scene = new QCM_Scene();
    m_scene->setParperSize(QCM::A4Paper);
    g_scene = m_scene;
    m_view = new QCM_View(m_scene);
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
        auto selectGrid = new QComboBox(m_view);
        selectGrid->addItems(QStringList({"10", "20", "30", "40", "50", "60"}));
        selectGrid->setGeometry(160, 0, 50, 20);
        selectGrid->setCurrentIndex(2);
        connect(selectGrid, SIGNAL(currentIndexChanged(int)), this, SLOT(gridChanged(int)));
    }

    {
        QToolButton *toolBtn = new QToolButton(m_view);
        toolBtn->setText("Link");
        toolBtn->setGeometry(220, 0, 60, 20);
        m_btns.append(toolBtn);
        connect(toolBtn, SIGNAL(clicked()), this, SLOT(btnClicked()));
        toolBtn = new QToolButton(m_view);
        toolBtn->setText("PID");
        toolBtn->setGeometry(290, 0, 60, 20);
        m_btns.append(toolBtn);
        connect(toolBtn, SIGNAL(clicked()), this, SLOT(btnClicked()));
        QLineEdit *lineEdit = new QLineEdit(m_view);
        lineEdit->setGeometry(360, 0, 200, 20);
        connect(lineEdit, SIGNAL(), this, SLOT());
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
    m_scene->setParperSize(QCM::DefaultPapers[index][0]);
}

void QCM_Window::gridChanged(int index)
{
    QCM::SetGrid((index + 1) * 10);
}

void QCM_Window::btnClicked()
{
    QAbstractButton *b = (QAbstractButton *)sender();
    if (b->text() == "PID")
    {
        QCM_Node *node3 = new QCM_Node(QCM::AlignToGrid(30, 90));
        for (auto l : m_scene->items())
        {
            if (l->type() == QGraphicsItem::UserType + QCM::LINE && l->isSelected())
            {
                ((QCM_Line *)l)->addNode(node3);
            }
        }
        m_scene->addItem(new QCM_PID(1));
    }
    else if (b->text() == "Link")
    {
        QCM_Node *node1 = new QCM_Node(QCM::AlignToGrid(100, 0));
        QCM_Node *node2 = new QCM_Node(QCM::AlignToGrid(30, 30));
        m_scene->addItem(node1);
        m_scene->addItem(node2);
        auto _line = new QCM_Line(node1, node2);
        m_scene->addItem(_line);
    }
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