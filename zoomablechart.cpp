#include "zoomablechart.h"

ZoomableChart::ZoomableChart(QGraphicsItem *parent, Qt::WindowFlags wFlags) :
    QChart(parent, wFlags)
{

}

void ZoomableChart::addSeries(QAbstractSeries *series)
{
    QChart::addSeries(series);
    emit seriesAdded(series);
}

void ZoomableChart::removeSeries(QAbstractSeries *series)
{
    QChart::removeSeries(series);
    emit seriesRemoved(series);
}

void ZoomableChart::addAxis(QAbstractAxis *axis, Qt::Alignment alignment)
{
    QChart::addAxis(axis, alignment);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 13, 0))
    axis->setLabelsEditable();
#endif
}
