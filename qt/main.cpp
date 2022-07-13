#include "mainwindow.h"

#include <QApplication>
#include <QTextCodec>
#include <QFontDatabase>
#include <QTime>
#include <QDebug>

#include "cm_log.h"
#include "cm_parse.h"
#include "io_port.h"

#include "pid.h"
#include "const.h"
#include "translate.h"
#include "limit.h"

void cm_elog_init()
{
    elog_init();
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_set_text_color_enabled(true);
    elog_start();
}

int main(int argc, char *argv[])
{
    cm_elog_init();
    if (parse_file("../tests/parse_test/cmodel.json") != CMODEL_STATUS_OK)
    {
        LOG_E("prase failed.");
    }

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

    MainWindow w;
    w.resize(1000, 650);
    w.show();
    return a.exec();
}
