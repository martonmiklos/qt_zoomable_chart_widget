#ifndef ZOOMABLECHARTWIDGET_H
#define ZOOMABLECHARTWIDGET_H

#include "zoomablechart.h"
#include "zoomablechartview.h"

#include <QRubberBand>
#include <QWidget>

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QT_CHARTS_USE_NAMESPACE
#endif

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
    ZoomableChartView *chartView() const;

    void addToolWidget(QWidget *widget);

public slots:
    void setSeriesVisible(QAbstractSeries *series, bool visible = true);

private slots:
    void on_comboBoxZoomMode_activated(int index);
    void legendMarkerClicked();
    void legendMarkerHovered(bool hover);
    void seriesAdded(QAbstractSeries *series);
    void seriesRemoved(QAbstractSeries *series);
    void on_toolButtonFitInView_clicked();

private:
    Ui::ZoomableChartWidget *ui;
    ZoomableChart *m_chart = nullptr;
};

#endif // ZOOMABLECHARTWIDGET_H
