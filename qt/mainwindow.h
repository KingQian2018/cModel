#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <QLabel>
#include <QComboBox>

#include "qcmodel/qcm_window.h"

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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void showEvent(QShowEvent *);

private:
    Ui::MainWindow *ui;
    static QList<QColor> colors;

    QList<int> icons;
    QList<QAbstractButton *> btns;

private slots:
    void initWidgets();
    void initNav();       //初始化导航按钮
    void initIcon();      //初始化导航按钮图标
    void buttonClicked(); //导航按钮单击事件
    void initForm();
};
#endif // MAINWINDOW_H
