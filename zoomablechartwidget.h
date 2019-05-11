#ifndef ZOOMABLECHARTWIDGET_H
#define ZOOMABLECHARTWIDGET_H

#include "zoomablechart.h"

#include <QRubberBand>
#include <QWidget>

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class ZoomableChartWidget;
}

class ZoomableChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZoomableChartWidget(QWidget *parent = nullptr);
    ~ZoomableChartWidget();

    ZoomableChart *chart() const;

public slots:
    void setSeriesVisible(QAbstractSeries *series, bool visible = true);

private slots:
    void on_comboBoxZoomMode_activated(int index);
    void legendMarkerClicked();
    void legendMarkerHovered(bool hover);
    void seriesAdded(QAbstractSeries *series);
    void seriesRemoved(QAbstractSeries *series);

private:
    Ui::ZoomableChartWidget *ui;
    ZoomableChart *m_chart = nullptr;
};

#endif // ZOOMABLECHARTWIDGET_H
