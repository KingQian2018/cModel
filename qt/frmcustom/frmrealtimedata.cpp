﻿#include "frmrealtimedata.h"
#include "ui_frmrealtimedata.h"
#include "qdebug.h"

#include "cm_log.h"
#include "cm_parse.h"
#include "io_port.h"

frmRealtimeData::frmRealtimeData(QWidget *parent) : QWidget(parent), ui(new Ui::frmRealtimeData)
{
    ui->setupUi(this);
    cm_cnt = 1;
    CModel cm = cm_getModelByID(cm_cnt);
    while (cm != NULL)
    {
        cm_showAll(cm);
        cm_cnt++;
        cm = cm_getModelByID(cm_cnt);
    }
    this->initForm();
}

frmRealtimeData::~frmRealtimeData()
{
    delete ui;
}

void frmRealtimeData::showEvent(QShowEvent *)
{
    timeStart = QTime::currentTime();
    dataTimer.start(0);
}

void frmRealtimeData::hideEvent(QHideEvent *)
{
    dataTimer.stop();
}

void frmRealtimeData::initForm()
{
    // include this section to fully disable antialiasing for higher performance:
    ui->customPlot->setNotAntialiasedElements(QCP::aeAll);
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables | QCP::iMultiSelect);
    QFont font;
    font.setStyleStrategy(QFont::NoAntialias);
    ui->customPlot->xAxis->setTickLabelFont(font);
    ui->customPlot->yAxis->setTickLabelFont(font);
    ui->customPlot->legend->setFont(font);

    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->customPlot->addGraph(); // red line
    // ui->customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->customPlot->graph(1)->setPen(QPen(QBrush(QColor(255, 110, 40)), 3));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTicker);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(-1.2, 1.2);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));

    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(mousePress(QMouseEvent *)));
}

void frmRealtimeData::realtimeDataSlot()
{
    // calculate two new data points:
    double key = timeStart.msecsTo(QTime::currentTime()) / 1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key - lastPointKey > 0.002)
    { // at most add point every 2 ms
        // add data to lines:
        // ui->customPlot->graph(0)->addData(key, qSin(key) + std::rand() / (double)RAND_MAX * 1 * qSin(key / 0.3843));
        // ui->customPlot->graph(1)->addData(key, qCos(key) + std::rand() / (double)RAND_MAX * 0.5 * qSin(key / 0.4364));
        for (unsigned char i = 0; i < cm_cnt; i++)
        {
            CModel cmShow = cm_getModelByID(i);
            if (cmShow != NULL)
            {
                if (cmShow->id == 1)
                {
                    a_value av = IO_GetAValue(cmShow->io, IOPIN_1, IOTYP_AO);
                    ui->customPlot->graph(0)->addData(key, av);
                }
                else if (cmShow->id == 7)
                {
                    a_value av = IO_GetAValue(cmShow->io, IOPIN_1, IOTYP_AO);
                    ui->customPlot->graph(1)->addData(key, av);
                }
            }
        }
        // rescale value (vertical) axis to fit the current data:
        // ui->customPlot->graph(0)->rescaleValueAxis();
        // ui->customPlot->graph(1)->rescaleValueAxis(true);

        lastPointKey = key;
    }

    // make key axis range scroll with the data (at a constant range size of 8):
    // ui->customPlot->xAxis->setRange(key, 50, Qt::AlignRight);
    ui->customPlot->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key - lastFpsKey > 2)
    { // average fps over 2 seconds
        QString fps = QString("%1").arg(frameCount / (key - lastFpsKey), 0, 'f', 0);
        int count = ui->customPlot->graph(0)->data()->size() + ui->customPlot->graph(1)->data()->size();
        QString info = QString("%1 FPS, Total Data points: %2").arg(fps).arg(count);
        ui->label->setText(info);
        lastFpsKey = key;
        frameCount = 0;
    }
}

void frmRealtimeData::mousePress(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        ui->customPlot->setSelectionRectMode(QCP::srmNone);
    }
    else
    {
        ui->customPlot->setSelectionRectMode(QCP::srmZoom);
    }
}
