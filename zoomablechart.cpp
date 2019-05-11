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
