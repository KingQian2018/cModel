#ifndef QCM_H
#define QCM_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QtGui/qpen.h>

namespace QCM
{
    static const unsigned int IOLen = 30, IOGrap = 30;

    enum Paper_TYPE
    {
        A2,
        A3,
        A4,
    };

    typedef struct _paperSize
    {
        Paper_TYPE type;
        unsigned int width, height;
    } PaperSize_s;
    static const PaperSize_s A2Paper = {.type = A2, .width = 5940, .height = 4200};
    static const PaperSize_s A3Paper = {.type = A3, .width = 4200, .height = 2970};
    static const PaperSize_s A4Paper = {.type = A4, .width = 2970, .height = 2100};
    static const PaperSize_s *DefaultPapers[] = {&A2Paper, &A3Paper, &A4Paper};

    class QCModel;
    enum IO_TYPE
    {
        AI,
        AO,
        DI,
        DO
    };
}

#endif // QCM_H
