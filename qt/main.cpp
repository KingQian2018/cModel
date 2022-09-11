#include "mainwindow.h"

#include <QApplication>
#include <QTextCodec>
#include <QFontDatabase>
#include <QTime>
#include <QDebug>
#include <QTranslator>

#include "qcmodel/qcmodel.h"
#include "qcmodel/qcm_window.h"

int main(int argc, char *argv[])
{
    // qcm_elog_init();
    // qcm_init();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QApplication::setAttribute(Qt::AA_Use96Dpi);
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Floor);
#endif
    QApplication a(argc, argv);
    QFont font;
    font.setFamily("Microsoft Yahei");
    font.setPixelSize(13);
    a.setFont(font);

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
    //初始化随机数种子
    QTime t = QTime::currentTime();
    srand(t.msec() + t.second() * 1000);

    QTranslator *translator=new QTranslator();
    translator->load("study3_zh_CN.qm");
    a.installTranslator(translator);

    // MainWindow w(argc, argv);
    // w.resize(1000, 650);
    // w.show();
    QCM_Window w;
    w.resize(1000, 650);
    w.show();
    return a.exec();
}
