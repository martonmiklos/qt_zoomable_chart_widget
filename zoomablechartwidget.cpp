#include "zoomablechartwidget.h"
#include "ui_zoomablechartwidget.h"

#include <QtCharts/QLegendMarker>

QT_CHARTS_USE_NAMESPACE


ZoomableChartWidget::ZoomableChartWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZoomableChartWidget)
{
    ui->setupUi(this);

    QIcon panIcon;
    panIcon.addFile(QStringLiteral(":/icons/pan.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->comboBoxZoomMode->addItem(panIcon, QString(""), ZoomableChartView::Pan);

    QIcon rectZoomIcon;
    rectZoomIcon.addFile(QStringLiteral(":/icons/rectangle_zoom.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->comboBoxZoomMode->addItem(rectZoomIcon, QString(""), ZoomableChartView::RectangleZoom);

    QIcon vZoomIcon;
    vZoomIcon.addFile(QStringLiteral(":/icons/vertical_zoom.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->comboBoxZoomMode->addItem(vZoomIcon, QString(""), ZoomableChartView::VerticalZoom);

    QIcon hZoomIcon;
    hZoomIcon.addFile(QStringLiteral(":/icons/horizontal_zoom.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->comboBoxZoomMode->addItem(hZoomIcon, QString(""), ZoomableChartView::HorizontalZoom);

    ui->chartView->setZoomMode(ZoomableChartView::Pan);

    m_chart = new ZoomableChart();
    ui->chartView->setChart(m_chart);
    connect(m_chart, &ZoomableChart::seriesAdded, this, &ZoomableChartWidget::seriesAdded);
    connect(m_chart, &ZoomableChart::seriesRemoved, this, &ZoomableChartWidget::seriesRemoved);
}

ZoomableChartWidget::~ZoomableChartWidget()
{
    delete m_chart;
    delete ui;
}

void ZoomableChartWidget::on_comboBoxZoomMode_activated(int index)
{
    ui->chartView->setZoomMode(static_cast<ZoomableChartView::ZoomMode>(ui->comboBoxZoomMode->itemData(index).toInt()));
}

void ZoomableChartWidget::legendMarkerClicked()
{
    auto* marker = qobject_cast<QLegendMarker*>(sender());
    Q_ASSERT(marker);

    // Toggle visibility of series
    setSeriesVisible(marker->series(), !marker->series()->isVisible());
}

void ZoomableChartWidget::legendMarkerHovered(bool hover)
{
    auto* marker = qobject_cast<QLegendMarker*>(sender());
    Q_ASSERT(marker);

    QFont font = marker->font();
    font.setBold(hover);
    marker->setFont(font);

    if (marker->series()->type() == QAbstractSeries::SeriesTypeLine) {
        auto *series = qobject_cast<QLineSeries*>(marker->series());
        auto pen = series->pen();
        pen.setWidth(hover ? (pen.width() * 2) : (pen.width() / 2));
        series->setPen(pen);
    }
}

ZoomableChart *ZoomableChartWidget::chart() const
{
    return m_chart;
}

void ZoomableChartWidget::setSeriesVisible(QAbstractSeries *series, bool visible)
{
    series->setVisible(visible);
    for (QLegendMarker *marker : m_chart->legend()->markers(series)) {
        // Turn legend marker back to visible, since hiding series also hides the marker
        // and we don't want it to happen now.
        marker->setVisible(true);

        // Dim the marker, if series is not visible
        qreal alpha = visible ? 1.0 : 0.5;
        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);

        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);
    }

    for (QAbstractAxis *axis : m_chart->axes(Qt::Vertical)) {
        bool hideAxis = true;
        for (QAbstractSeries *series : m_chart->series()) {
            for (QAbstractAxis *attachedAxis : series->attachedAxes()) {
                if (series->isVisible() && attachedAxis == axis) {
                    hideAxis = false;
                    break;
                }
            }
            if (!hideAxis)
                break;
        }
        axis->setVisible(!hideAxis);
    }
}

void ZoomableChartWidget::seriesAdded(QAbstractSeries *series)
{
    // Connect all markers to handler
    const auto markers = m_chart->legend()->markers(series);
    for (auto *marker : markers) {
        QObject::connect(marker, &QLegendMarker::clicked,
                         this, &ZoomableChartWidget::legendMarkerClicked);
        QObject::connect(marker, &QLegendMarker::hovered,
                         this, &ZoomableChartWidget::legendMarkerHovered);
    }
}

void ZoomableChartWidget::seriesRemoved(QAbstractSeries *series)
{
    // Connect all markers to handler
    const auto markers = m_chart->legend()->markers(series);
    for (auto *marker : markers) {
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &ZoomableChartWidget::legendMarkerClicked);
        QObject::disconnect(marker, &QLegendMarker::hovered,
                         this, &ZoomableChartWidget::legendMarkerHovered);
    }
}

