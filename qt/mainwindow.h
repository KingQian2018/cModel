#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <QLabel>
#include <QComboBox>
#include <QAction>

#include "qcmodel/qcm_window.h"
#include "qcmodel/qcm_lua.h"

class QAbstractButton;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(int argc, char *argv[], QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void showEvent(QShowEvent *);

private:
    Ui::MainWindow *ui;
    static QList<QColor> colors;
    QCM_Lua *m_lua;

    QList<int> icons;
    QList<QAbstractButton *> btns;

    QAction *openAction;

private slots:
    void initWidgets();
    void initNav();       //初始化导航按钮
    void initIcon();      //初始化导航按钮图标
    void buttonClicked(); //导航按钮单击事件
    void initForm();
    void initMenu();
};
#endif // MAINWINDOW_H
