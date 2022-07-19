#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iconhelper.h>
#include <frmrealtimedata.h>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    qcm.start();
    ui->setupUi(this);
    this->initForm();
    this->initNav();
    this->initIcon();
    ui->stackedWidget->addWidget(new frmRealtimeData);
}

MainWindow::~MainWindow()
{
    qcm.stop();
    qcm.quit();
    qcm.wait();
    delete ui;
}

QList<QColor> MainWindow::colors = QList<QColor>();

void MainWindow::showEvent(QShowEvent *)
{
    //滚动到底部
    QScrollBar *bar = ui->scrollArea->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void MainWindow::initWidgets()
{
    ui->stackedWidget->addWidget(new frmRealtimeData);
}

void MainWindow::initForm()
{
    //颜色集合供其他界面使用
    colors << QColor(211, 78, 78) << QColor(29, 185, 242) << QColor(170, 162, 119) << QColor(255, 192, 1);
    colors << QColor(0, 176, 180) << QColor(0, 113, 193) << QColor(255, 192, 0);
    colors << QColor(72, 103, 149) << QColor(185, 87, 86) << QColor(0, 177, 125);
    colors << QColor(214, 77, 84) << QColor(71, 164, 233) << QColor(34, 163, 169);
    colors << QColor(59, 123, 156) << QColor(162, 121, 197) << QColor(72, 202, 245);
    colors << QColor(0, 150, 121) << QColor(111, 9, 176) << QColor(250, 170, 20);

    ui->scrollArea->setFixedWidth(170);
    ui->widgetLeft->setProperty("flag", "left");
}

void MainWindow::initNav()
{    
    //按钮文字集合
    QStringList names;
    names << "实时曲线";

    //自动生成按钮
    for (int i = 0; i < names.count(); i++) {
        QToolButton *btn = new QToolButton;
        //设置按钮固定高度
        btn->setFixedHeight(35);
        //设置按钮的文字
        btn->setText(QString("%1. %2").arg(i + 1, 2, 10, QChar('0')).arg(names.at(i)));
        //设置按钮可选中按下类似复选框的功能
        btn->setCheckable(true);
        //设置按钮图标在左侧文字在右侧
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        //设置按钮拉伸策略为横向填充
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        //关联按钮单击事件
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
        //将按钮加入到布局
        ui->widgetLeft->layout()->addWidget(btn);
        //可以指定不同的图标
        icons << 0xf061;
        btns << btn;
    }

    //底部加个弹簧
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->widgetLeft->layout()->addItem(verticalSpacer);
    btns.at(names.count() - 1)->click();
}

void MainWindow::initIcon()
{
    //左侧导航样式,可以设置各种牛逼的参数,超级棒
    IconHelper::StyleColor styleColor;
    styleColor.defaultBorder = true;
    IconHelper::setStyle(ui->widgetLeft, btns, icons, styleColor);
}

void MainWindow::buttonClicked()
{
    QAbstractButton *b = (QAbstractButton *)sender();
    int count = btns.count();
    int index = btns.indexOf(b);
    // ui->stackedWidget->setCurrentIndex(index);

    for (int i = 0; i < count; i++) {
        QAbstractButton *btn = btns.at(i);
        btn->setChecked(btn == b);
    }

}
